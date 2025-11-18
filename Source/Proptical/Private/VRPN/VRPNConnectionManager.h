#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Common/TcpSocketBuilder.h"
#include "VRPN/VRPNTransformData.h"

class FSocket;
class FInternetAddr;

/**
 * Manages VRPN connection lifecycle
 * Handles TCP handshake and UDP data reception
 * Thread-safe socket operations with game thread marshaling
 */
class FVRPNConnectionManager : public FRunnable
{
public:
	FVRPNConnectionManager();
	~FVRPNConnectionManager();

	/**
	 * Initialize connection to VRPN server
	 * @param ServerAddress IP address or hostname of VRPN server
	 * @param ServerPort UDP port for initial contact (default: 3883)
	 * @return true if initialization started successfully
	 */
	bool InitializeConnection(const FString& ServerAddress, int32 ServerPort = 3883);

	/**
	 * Start receiving data on background thread
	 * @return true if thread started successfully
	 */
	bool StartReceiving();

	/**
	 * Stop receiving data and close connection
	 */
	void StopReceiving();

	/**
	 * Check if currently connected
	 */
	bool IsConnected() const { return bIsConnected; }

	/**
	 * Get last received transform data
	 */
	FVRPNTransformData GetLastTransform() const;

	/**
	 * Delegate for transform updates (called on game thread)
	 */
	DECLARE_DELEGATE_OneParam(FOnTransformUpdated, const FVRPNTransformData&);
	FOnTransformUpdated OnTransformUpdated;

	/**
	 * Delegate for connection events (called on game thread)
	 */
	DECLARE_DELEGATE(FOnConnectionEstablished);
	DECLARE_DELEGATE_OneParam(FOnConnectionLost, const FString&);
	FOnConnectionEstablished OnConnectionEstablished;
	FOnConnectionLost OnConnectionLost;

	// FRunnable interface
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	/** TCP socket for handshake (low priority) */
	TSharedPtr<FSocket> TCPSocket;

	/** UDP socket for data reception (primary) */
	TSharedPtr<FSocket> UDPSocket;

	/** Server address */
	TSharedPtr<FInternetAddr> ServerAddr;

	/** Background thread for receiving data */
	FRunnableThread* ReceiveThread;

	/** Thread control flags */
	FThreadSafeBool bShouldStop;
	FThreadSafeBool bIsConnected;

	/** Last received transform data */
	mutable FCriticalSection TransformDataCS;
	FVRPNTransformData LastTransform;

	/** Server address and port */
	FString ServerAddress;
	int32 ServerPort;

	/**
	 * Perform TCP handshake (low priority - may be deferred)
	 * @return true if handshake succeeded
	 */
	bool PerformTCPHandshake();

	/**
	 * Setup UDP socket for data reception
	 * @return true if UDP socket setup succeeded
	 */
	bool SetupUDPSocket();

	/**
	 * Process received UDP packet
	 * @param Data Raw packet data
	 * @param DataSize Size of packet
	 */
	void ProcessUDPPacket(const uint8* Data, int32 DataSize);
};

