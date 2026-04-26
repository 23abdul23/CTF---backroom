# CGV Viva Preparation - Team Split and Readiness Plan

## Team Members
- **Kavyan (Lead)**
- **Khushi**
- **Abdul**
- **RB**
- **Mohit**

This document splits ownership, summarizes contributions in this project, and lists what each member should prepare for viva.

---

## 1) Project Snapshot (everyone must know)

**Project:** CGV multiplayer raycasting FPS prototype in C/OpenGL.

**Current implemented highlights:**
- 6-player support (up from 4)
- Lobby + ready gating before match start
- Character selection (fighting-game style) in lobby
- Folder-driven texture discovery from `textures/*.ppm`
- CTF flag-holder/steal/cooldown gameplay logic
- ASCII-based map builder
- Sprite rendering performance improvements
- Per-column depth occlusion fix to prevent wall peek-through artifacts
- Server-side lobby reliability fixes

**Architecture in one line:**
- Server handles matchmaking/lobby sync, clients render locally and exchange gameplay updates peer-to-peer after start.

---

## 2) Work Split and Contribution Mapping

## Kavyan - Architecture, Integration, Final Stability

### Contribution Summary
- Led end-to-end feature planning and integration.
- Coordinated transition from basic startup flow to full lobby pipeline.
- Integrated 6-player scaling across protocol, client, server, scripts.
- Final stability pass and bug resolution (including server behavior fixes).
- Ensured feature compatibility across networking + rendering + gameplay.

### Key Files to Own
- `common/protocol.h`
- `client/client.c`
- `client/core/game.h`
- `client/core/game.c`
- `server/server.c`
- `server/server.h`
- `Makefile`
- `run_localhost_2players.sh`
- `run_localhost_4players.sh`

### Concepts to Master
- Full data flow: join -> lobby -> start -> peer updates.
- Why server is used in lobby but gameplay is peer-sync heavy.
- Threading model and shared state protection with mutexes.
- Feature integration risks when changing constants like max players.
- Tradeoff decisions: simplicity vs scalability vs reliability.

### Viva-ready Talking Points
- Explain why `MAX_PLAYERS` changes require multi-file edits.
- Explain how lead ensured no subsystem broke after each major change.
- Explain current known constraints and future roadmap.

### Potential Bugs/Issues Encountered + Resolution
- **Issue:** Client showed no window when only one player connected.
   - **Cause:** Server waits for minimum players before full startup response flow finishes.
   - **Resolution:** Added clear runbook guidance: launch at least 2 clients for lobby/game startup.
- **Issue:** `Bind failed: Address already in use` on server start.
   - **Cause:** Previous `server_app` process still listening on port `5000`.
   - **Resolution:** Added operational check and process cleanup workflow before restart.
- **Issue:** Feature merges broke constants consistency (4 vs 6 players).
   - **Cause:** Multi-module constants drift across protocol/client/server arrays.
   - **Resolution:** Unified constants and audited all dependent arrays/loops/scripts together.

---

## Khushi - Documentation, UX Clarity, User Journey

### Contribution Summary
- Improved project communication and user-facing clarity.
- Helped align docs with current implementation state.
- Improved discoverability of controls and startup behavior.
- Supported usability of lobby flow and player onboarding.

### Key Files to Own
- `README.md`
- `PROJECT_OVERVIEW_AND_CG_GUIDE.md`
- `PLAYER_SPRITES.md`
- `QUICKSTART.md`
- `DEVELOPMENT.md`
- `IMPROVEMENTS.md`

### Concepts to Master
- Build/run prerequisites and launch commands.
- Difference between localhost scripts and multi-system LAN usage.
- Lobby controls vs in-game controls.
- Texture pipeline constraints (PPM only) and troubleshooting.

### Viva-ready Talking Points
- Why docs needed updates after 6-player and lobby changes.
- How documentation reduces operational failures in demos.
- How to onboard a new contributor in under 10 minutes.

### Potential Bugs/Issues Encountered + Resolution
- **Issue:** Docs described 4 players while code supported 6.
   - **Cause:** Documentation lagged behind implementation changes.
   - **Resolution:** Updated all major docs to reflect current limits and lobby flow.
