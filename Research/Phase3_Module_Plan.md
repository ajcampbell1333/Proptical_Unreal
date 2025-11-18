# Phase 3 Module Architecture Plan (Updated)

## Overview
This document outlines the module structure we will have by the end of Phase 3 (Demo Integration). Updated based on user feedback to prioritize core tracking accuracy validation over rigs/IK/solvers.

## Module Structure

### Core Plugin Module
**Location:** `Source/Proptical/`
- **Proptical.Build.cs** - Build configuration with networking dependencies
- **Proptical.h/cpp** - Main plugin module interface

### VRPN Client Module
**Location:** `Source/Proptical/Public/VRPN/` and `Source/Proptical/Private/VRPN/`

#### Classes:
1. **UVRPNClient** (UObject, UActorComponent)
   - Blueprint-exposed component for VRPN connection
   - **Primary focus:** UDP data reception for real-time tracking
   - TCP only for connection establishment (low priority for real-time data)
   - Properties: ServerAddress, ServerPort, RigidBodyName
   - Events: OnTransformUpdated, OnConnectionEstablished, OnConnectionLost
   - **Network warnings:** Tooltips and log messages to guide users on firewall configuration

2. **FVRPNConnectionManager** (Struct/Class)
   - Manages TCP/UDP socket lifecycle
   - **UDP-focused:** Primary path for real-time data
   - Handles TCP connection handshake (connection only, not data)
   - Manages UDP port allocation and exchange
   - Thread-safe socket operations
   - **Network guidance:** Provides warnings when UDP connection fails

3. **FVRPNMessageParser** (Struct/Class)
   - Parses VRPN protocol messages (minimal subset)
   - Extracts transform data (position, rotation)
   - Handles Tracker message type (primary focus)
   - Validates message integrity
   - **Implementation:** Custom minimal VRPN protocol parser (not full library)

4. **FVRPNTransformData** (Struct)
   - Contains position (FVector), rotation (FRotator/Quat), timestamp
   - Blueprint-exposed via USTRUCT

### Demo Integration Module (Simplified)
**Location:** `Source/Proptical/Public/Demo/` and `Source/Proptical/Private/Demo/`

#### Classes:
1. **AVRPNTrackedActor** (AActor)
   - Base demo actor that uses UVRPNClient
   - Applies received transforms to actor's root component
   - Blueprint-friendly for easy setup in demo map
   - Properties: RigidBodyName, bSmoothInterpolation, InterpolationSpeed

2. **AVRPNTransformNode** (AVRPNTrackedActor)
   - **Simplified demo actor** - replaces sword actor concept
   - Transform node with primitive visualizer
   - Supports basic primitives: Cube, Sphere, Cylinder
   - Used for debugging and validating tracking accuracy
   - **Purpose:** Validate core tracking accuracy before moving to rigs/IK/solvers

### Content
**Location:** `Content/`
- Demo map: `Maps/VRPNDemo.umap`
- Basic primitive meshes (cube, sphere, cylinder) for visualizers
- Material for tracked object visualization

## Module Dependencies

### Public Dependencies (Proptical.Build.cs)
- Core
- CoreUObject
- Engine
- Sockets
- Networking

### Private Dependencies (if needed)
- None currently planned

## Blueprint Integration

### Components
- `UVRPNClient` - Fully Blueprint-exposed
- All properties marked with `UPROPERTY(BlueprintReadWrite)`
- All events marked with `DECLARE_DYNAMIC_MULTICAST_DELEGATE`

### Actors
- `AVRPNTrackedActor` - Blueprint base class
- `AVRPNTransformNode` - Blueprint-ready demo actor with primitive visualizers

## Threading Model

- **Game Thread**: All Blueprint callbacks, actor updates
- **Background Thread**: Socket I/O, message parsing (marshaled back to game thread)
- **Async Tasks**: Use `FAsyncTask` or `AsyncTask` for socket operations

## Network Architecture

```
[Proptical Server] 
    ↓ (UDP port 3883 - initial contact)
[UVRPNClient]
    ↓ (TCP - handshake only, low priority)
[FVRPNConnectionManager]
    ↓ (UDP - PRIMARY data stream, high priority)
[FVRPNMessageParser]
    ↓ (parsed data)
[FVRPNTransformData]
    ↓ (game thread)
[AVRPNTransformNode]
    ↓ (applied to transform + primitive visualizer)
[Scene]
```

## File Structure Summary

