#pragma once

#include "CoreMinimal.h"
#include "VRPNTransformData.generated.h"

/**
 * Transform data received from VRPN server
 * Contains position, rotation, and timestamp for a tracked rigid body
 */
USTRUCT(BlueprintType)
struct PROPTICAL_API FVRPNTransformData
{
	GENERATED_BODY()

	/** Position in world space (X, Y, Z) */
	UPROPERTY(BlueprintReadWrite, Category = "VRPN")
	FVector Position;

	/** Rotation as quaternion (W, X, Y, Z) */
	UPROPERTY(BlueprintReadWrite, Category = "VRPN")
	FQuat Rotation;

	/** Timestamp when this transform was received (in seconds since connection start) */
	UPROPERTY(BlueprintReadWrite, Category = "VRPN")
	float Timestamp;

	/** Default constructor */
	FVRPNTransformData()
		: Position(FVector::ZeroVector)
		, Rotation(FQuat::Identity)
		, Timestamp(0.0f)
	{
	}

	/** Constructor with values */
	FVRPNTransformData(const FVector& InPosition, const FQuat& InRotation, float InTimestamp)
		: Position(InPosition)
		, Rotation(InRotation)
		, Timestamp(InTimestamp)
	{
	}

	/** Check if transform data is valid */
	bool IsValid() const
	{
		return !Position.ContainsNaN() && Rotation.IsNormalized();
	}
};

