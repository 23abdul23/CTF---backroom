# CGV Project Overview and Computer Graphics Guide (Current State)

This document explains how CGV is implemented *today* and how its core computer graphics and networking systems work.

---

## 1) Project summary

CGV is a C11 multiplayer raycasting FPS prototype built on OpenGL/GLUT.

It includes:
- server-based matchmaking and lobby synchronization,
- peer-to-peer gameplay updates,
- first-person wall raycasting,
- billboarded player sprites,
- runtime character selection from `textures/*.ppm`,
- and simple capture-the-flag (CTF) mechanics.

Current multiplayer scale:
- max players: **6**,
- minimum players required to enter a match: **2**,
- actual start condition: **all connected players ready**.

---

## 2) High-level runtime flow

1. **Server starts** and waits for client joins.
2. Each client:
	 - creates local listen socket,
	 - joins server with its listen port,
	 - receives assigned ID + peer endpoints.
3. Clients establish **peer mesh sockets**.
4. Lobby runs in server-mediated mode:
	 - ready toggles,
	 - character selection sync,
	 - start broadcast.
5. After start:
	 - client main thread runs input/simulation/render loop,
	 - receive thread processes peer updates.

---

## 3) Core modules and responsibilities

### `server/`
- Accepts join requests.
- Assigns player IDs.
- Sends `JoinResponse`.
- Maintains lobby state (`connected[]`, `ready[]`, `selected_character[]`).
- Broadcasts lobby updates and start signal.

### `client/core/`
- Owns global game state (`g_game`).
- Handles movement, collision checks, and CTF logic.
- Maintains per-player character selection and sprite texture assignment.
- Scans character catalog from `textures/` at startup.

### `client/networking/`
- `net.c`: socket helpers and connection setup.
- `peer_manager.c`: peer socket ownership.
- `recv_thread.c`: async receive loop for lobby + in-game updates.

### `client/render/`
- `raycaster.c`: wall casting + depth per screen column.
- `sprite.c`: remote player billboard rendering with wall-occlusion clipping.
- `texture.c`: PPM loading, texture clone, GL texture bind/upload.
- `render.c`: lobby UI and main render dispatch.

### `client/simulation/`
- `level.c`: ASCII-authored map parser to wall grid.
- `level.h`: dimensions and wall-type constants.

### `common/`
- Shared protocol structures/constants used by client and server.

---

## 4) Graphics pipeline details

CGV uses a classic 2.5D approach.

### 4.1 Camera + projection
- Player has `(x, y, angle, fov)`.
- One vertical ray is cast per output column.
- Ray hit distance controls projected wall slice height:
	$$h_{screen} \propto \frac{1}{d_{wall}}$$

### 4.2 Wall rendering
- Ray traversal samples the map grid until a solid wall tile is hit.
- Each column stores wall depth in an internal depth array.
- Distance-based shading is applied to wall slices.

### 4.3 Sprite rendering
- Each remote player is a vertical quad billboard in camera space.
- Sprite screen rectangle is computed from angle and distance.
- Texture is sampled through OpenGL texturing.
- Per-column wall depth test clips sprite columns behind walls.

This prevents classic “see-through wall” artifacts when peeking around corners.

### 4.4 UI rendering
- Before match start: full-screen lobby overlay with text + texture previews.
- During match: first-person rendering + HUD/minimap overlays.

---

## 5) Map system (ASCII-driven)

Map dimensions are fixed at:
- `LEVEL_WIDTH = 40`
- `LEVEL_HEIGHT = 32`

The map is defined as an array of 32 strings with 40 chars each.

Tile legend:
- `#` → `WALL_BRICK`
- `=` → `WALL_STONE`
- `+` → `WALL_WOOD`
- others (typically `.`) → `WALL_NONE`

Benefits of this approach:
- fast manual iteration,
- clear visual authoring,
- no external parser dependency.

Spawn safety logic opens tiles near known spawn points so players do not spawn blocked.

---

## 6) Character selection and asset pipeline

### 6.1 Discovery
- On startup, client scans `textures/` for `.ppm` files.
- Entries are sorted and converted into display names.

### 6.2 Loading
- Catalog base textures are preloaded to avoid repeated disk I/O during selection.

### 6.3 Selection
- Lobby input:
	- `Q`: previous
	- `E`: next
	- `R`: ready toggle
- Selection is sent to server and rebroadcast to all clients.

### 6.4 Application
- Chosen base texture is cloned for per-player sprite ownership.
- If load fails, placeholder colored texture is used.

---

## 7) Networking model and protocol

### 7.1 Protocol constants
- `MAX_PLAYERS_CONST = 6`
- Lobby messages include:
	- ready toggle,
	- lobby state,
	- start signal,
	- character selection.

### 7.2 Phase split
- **Lobby phase**: authoritative server state distribution.
- **Gameplay phase**: direct peer updates for position/angle.

### 7.3 Concurrency
- Main thread:
	- input,
	- simulation,
	- rendering,
	- outbound player updates.
- Receive thread:
	- inbound lobby packets (pre-start),
	- inbound peer updates (post-start).

Mutexes protect shared game state sections that are touched from multiple threads.

---

## 8) CTF gameplay logic

Tracked values per player:
- total flag hold time,
- number of successful steals.

Steal rules:
- if a non-holder comes within threshold distance of current holder,
- and steal cooldown has expired,
- flag ownership transfers.

This gives a compact competitive objective without additional map objectives.

---

## 9) Build and run model

Build outputs:
- `build/server_app`
- `build/client_app`

Common workflows:
- `make` for release build,
- `make debug` for instrumentation and debugging.

Local launch scripts are available for quick 2-player and 6-player sessions.

---

## 10) Known technical constraints

- PPM-only texture loading in current implementation.
- Peer mesh approach may become difficult to scale beyond small lobbies.
- Uses legacy OpenGL immediate mode for parts of rendering/UI.
- No client prediction/interpolation for smooth remote motion yet.

---

## 11) Recommended next technical steps

1. External map file loader (keep ASCII format, move out of source).
2. Add remote player interpolation/extrapolation.
3. Migrate additional render paths to buffered modern OpenGL.
4. Add disconnect/reconnect handling for mid-session dropouts.
5. Improve UX consistency by renaming legacy scripts/targets.
