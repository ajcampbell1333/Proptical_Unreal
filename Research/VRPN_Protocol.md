# VRPN Protocol Research

## Protocol Overview

VRPN (Virtual Reality Peripheral Network) uses a hybrid TCP/UDP approach for communication between clients and servers.

## Connection Process

### Initial Handshake (TCP)
1. **Client Initialization**: Client opens a TCP socket on an arbitrary port
2. **Client Announcement**: Client sends port number and machine name via UDP datagram to server's well-known port (default: 3883)
3. **Server Response**: Server establishes TCP connection to client's specified port
4. **Version Check**: Both client and server exchange supported VRPN versions over TCP to ensure compatibility
5. **UDP Setup**: If versions are compatible, both devices set up new UDP ports and exchange port numbers over the established TCP connection

### Data Transmission (UDP)
- After setup, tracking data streams occur over UDP for low latency
- UDP is preferred for real-time tracking updates
- This is the primary data path for tracking information

### Alternative: TCP-Only Mode
- VRPN supports a "reliable" mode using only TCP
- Useful when UDP is blocked or NAT traversal is problematic
- Higher latency but more reliable
- Standard client-server interaction without UDP involvement

## Implementation Implications

### Required Components
1. **UDP Socket** - **PRIMARY:** For receiving real-time tracking data streams (HIGH PRIORITY)
2. **TCP Socket** - **SECONDARY:** For initial connection handshake only (low priority for real-time data)
3. **Connection Manager** - Handles TCP handshake, then switches to UDP for data
4. **Protocol Parser** - Decodes VRPN message format (minimal subset - Tracker messages)

### Network Considerations
- Default VRPN port: 3883 (UDP for initial contact)
- **UDP is primary focus** - real-time 3D data must use UDP
- **TCP for real-time data:** Low priority, move to v1.0
- **NAT traversal:** Low priority, move to v1.0
- **User guidance:** Tooltips, warnings, log comments to help users configure firewalls manually
- **User archetype:** Tech-savvy VFX devs/artists who can configure firewalls with guidance

## References
- VRPN Wikipedia: https://en.wikipedia.org/wiki/VRPN
- VRPN supports both TCP and UDP, with UDP preferred for low-latency tracking data

