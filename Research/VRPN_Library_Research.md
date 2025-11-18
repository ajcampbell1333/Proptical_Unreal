# VRPN Library Research

## Research Summary

**Decision:** Implement minimal VRPN protocol subset from scratch

## Findings

### VRPN Library Options

#### Option 1: Use Official VRPN Library
- **Status:** Research indicates VRPN library exists but integration complexity unknown
- **Location:** Need to verify official repository (vrpn.sourceforge.net or GitHub)
- **Assessment:** Likely requires significant integration work, potential Unreal compatibility issues

#### Option 2: Implement Minimal VRPN Protocol Subset (SELECTED)
- **Status:** Recommended approach
- **Pros:** 
  - Full control over implementation
  - Unreal-native, optimized for our use case
  - Only implement what we need (Tracker messages)
  - Easier to maintain and debug
- **Cons:** 
  - More initial development time
  - Need to implement protocol from specification

## Decision Rationale

**We will implement a minimal VRPN protocol subset from scratch.**

**Reasons:**
1. We only need Tracker message support (position/rotation data)
2. Full VRPN library may be overkill and introduce compatibility issues
3. Unreal-native implementation gives us better control and performance
4. We can reference VRPN protocol specification for message formats
5. Easier to optimize for our specific use case (real-time tracking)

## Protocol Implementation Focus

### Phase 2: Minimal VRPN Client

1. **Connection Handshake (TCP) - Low Priority**
   - Client opens TCP socket
   - Send UDP port and machine name to server (UDP port 3883)
   - Server connects via TCP
   - Exchange version information
   - Exchange UDP port numbers
   - **Note:** TCP only for connection establishment, not real-time data

2. **Data Reception (UDP) - HIGH PRIORITY**
   - **Primary focus:** UDP data stream for real-time tracking
   - Parse Tracker messages
   - Extract position (x, y, z) and rotation (quaternion)
   - Validate message integrity
   - **TCP for real-time data is low priority - move to v1.0**

3. **Error Handling & User Guidance**
   - Connection failures
   - UDP packet loss (expected, acceptable for real-time)
   - Version mismatches
   - **Network configuration warnings:** Tooltips, warnings, log comments to guide users
   - **User archetype:** Tech-savvy VFX devs/artists who can configure firewalls manually
   - **Guidance:** Help users manually punch through firewalls or disable firewalls on networked devices
   - **NAT traversal:** Low priority, move to v1.0 (same as TCP real-time mode)

## TCP Mode (v1.0)

- Keep TCP-only mode in roadmap for v1.0
- Pros on capture stages may be accustomed to TCP fallback option
- Not priority for alpha release

## Next Steps

1. Document VRPN Tracker message format (binary protocol)
2. Implement minimal protocol parser (UDP focus)
3. Implement TCP handshake (connection only)
4. Add network configuration warnings and tooltips
5. Test with Proptical Server

## References

- VRPN Protocol: TCP handshake + UDP data stream
- Default VRPN port: 3883 (UDP for initial contact)
- Tracker messages contain: position (3 floats), rotation (quaternion), timestamp
- **Emphasis:** UDP for real-time 3D data is top priority