- **Issue:** Users launched client without server IP and saw failures.
   - **Cause:** Command usage not obvious in old docs.
   - **Resolution:** Added explicit run commands and common LAN examples.
- **Issue:** Confusion around server disconnect message after lobby.
   - **Cause:** Architecture phase shift (lobby server -> peer gameplay) not clearly documented.
   - **Resolution:** Added architecture explanation and expected behavior notes.

---

## Abdul - Rendering, Texture Pipeline, Graphics Performance

### Contribution Summary
- Worked on sprite rendering quality/performance path.
- Improved texture handling for smoother character selection.
- Helped optimize preview and runtime texture usage.
- Contributed to visual correctness with occlusion work.

### Key Files to Own
- `client/render/render.c`
- `client/render/raycaster.c`
- `client/render/raycaster.h`
- `client/render/sprite.c`
- `client/render/sprite.h`
- `client/render/texture.c`
- `client/render/texture.h`
- `textures/` (asset understanding)

### Concepts to Master
- Raycasting math basics (`distance`, `projection`, `FOV`).
- Billboard sprite placement in camera space.
- Texture upload/binding lifecycle and memory management.
- Per-column wall depth clipping for sprites (occlusion fix).
- Why reducing repeated texture loads improves FPS.

### Viva-ready Talking Points
- Explain old lag symptom and why optimization fixed it.
- Explain wall peek-through bug root cause and final fix.
- Explain CPU vs GPU responsibilities in current renderer.

### Potential Bugs/Issues Encountered + Resolution
- **Issue:** Lobby character preview and selection felt slow.
   - **Cause:** Repeated heavy texture handling and large texture cost in draw path.
   - **Resolution:** Preloaded catalog textures and switched to clone/apply strategy for smoother selection.
- **Issue:** In-game sprite rendering lag under load.
   - **Cause:** Expensive sprite rendering path and texture handling overhead.
   - **Resolution:** Improved rendering path with GPU texturing and reduced repeated expensive operations.
- **Issue:** Player sprite visible through walls at edge peeks.
   - **Cause:** Occlusion check was not strict per screen column.
   - **Resolution:** Added per-column depth comparison using raycaster wall depth buffer.

---

## RB - Networking and Server-Lobby Protocol

### Contribution Summary
- Implemented/maintained lobby messaging protocol updates.
- Worked on ready state and character selection synchronization.
- Improved server-side lobby robustness and lifecycle handling.
- Supported debugging of connection/disconnect startup issues.

### Key Files to Own
- `common/protocol.h`
- `client/networking/net.c`
- `client/networking/net.h`
- `client/networking/recv_thread.c`
- `client/networking/recv_thread.h`
- `client/networking/peer_manager.c`
- `client/networking/peer_manager.h`
- `server/server.c`

### Concepts to Master
- Packet structures and message types.
- Blocking send/receive (`send_all`, `recv_all`) semantics.
- `select()` based socket multiplexing.
- Lobby state broadcasts (`connected[]`, `ready[]`, `selected_character[]`).
- Why/when server socket is closed and gameplay continues.

### Viva-ready Talking Points
- Explain exact startup handshake and assignment logic.
- Explain why lobby server disconnect logs may be expected in current design.
- Explain failure cases: port in use, one client only, NAT issues.

### Potential Bugs/Issues Encountered + Resolution
- **Issue:** Lobby socket disconnect seen by clients.
   - **Cause:** Server lifecycle ends after broadcasting start in current design.
   - **Resolution:** Confirmed as expected behavior and documented transition to peer gameplay phase.
- **Issue:** Ready/character updates failed after disconnect.
   - **Cause:** Client attempted to send lobby packets after server socket closure.
   - **Resolution:** Added guards and error logging; operationally ensure lobby interactions occur before start.
- **Issue:** Connection issues across non-LAN environments.
   - **Cause:** Peer-to-peer path needs mutual reachability (NAT/firewall constraints).
   - **Resolution:** Recommended same LAN/VPN usage and firewall/port checks.

