#pragma once

#include "CoreMinimal.h"
#include "VRPN/VRPNTransformData.h"

/**
 * Parses VRPN protocol messages
 * Implements minimal VRPN protocol subset focused on Tracker messages
 */
class FVRPNMessageParser
{
public:
	/**
	 * Parse a VRPN Tracker message from raw UDP data
	 * @param Data Raw UDP packet data
	 * @param DataSize Size of the data buffer
	 * @param OutTransform Output transform data if parsing succeeds
	 * @param OutRigidBodyName Output rigid body name if present in message
	 * @return true if message was successfully parsed, false otherwise
	 */
	static bool ParseTrackerMessage(const uint8* Data, int32 DataSize, FVRPNTransformData& OutTransform, FString& OutRigidBodyName);

	/**
	 * Validate VRPN message header
	 * @param Data Raw message data
	 * @param DataSize Size of the data buffer
	 * @return true if header is valid, false otherwise
	 */
	static bool ValidateMessageHeader(const uint8* Data, int32 DataSize);

private:
	/** VRPN message type constants */
	static constexpr int32 VRPN_MESSAGE_TYPE_TRACKER = 0;
	
	/** Minimum message size for a valid VRPN message */
	static constexpr int32 VRPN_MIN_MESSAGE_SIZE = 16;
};

