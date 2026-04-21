# Development Configuration

## Physics
- Adjust `DT` in [client/config.h](client/config.h#L7) for simulation speed
- Modify `MAX_SPEED` for velocity limits
- Change `RESTITUTION` for ball bounciness (0.0 = no bounce)

## Graphics
- Window resolution: `WIDTH` and `HEIGHT` in [client/config.h](client/config.h#L4-L5)
- Ball appearance: Colors and radius in config.h

## Networking
- Server port: `SERVER_PORT` in [server/server.h](server/server.h#L5)
- Max players: `MAX_PLAYERS` in [server/server.h](server/server.h#L6)
- Network timeout: `NETWORK_TIMEOUT_SEC` in [common/protocol.h](common/protocol.h#L12)

## Debugging
Build with debug symbols and verbose logging:
```bash
make debug
```

Then run with full logging output.

## Code Organization

**Server**: Simple matchmaking - waits for N clients, provides peer addresses
**Client**: 
- Game loop runs in main thread (OpenGL)
- Receive thread handles network packets
- Peer manager handles P2P connections
- Physics simulation updates ball positions

**Physics**: Simple Euler integration for balls
**Rendering**: OpenGL immediate mode (consider modernizing to shaders)
