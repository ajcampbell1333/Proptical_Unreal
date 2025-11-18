#include "VRPNConnectionManager.h"
#include "VRPNMessageParser.h"
#include "Async/Async.h"
#include "HAL/PlatformProcess.h"

FVRPNConnectionManager::FVRPNConnectionManager()
	: ReceiveThread(nullptr)
	, bShouldStop(false)
	, bIsConnected(false)
	, ServerPort(3883)
{
}

FVRPNConnectionManager::~FVRPNConnectionManager()
{
	StopReceiving();
}

bool FVRPNConnectionManager::InitializeConnection(const FString& InServerAddress, int32 InServerPort)
{
	ServerAddress = InServerAddress;
	ServerPort = InServerPort;

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to get socket subsystem"));
		return false;
	}

	// Resolve server address
	ServerAddr = SocketSubsystem->CreateInternetAddr();
	bool bIsValid = false;
	ServerAddr->SetIp(*ServerAddress, bIsValid);
	if (!bIsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Invalid server address: %s"), *ServerAddress);
		return false;
	}
	ServerAddr->SetPort(ServerPort);

	// Setup UDP socket (primary for data reception)
	if (!SetupUDPSocket())
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to setup UDP socket"));
		return false;
	}

	// TCP handshake is low priority - may be deferred
	// For now, we'll attempt it but won't fail if it doesn't work
	PerformTCPHandshake();

	return true;
}

bool FVRPNConnectionManager::SetupUDPSocket()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		return false;
	}

	// Create UDP socket
	FSocket* RawSocket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("VRPN_UDP"), ServerAddr->GetProtocolType());
	UDPSocket = MakeShareable(RawSocket);
	if (!UDPSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to create UDP socket"));
		return false;
	}

	// Set socket to non-blocking
	UDPSocket->SetNonBlocking(true);

	// Set receive buffer size
	int32 ReceiveBufferSize = 1024 * 64; // 64KB
	UDPSocket->SetReceiveBufferSize(ReceiveBufferSize, ReceiveBufferSize);

	UE_LOG(LogTemp, Log, TEXT("VRPN: UDP socket created successfully"));
	return true;
}

bool FVRPNConnectionManager::PerformTCPHandshake()
{
	// TCP handshake is low priority for alpha
	// This is a placeholder - full implementation deferred if needed
	// For now, we'll focus on UDP data reception
	
	UE_LOG(LogTemp, Warning, TEXT("VRPN: TCP handshake not yet implemented (low priority)"));
	return false;
}

bool FVRPNConnectionManager::StartReceiving()
{
	if (ReceiveThread != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("VRPN: Receive thread already running"));
		return false;
	}

	if (!UDPSocket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: UDP socket not initialized"));
		return false;
	}

	bShouldStop = false;
	ReceiveThread = FRunnableThread::Create(this, TEXT("VRPNConnectionManager"), 0, TPri_Normal);
	
	if (ReceiveThread == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to create receive thread"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("VRPN: Started receiving data on background thread"));
	return true;
}

void FVRPNConnectionManager::StopReceiving()
{
	if (ReceiveThread != nullptr)
	{
		bShouldStop = true;
		ReceiveThread->WaitForCompletion();
		delete ReceiveThread;
		ReceiveThread = nullptr;
	}

	if (UDPSocket.IsValid())
	{
		UDPSocket->Close();
		UDPSocket.Reset();
	}

	if (TCPSocket.IsValid())
	{
		TCPSocket->Close();
		TCPSocket.Reset();
	}

	bIsConnected = false;
	UE_LOG(LogTemp, Log, TEXT("VRPN: Stopped receiving data"));
}

uint32 FVRPNConnectionManager::Run()
{
	const int32 BufferSize = 1024 * 64; // 64KB buffer
	uint8* ReceiveBuffer = new uint8[BufferSize];
	
	FDateTime ConnectionStartTime = FDateTime::Now();

	while (!bShouldStop)
	{
		if (!UDPSocket.IsValid())
		{
			FPlatformProcess::Sleep(0.01f); // 10ms sleep
			continue;
		}

		// Receive UDP packet
		int32 BytesRead = 0;
		TSharedRef<FInternetAddr> FromAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		
		if (UDPSocket->RecvFrom(ReceiveBuffer, BufferSize, BytesRead, *FromAddr))
		{
			if (BytesRead > 0)
			{
				if (!bIsConnected)
				{
					bIsConnected = true;
					// Marshal connection established event to game thread
					AsyncTask(ENamedThreads::GameThread, [this]()
					{
						OnConnectionEstablished.ExecuteIfBound();
					});
				}

				// Process received packet
				ProcessUDPPacket(ReceiveBuffer, BytesRead);
			}
		}
		else
		{
			// Check for connection errors
			ESocketErrors ErrorCode = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
			if (ErrorCode != SE_EWOULDBLOCK)
			{
				FString ErrorMsg = FString::Printf(TEXT("VRPN: UDP receive error: %d"), (int32)ErrorCode);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMsg);
				
				// Check if we should warn about firewall/network configuration
				if (ErrorCode == SE_ECONNREFUSED || ErrorCode == SE_EACCES)
				{
					ErrorMsg = TEXT("VRPN: UDP connection failed. Check firewall settings and ensure UDP port is open.");
					UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
				}

				// Marshal connection lost event to game thread
				AsyncTask(ENamedThreads::GameThread, [this, ErrorMsg]()
				{
					if (bIsConnected)
					{
						bIsConnected = false;
						OnConnectionLost.ExecuteIfBound(ErrorMsg);
					}
				});
			}

			// Small sleep to prevent busy-waiting
			FPlatformProcess::Sleep(0.001f); // 1ms
		}
	}

	delete[] ReceiveBuffer;
	return 0;
}

void FVRPNConnectionManager::Stop()
{
	bShouldStop = true;
}

void FVRPNConnectionManager::ProcessUDPPacket(const uint8* Data, int32 DataSize)
{
	// Parse VRPN message
	FVRPNTransformData TransformData;
	FString RigidBodyName;
	
	// Use FVRPNMessageParser to parse the packet
	if (FVRPNMessageParser::ParseTrackerMessage(Data, DataSize, TransformData, RigidBodyName))
	{
		// Update last transform (thread-safe)
		{
			FScopeLock Lock(&TransformDataCS);
			LastTransform = TransformData;
		}

		// Marshal transform update to game thread
		AsyncTask(ENamedThreads::GameThread, [this, TransformData]()
		{
			OnTransformUpdated.ExecuteIfBound(TransformData);
		});
	}
}

FVRPNTransformData FVRPNConnectionManager::GetLastTransform() const
{
	FScopeLock Lock(&TransformDataCS);
	return LastTransform;
}

