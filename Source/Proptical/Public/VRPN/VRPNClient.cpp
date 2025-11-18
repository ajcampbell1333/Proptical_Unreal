#include "VRPNClient.h"
#include "VRPN/VRPNConnectionManager.h"
#include "VRPN/VRPNTransformData.h"

UVRPNClient::UVRPNClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ServerAddress(TEXT("127.0.0.1"))
	, ServerPort(3883)
	, RigidBodyName(TEXT(""))
	, bAutoConnect(false)
	, bSmoothInterpolation(true)
	, InterpolationSpeed(0.1f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

// Destructor must be defined here so TUniquePtr can see full definition of FVRPNConnectionManager
UVRPNClient::~UVRPNClient()
{
	DisconnectFromServer();
	// TUniquePtr will automatically clean up ConnectionManager
}

void UVRPNClient::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoConnect)
	{
		ConnectToServer(ServerAddress, ServerPort, RigidBodyName);
	}
}

void UVRPNClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DisconnectFromServer();
	Super::EndPlay(EndPlayReason);
}

void UVRPNClient::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ConnectionManager.IsValid() && ConnectionManager->IsConnected())
	{
		FVRPNTransformData LastTransform = ConnectionManager->GetLastTransform();
		
		if (bSmoothInterpolation && LastTransform.IsValid())
		{
			// Smooth interpolation
			CurrentTransform.Position = FMath::Lerp(CurrentTransform.Position, LastTransform.Position, InterpolationSpeed);
			CurrentTransform.Rotation = FQuat::Slerp(CurrentTransform.Rotation, LastTransform.Rotation, InterpolationSpeed);
			CurrentTransform.Timestamp = LastTransform.Timestamp;
		}
		else if (LastTransform.IsValid())
		{
			CurrentTransform = LastTransform;
		}
	}
}

void UVRPNClient::ConnectToServer(const FString& InServerAddress, int32 InServerPort, const FString& InRigidBodyName)
{
	// Update properties
	ServerAddress = InServerAddress;
	ServerPort = InServerPort;
	RigidBodyName = InRigidBodyName;

	// Disconnect existing connection if any
	DisconnectFromServer();

	// Create connection manager
	ConnectionManager = MakeShareable(new FVRPNConnectionManager());

	// Setup callbacks
	ConnectionManager->OnConnectionEstablished.BindUObject(this, &UVRPNClient::HandleConnectionEstablished);
	ConnectionManager->OnConnectionLost.BindUObject(this, &UVRPNClient::HandleConnectionLost);
	ConnectionManager->OnTransformUpdated.BindUObject(this, &UVRPNClient::HandleTransformUpdated);

	// Initialize connection
	if (!ConnectionManager->InitializeConnection(ServerAddress, ServerPort))
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to initialize connection to %s:%d"), *ServerAddress, ServerPort);
		UE_LOG(LogTemp, Warning, TEXT("VRPN: Check firewall settings and ensure UDP port %d is open"), ServerPort);
		return;
	}

	// Start receiving data
	if (!ConnectionManager->StartReceiving())
	{
		UE_LOG(LogTemp, Error, TEXT("VRPN: Failed to start receiving data"));
		ConnectionManager.Reset();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("VRPN: Connecting to server %s:%d (Rigid Body: %s)"), *ServerAddress, ServerPort, *RigidBodyName);
}

void UVRPNClient::DisconnectFromServer()
{
	if (ConnectionManager.IsValid())
	{
		ConnectionManager->StopReceiving();
		ConnectionManager.Reset();
		UE_LOG(LogTemp, Log, TEXT("VRPN: Disconnected from server"));
	}
}

bool UVRPNClient::IsConnected() const
{
	return ConnectionManager.IsValid() && ConnectionManager->IsConnected();
}

FVRPNTransformData UVRPNClient::GetLastTransform() const
{
	if (ConnectionManager.IsValid())
	{
		return ConnectionManager->GetLastTransform();
	}
	return FVRPNTransformData();
}

void UVRPNClient::HandleConnectionEstablished()
{
	UE_LOG(LogTemp, Log, TEXT("VRPN: Connection established"));
	OnConnectionEstablished.Broadcast();
}

void UVRPNClient::HandleConnectionLost(const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("VRPN: Connection lost - %s"), *ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("VRPN: Troubleshooting: Check firewall settings, ensure UDP port %d is open on both client and server"), ServerPort);
	OnConnectionLost.Broadcast(ErrorMessage);
}

void UVRPNClient::HandleTransformUpdated(const FVRPNTransformData& Transform)
{
	if (Transform.IsValid())
	{
		OnTransformUpdated.Broadcast(Transform);
	}
}

