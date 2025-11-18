#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRPNTransformData.h"
#include "VRPNClient.generated.h"

// Forward declaration
class FVRPNConnectionManager;

/**
 * VRPN Client
 * Blueprint-exposed component for connecting to VRPN server and receiving tracking data
 * 
 * Network Configuration:
 * - UDP is primary for real-time tracking data
 * - TCP handshake is low priority (may be deferred)
 * - If UDP connection fails, check firewall settings and ensure UDP port is open
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROPTICAL_API UVRPNClient : public UActorComponent
{
	GENERATED_BODY()

public:
	UVRPNClient(const FObjectInitializer& ObjectInitializer);
	virtual ~UVRPNClient();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Connect to VRPN server
	 * Server Address: IP or hostname (e.g., 127.0.0.1)
	 * Server Port: UDP port (default 3883)
	 * If connection fails, check firewall settings and ensure UDP port is open.
	 * @param InServerAddress IP address or hostname of VRPN server (e.g., "127.0.0.1" or "192.168.1.100")
	 * @param InServerPort UDP port for VRPN server (default: 3883)
	 * @param InRigidBodyName Name of the rigid body to track (optional, can be empty to track all)
	 */
	UFUNCTION(BlueprintCallable, Category = "VRPN", CallInEditor)
	void ConnectToServer(const FString& InServerAddress = TEXT("127.0.0.1"), int32 InServerPort = 3883, const FString& InRigidBodyName = TEXT(""));

	/**
	 * Disconnect from VRPN server
	 */
	UFUNCTION(BlueprintCallable, Category = "VRPN", CallInEditor)
	void DisconnectFromServer();

	/**
	 * Check if currently connected to server
	 */
	UFUNCTION(BlueprintPure, Category = "VRPN")
	bool IsConnected() const;

	/**
	 * Get last received transform data
	 */
	UFUNCTION(BlueprintPure, Category = "VRPN")
	FVRPNTransformData GetLastTransform() const;

	/** Server address (IP or hostname). Example: 127.0.0.1 or 192.168.1.100 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN", meta = (ToolTip = "VRPN server IP address or hostname (e.g., 127.0.0.1 or 192.168.1.100)"))
	FString ServerAddress;

	/** Server UDP port (default VRPN port is 3883). Ensure this port is open in your firewall. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN", meta = (ToolTip = "VRPN server UDP port. Default is 3883. Ensure this port is open in your firewall."))
	int32 ServerPort;

	/** Name of rigid body to track (empty = track all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN", meta = (ToolTip = "Name of the rigid body to track. Leave empty to track all rigid bodies."))
	FString RigidBodyName;

	/** Auto-connect on BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN")
	bool bAutoConnect;

	/** Enable smooth interpolation of transforms */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN")
	bool bSmoothInterpolation;

	/** Interpolation speed (0-1, higher = faster) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPN", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float InterpolationSpeed;

	/** Delegate type for transform updates */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransformUpdatedDelegate, const FVRPNTransformData&, Transform);

	/** Event fired when transform is updated */
	UPROPERTY(BlueprintAssignable, Category = "VRPN")
	FOnTransformUpdatedDelegate OnTransformUpdated;

	/** Delegate type for connection established */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionEstablishedDelegate);

	/** Event fired when connection is established */
	UPROPERTY(BlueprintAssignable, Category = "VRPN")
	FOnConnectionEstablishedDelegate OnConnectionEstablished;

	/** Delegate type for connection lost */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionLostDelegate, const FString&, ErrorMessage);

	/** Event fired when connection is lost */
	UPROPERTY(BlueprintAssignable, Category = "VRPN")
	FOnConnectionLostDelegate OnConnectionLost;

private:
	/** Connection manager instance (forward declared, full definition in .cpp) */
	TSharedPtr<FVRPNConnectionManager> ConnectionManager;

	/** Current interpolated transform */
	FVRPNTransformData CurrentTransform;

	/** Handle connection established callback */
	void HandleConnectionEstablished();

	/** Handle connection lost callback */
	void HandleConnectionLost(const FString& ErrorMessage);

	/** Handle transform updated callback */
	void HandleTransformUpdated(const FVRPNTransformData& Transform);
};

