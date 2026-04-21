# CGV Quick Reference

## Building
```bash
make              # Standard build (info-level logging)
make debug        # Debug build (verbose logging + debug symbols)
make clean        # Clean build artifacts
```

## Running

### Local Test (4 Players)
```bash
make run-localhost
```
Starts server + 4 clients on localhost. Watch the physics simulation in real-time!

### Server Only
```bash
make run-server
```
Waits for 4 clients to connect to port 5000.

### Client Only  
```bash
./build/client_app 127.0.0.1
```
Connect to server at given IP. Can spawn multiple instances.

## Game Controls
- **Click & drag** balls to throw them
- **Ctrl+C** to exit

## Logging Output
When running with logging, you'll see timestamped messages like:
```
[HH:MM:SS] INFO : Server listening on port 5000 (max 4 players)
[HH:MM:SS] INFO : Client connected from 127.0.0.1
[HH:MM:SS] INFO : Assigned Player 0
```

## Files You Might Want to Edit

| File | What It Does |
|------|-------------|
| [client/config.h](client/config.h) | Physics, graphics, gameplay parameters |
| [server/server.h](server/server.h) | Server port and max players |
| [common/protocol.h](common/protocol.h) | Network constants |
| [common/logging.h](common/logging.h) | Log levels and formatting |

## Troubleshooting

**"Permission denied" when running script:**
```bash
chmod +x run_localhost_4players.sh
```

**"Address already in use" error:**
Server port still in use. Wait a few seconds or kill the process:
```bash
lsof -i :5000  # Find process using port 5000
kill -9 <PID>
```

**Missing OpenGL libraries:**
```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

## Architecture at a Glance

```
┌─────────────────────────────────────────┐
│           Server (Matchmaking)          │
│  Accepts 4 clients, provides peer IPs   │
└─────────────────────────────────────────┘
         ↑      ↑      ↑      ↑
         │      │      │      │
    Client 1  Client 2 Client 3 Client 4
    (mesh connected peer-to-peer)
```

Each client runs the full game simulation and physics independently.
