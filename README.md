# CGV - Capture-the-Flag Raycasting Game

This project is a C/OpenGL multiplayer raycasting FPS prototype. Players move through a maze, see each other as sprites, and compete in a simple capture-the-flag mode where the flag can be stolen by getting close to the current holder.

## What it does

- Starts a matchmaking server and connects up to 4 players.
- Renders a first-person raycasted view of a maze.
- Draws other players as billboard sprites.
- Tracks a flag holder, steal cooldown, and hold-time ranking.
- Shows a minimap and in-game HUD overlays.
- Supports custom PPM textures for player sprites.

## Build

### Requirements
- GCC or Clang
- OpenGL / GLUT development libraries
- POSIX-compatible system (Linux/macOS)

### Ubuntu / Debian
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

### Build commands
```bash
make       # Build server and client
make clean # Remove build artifacts
make debug # Debug build with symbols and verbose logging
```

## Run

### Local test
```bash
make run-localhost
```
Starts the server and local clients on `localhost`.

### Server only
```bash
make run-server
```

### Client only
```bash
./build/client_app <server_ip>
```

## Controls

- `WASD`: Move
- Arrow keys: Turn
- `ESC`: Exit

## Current game features

- Maze-based raycasting renderer
- Multi-player network synchronization
- Capture-the-flag gameplay
- Flag holder highlight on sprite and minimap
- Ranking by total time holding the flag
- Custom player textures from `textures/`

## Project structure

```
├── server/           # Matchmaking server
├── client/
│   ├── core/         # Game state and per-frame update
│   ├── networking/   # Socket connection and receive thread
│   ├── render/       # Raycaster, sprites, textures, HUD
│   ├── simulation/   # Level/map generation
│   ├── input/        # Keyboard input
│   └── util/         # Helpers
├── common/           # Shared protocol and logging
├── textures/         # Player sprite textures
└── build/            # Output binaries
```

## Notes on custom textures

- Player textures are loaded from PPM files in `textures/`.
- You can replace the files used in [client/core/game.c](client/core/game.c) with your own PPM sprites.
- The texture loader currently expects PPM input, not PNG/JPG.

## How the architecture works

- The server assigns player IDs and shares peer connection info.
- Clients connect to the server, then establish direct peer sockets.
- The main loop updates movement and broadcast state.
- A receive thread applies remote player updates.
- The renderer raycasts against the map grid and draws walls/sprites/HUD.

## Troubleshooting

- If a texture is not showing, confirm the file exists in `textures/` and is valid PPM.
- If the game does not start, make sure the build finished successfully and the required graphics libraries are installed.
- If port `5000` is busy, stop the existing server process and try again.
