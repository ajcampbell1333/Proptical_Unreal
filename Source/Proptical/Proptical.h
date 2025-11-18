#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Proptical Plugin Module
 * 
 * VRPN-based motion capture integration for Unreal Engine.
 * Provides drop-in Vicon/OptiTrack replacement using ESP32-CAMs, Wi-Fi, UWB anchors,
 * and state-of-the-art 3D pose models.
 */
class FPropticalModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

