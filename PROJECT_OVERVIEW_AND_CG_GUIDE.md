# CGV Project Overview and Computer Graphics Implementation Guide

## 1) What this project does

This project is a C-based, networked, real-time raycasting game prototype.

Core behavior:
- A matchmaking server accepts players and shares connection info.
- Clients connect, then form a peer mesh for gameplay data exchange.
- Each client renders a first-person 3D-like scene using 2D raycasting.
- Players move in a grid-based map (maze) with wall collision.
- Other players are rendered as billboard sprites.
- Position/angle updates are synchronized over sockets in real time.

Primary modules:
- `server/`: matchmaking and session start logic.
- `client/core/`: game state and per-frame update.
- `client/networking/`: connection setup, peer sockets, receive thread.
- `client/render/`: raycaster, sprites, textures, main rendering pipeline.
- `client/simulation/level.*`: map data and wall queries.
- `common/protocol.h`: packet structures and shared constants.

Build/runtime:
- Built with GCC and a Makefile.
- Uses OpenGL/GLUT for display and draw calls.
- Uses pthreads for async receiving/network handling.

---

## 2) How the rendering works in this project

### 2.1 Raycasting pipeline
1. For each vertical screen column, cast one ray from player position at a specific angle.
2. Step through the world grid until a wall cell is hit.
3. Compute wall distance and projected wall-slice height.
4. Draw a vertical wall slice (with distance-based brightness).
5. Draw sky/floor for non-hit regions.
6. Draw sprites (other players) with blending and occlusion checks.

This is a classic “2.5D” approach: the world is stored as a 2D map, but rendered to appear 3D.

### 2.2 Sprite rendering
- Each remote player is a billboard sprite (flat image facing camera space).
- Sprite visibility is filtered by distance and field-of-view.
- A ray-to-sprite occlusion check prevents rendering through walls.
- Sprite texture pixels are sampled and alpha-tested for transparency.

### 2.3 Map representation
- The level is a grid (`LEVEL_WIDTH x LEVEL_HEIGHT`).
- Each cell stores wall type (`none`, `brick`, `stone`, `wood`, etc.).
- Collision and ray intersections use grid lookups.

---

## 3) Networking architecture in this project

### 3.1 Matchmaking phase
- Server listens on a TCP port.
- Clients send join info (including local listen port).
- Server assigns player IDs and broadcasts a `JoinResponse` with peer endpoints.

### 3.2 Gameplay phase
- Clients connect to each other (peer mesh).
- A receive thread listens for remote `PlayerUpdate` packets.
- Main thread updates local movement and periodically sends local position/angle.
- Shared state is protected with mutexes where required.

---

## 4) How computer graphics projects are generally implemented

A practical implementation path for graphics projects:

1. Define goals
- Visual style (2D, 2.5D, 3D)
- Performance target (FPS, resolution)
- Platform/runtime constraints

2. Choose render architecture
- Software rasterization, fixed pipeline, or shader pipeline
- Camera model and projection strategy
- Data model for world geometry

3. Build core loop
- Input -> simulation update -> render -> present
- Stable timestep or fixed delta update

4. Build scene data structures
- Spatial layout (grid, BSP, mesh, ECS)
- Asset formats (textures, sprites, models)
- Material/lighting data

5. Implement visibility and draw ordering
- Frustum/FOV culling
- Depth sorting or depth buffer
- Occlusion handling

6. Add interaction and physics/collision
- Movement model
- Collision volumes and response
- Game rules

7. Add networking (if multiplayer)
- Authority model (server-authoritative, peer, hybrid)
- Packet protocol and serialization
- State replication/interpolation/reconciliation

8. Optimize and profile
- CPU hotspots
- Draw call count
- Memory allocations
- Network packet frequency/size

9. Tooling and debugging
- Logging
- Render debug overlays (minimap, bounding visuals, ray debug)
- Deterministic repro cases

10. Polish
- Better textures/art direction
- UI/HUD refinement
- Audio and feedback
- Stability and edge-case handling

---

## 5) Typical deliverables for CG projects

- Rendering engine/module
- Scene/map/asset pipeline
- Input and simulation systems
- Performance metrics and profiling notes
- Build scripts and run instructions
- Technical documentation (architecture + tradeoffs)

---

## 6) Suggested next improvements for this project

- Texture-mapped walls (sample wall textures by hit coordinate)
- Better sprite depth handling per screen column
- Minimap polish and configurable HUD
- Network smoothing/interpolation for remote players
- Map loader from external files (instead of hardcoded grid)
- Robust disconnect/reconnect handling
- Optional migration to modern OpenGL shader pipeline