---

## Mohit - Gameplay Logic, Map System, Player Experience

### Contribution Summary
- Worked on gameplay-side feature progression (lobby to gameplay transitions).
- Improved map design and eventually shifted to ASCII map pipeline.
- Supported spawn safety and larger map/play area adjustments.
- Contributed to practical gameplay balancing with wider lanes/open flow.

### Key Files to Own
- `client/simulation/level.c`
- `client/simulation/level.h`
- `client/core/game.c`
- `client/core/game.h`
- `client/input/input.c`
- `client/input/input.h`

### Concepts to Master
- Grid map representation and wall typing.
- ASCII map parsing and why it improves iteration speed.
- Spawn placement and anti-stuck spawn clearing.
- Movement, collision checks, and turn/strafe controls.
- CTF mechanics (`flag_holder`, steal distance, cooldown, hold timers).

### Viva-ready Talking Points
- Why ASCII map design was chosen over procedural hardcoding.
- How map edits affect both rendering and collision.
- How gameplay loop remains deterministic enough for a prototype.

### Potential Bugs/Issues Encountered + Resolution
- **Issue:** Map iteration was slow with procedural/hardcoded builder style.
   - **Cause:** Geometry edits required code-logic edits rather than direct layout edits.
   - **Resolution:** Migrated to ASCII map parser for faster visual map editing.
- **Issue:** Spawn positions could collide with map walls after layout changes.
   - **Cause:** Map updates invalidated old spawn assumptions.
   - **Resolution:** Added spawn safety clearing near spawn cells during map init.
- **Issue:** Early maps felt cramped and movement was frustrating.
   - **Cause:** Corridor width and wall density too high.
   - **Resolution:** Iteratively widened lanes and reduced wall density while preserving structure.

---

## 3) Cross-Training Matrix (minimum each member should know)

Each person should be able to explain at least:
1. How to start server and clients on same LAN.
2. Why minimum 2 players are needed before lobby/start flow proceeds.
3. Difference between lobby-server sync and gameplay peer updates.
4. One major bug fixed and the technical fix.
5. One future improvement with implementation direction.

---

## 4) Viva Question Bank (short answers expected)

1. **Why raycasting and not full 3D meshes?**
   - Simpler 2.5D pipeline, faster to build/debug for course timeline.

2. **How do you prevent seeing sprites through walls?**
   - Compare sprite columns against per-column wall depth from raycaster and clip hidden columns.

3. **How is multiplayer synchronized?**
   - Server for matchmaking/lobby state; clients exchange player updates in gameplay.

4. **How did you scale from 4 to 6 players?**
   - Unified constants and arrays in protocol/client/server/render/network scripts.

5. **How are character textures managed?**
   - Runtime folder scan of `textures/*.ppm`, sorted catalog, selection broadcast, texture assignment per player.

6. **What are known limitations?**
   - PPM-only assets, limited NAT friendliness, basic interpolation, classic OpenGL path.

---

## 5) Demo Order for Viva (recommended)

1. **Kavyan:** architecture and startup flow (1-2 min)
2. **RB:** networking handshake and lobby protocol (1-2 min)
3. **Khushi:** docs + controls + runbook clarity (1 min)
4. **Mohit:** ASCII map + gameplay/CTF logic (1-2 min)
5. **Abdul:** rendering + performance + occlusion fix (1-2 min)

Finish with one limitation and one roadmap item from each person.

---

## 6) 1-Day Preparation Checklist

### Everyone
- Build once: `make`
- Run local test with 2 clients and 1 server.
- Memorize controls and startup sequence.
- Read this file and your owned files once end-to-end.

### Lead (Kavyan)
- Conduct 20-minute mock viva:
  - architecture
  - one bug story per member
  - one tradeoff explanation per member

---

## 7) Final Viva Rule

If asked a cross-module question, answer in this order:
1. **What component** handles it,
2. **Which file/function** implements it,
3. **Why design choice** was taken,
4. **What limitation** remains,
5. **How to improve** next.

This structure keeps answers clear, technical, and high-scoring.