```
Plugins/Proptical/
├── Source/
│   └── Proptical/
│       ├── Proptical.Build.cs
│       ├── Proptical.h
│       ├── Proptical.cpp
│       ├── Public/
│       │   ├── VRPN/
│       │   │   ├── VRPNClient.h
│       │   │   ├── VRPNTransformData.h
│       │   └── Demo/
│       │       ├── VRPNTrackedActor.h
│       │       └── VRPNTransformNode.h
│       └── Private/
│           ├── VRPN/
│           │   ├── VRPNClient.cpp
│           │   ├── VRPNConnectionManager.h
│           │   ├── VRPNConnectionManager.cpp
│           │   ├── VRPNMessageParser.h
│           │   └── VRPNMessageParser.cpp
│           └── Demo/
│               ├── VRPNTrackedActor.cpp
│               └── VRPNTransformNode.cpp
├── Content/
│   └── Maps/
│       └── VRPNDemo.umap
└── Research/
    ├── VRPN_Protocol.md
    ├── VRPN_Library_Research.md
    └── Phase3_Module_Plan.md (this file)
```

## Implementation Order (Phases 2-3)

### Phase 2: VRPN Client Core
1. FVRPNTransformData (data structure)
2. FVRPNMessageParser (minimal VRPN protocol parsing - UDP focus)
3. FVRPNConnectionManager (UDP socket management, TCP handshake low priority)
4. UVRPNClient (Blueprint integration, network warnings)

### Phase 3: Demo Integration (Simplified)
5. AVRPNTrackedActor (base demo actor)
6. AVRPNTransformNode (transform node with primitive visualizers - Cube, Sphere, Cylinder)
7. Demo map creation with multiple transform nodes
8. Testing and validation of core tracking accuracy

## Priority Adjustments

### High Priority (Alpha - Phase 2-3)
- ✅ UDP real-time data stream
- ✅ Minimal VRPN protocol parser (Tracker messages)
- ✅ Transform nodes with primitive visualizers
- ✅ Network configuration warnings and tooltips
- ✅ Core tracking accuracy validation

### Low Priority (v1.0)
- ⏸️ TCP mode for real-time data (fallback for pros on capture stages)
- ⏸️ NAT traversal automation
- ⏸️ Rigs, solvers, IK (validate core tracking first)

## User Archetype Assumptions

**Alpha Users:** Tech-savvy VFX devs/artists working in virtual production
- Have networked PCs over UDP before
- Can configure firewalls manually with guidance
- Need tooltips, warnings, and log comments to assist
- Understand firewall configuration or can learn with our help

## Network Configuration Guidance

### Warnings and Tooltips
- **UDP Connection Failed:** Show warning with guidance on firewall configuration
- **Tooltips:** Explain UDP port requirements, firewall rules
- **Log Comments:** Detailed messages about network configuration needs
- **User Guidance:** 
  - Manual firewall port opening instructions
  - Option to disable firewalls on networked devices (with warning)
  - Network troubleshooting steps

### Implementation
- Pop-up warnings when UDP connection fails
- In-editor tooltips explaining network requirements
- Detailed log messages for troubleshooting
- Help users manually configure firewalls (NAT traversal automation later)

## Open Questions / Roadblocks

1. **VRPN Library Integration**: ✅ **RESOLVED** - Implement minimal VRPN protocol subset from scratch
   - Focus on Tracker messages only
   - Reference VRPN protocol specification
   - Unreal-native implementation

2. **Threading Safety**: Socket operations on background thread, marshaling to game thread
   - Solution: Use `AsyncTask(ENamedThreads::GameThread, ...)` for callbacks

3. **NAT Traversal**: ⏸️ **DEFERRED TO v1.0**
   - Low priority - users can configure manually
   - TCP-only mode also deferred to v1.0

4. **Blueprint Performance**: Ensure Blueprint callbacks don't cause frame drops
   - Solution: Batch updates, use tick groups appropriately

5. **Core Tracking Validation**: ✅ **PRIORITY**
   - Focus on validating OpenCV & Docker integration
   - Answer: "Do we get accurate 3D positions from OpenCV or model output?"
   - Only then move to rigs, solvers, IK

## Notes

- All public classes should be Blueprint-exposed for artist-friendly usage
- Follow Unreal Engine coding standards (AGENT Unreal Rules.md)
- Keep controllers thin, extract services (FVRPNConnectionManager, FVRPNMessageParser)
- Use interfaces where appropriate for testability
- Document all public APIs with XML comments
- **Emphasis:** Validate core tracking accuracy before advanced features
