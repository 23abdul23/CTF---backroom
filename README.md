# CGV - Collaborative Game Visualization

A multiplayer networked physics simulation where up to 4 players collaboratively interact with physics-based balls in real-time.

## Features

- **Multiplayer Networking**: Peer-to-peer client communication via central server
- **Real-time Physics**: Ball simulation with velocity, acceleration, and drag
- **3D Graphics**: OpenGL/GLUT rendering
- **Interactive**: Mouse-based ball manipulation (drag and throw)
- **Synchronized State**: Server coordinates player connections

## Build

### Requirements
- GCC or Clang
- OpenGL development libraries
- GLUT development libraries
- POSIX-compliant system (Linux/macOS)

### Ubuntu/Debian
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

### Build Commands
```bash
make          # Build both server and client
make clean    # Remove build artifacts
```

## Running

### Local 4-Player Test
```bash
make run-localhost
```
Starts server and 4 clients connecting to localhost.

### Manual Server
```bash
make run-server
```
Starts server on port 5000.

### Manual Client
```bash
./build/client_app <server_ip>
```
Connect a client to server at `<server_ip>`.

## Project Structure

```
├── server/           # Matchmaking server
├── client/
│   ├── core/         # Game state & logic
│   ├── networking/   # Client network layer
│   ├── physics/      # Physics calculations
│   ├── simulation/   # Ball management
│   ├── render/       # OpenGL rendering
│   ├── input/        # Input handling
│   └── util/         # Utilities
├── common/           # Shared protocol
└── build/            # Compiled binaries
```

## Game Controls

- **Left Mouse**: Click and drag balls to move them
- **Ctrl+C**: Exit client

## Architecture

- **Server**: Accepts 4 clients, provides peer addresses for mesh connectivity
- **Clients**: Connect to server, then establish peer connections with other clients
- **Communication**: TCP sockets for reliability
- **Simulation**: Each ball owned by one player; transfers between players as balls move between regions

## Performance

- Compiled with `-O2` optimization
- Physics updates at ~60 FPS (configurable via `DT` in config.h)
- Multithreaded networking (receive thread per client)
