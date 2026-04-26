# CGV - Multiplayer Raycasting FPS Prototype

CGV is a C/OpenGL (GLUT) networked first-person prototype with:
- lobby + ready gating,
- folder-driven character selection,
- peer-to-peer runtime position sync,
- and capture-the-flag (CTF) style flag stealing.

It uses classic 2.5D raycasting for walls and billboard sprites for players.

---

## Current project status (April 2026)

### Player/session model
- Maximum players supported: **6**.
- Matchmaking starts when at least **2** players connect to the server.
- Game starts only after **all connected players** mark ready in the lobby.

### Lobby system
- Dedicated pre-game lobby UI before gameplay starts.
- Per-player ready state synchronized via server.
- Character selection synchronized via server.
- Local player texture preview + bottom thumbnail roster (“fighting-game style” selector).

### Character system
- Character textures are discovered at runtime by scanning `textures/` for `.ppm` files.
- No hardcoded 4-sprite list is required anymore.
- Files are sorted and exposed as a selectable catalog.

### Map system
- Map is currently authored as an **ASCII grid** and parsed into wall IDs.
- Wall symbols:
	- `#` → brick wall
	- `=` → stone wall
	- `+` → wood wall
	- `.` (or any other symbol) → empty/walkable

### Rendering/performance
- Wall rendering: per-column raycasting.
- Player sprite rendering: textured quads (GPU texturing path).
- Occlusion: per-screen-column wall depth is used so hidden sprite parts are clipped correctly.

---

## Core gameplay

- First-person movement in a 40x32 grid map.
- Real-time remote player updates over peer sockets.
- CTF logic:
	- one flag holder at a time,
	- nearby players can steal within threshold distance,
	- cooldown to prevent instant repeated steals,
	- hold-time and steal counters tracked per player.

---

## Build

### Requirements
- GCC or Clang (C11)
- OpenGL + GLU + GLUT dev libraries
- POSIX-like environment (Linux/macOS)

### Ubuntu / Debian
```bash
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

### Build commands
```bash
make        # Build server and client
make clean  # Remove build artifacts
make debug  # Debug build (-g + verbose logging)
```

---

## Run

### Fast local run (default make target)
```bash
make run-localhost
```
This runs `run_localhost_4players.sh` (legacy filename), which currently spawns **6 clients**.

### Explicit 2-player local run
```bash
./run_localhost_2players.sh
```

### Explicit 6-player local run
```bash
./run_localhost_4players.sh
```

### Server only
```bash
make run-server
```

### Client only
```bash
./build/client_app <server_ip>
```

---

## Controls

### Lobby controls (before game start)
- `Q`: previous character
- `E`: next character
- `R`: toggle ready

### In-game controls
- `W`, `A`, `S`, `D`: move/strafe
- `←`, `→`: turn
- `ESC`: quit

---

## Networking architecture (current)

1. Client opens local listener socket.
2. Client sends join request to server with local listen port.
3. Server assigns player IDs and returns all player endpoints (`JoinResponse`).
4. Clients establish peer mesh connections.
5. Lobby phase remains server-mediated (ready/character/start messages).
6. Once started, gameplay position updates are exchanged peer-to-peer.

Shared packet definitions are in `common/protocol.h`.

---

## Rendering architecture (current)

1. **Raycast pass**
	 - Cast one ray per screen column.
	 - Fill wall slice.
	 - Store wall depth per column.
2. **Sprite pass**
	 - For each remote player, compute camera-relative screen quad.
	 - Texture the quad.
	 - Clip per-column against ray depth buffer for proper occlusion.
3. **HUD/Lobby UI pass**
	 - Draw overlays, labels, and lobby character selection UI.

---

## Character texture pipeline

- Source format: **PPM P6**.
- Discovery: runtime scan of `textures/*.ppm`.
- Loading: preloads base textures into memory.
- Assignment: selected texture is cloned and applied per player sprite.
- Fallback: placeholder texture is generated if loading fails.

---

## Project layout

```text
server/                  Matchmaking + lobby orchestration
client/
	core/                  Game state, movement, CTF logic
	networking/            Server + peer sockets, recv thread
	render/                Raycaster, sprites, textures, UI
	simulation/            Map representation and lookup
	input/                 Keyboard handling
	util/                  Helpers/math
common/                  Shared protocol/logging
textures/                Character sprite assets (.ppm)
build/                   Generated binaries
```

---

## Troubleshooting

### Game never starts from lobby
- Ensure every connected player pressed `R`.
- Check server log for ready-state updates.

### Missing or broken character textures
- Confirm file is valid **P6 PPM** and placed in `textures/`.
- Rebuild/restart to refresh catalog on startup.

### Cannot connect to server
- Confirm port `5000` is free.
- Verify firewall/local network permissions.

### Rendering artifacts or low FPS
- Use release build (`make`) rather than debug.
- Reduce number/size of custom textures if necessary.
- Check GPU/OpenGL driver availability.

---

## Notes

- Some script/target names are legacy (`run_localhost_4players.sh`) but behavior reflects current 6-player support.
- If you are extending this project, prefer updating docs and script names together to keep UX consistent.
