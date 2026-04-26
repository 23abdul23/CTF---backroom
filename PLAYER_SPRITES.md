# Player Sprites and Character Selection Guide (Current)

This guide describes how player sprites work in CGV **right now**, including:
- supported texture format,
- folder-driven character discovery,
- lobby character selection,
- runtime sprite rendering and occlusion.

---

## 1) System overview

In-game players are rendered as **billboard sprites**:
- flat textured quads,
- always facing the camera,
- scaled by distance,
- clipped by wall depth so hidden portions are not drawn.

Character selection is done in the lobby and synchronized across clients.

---

## 2) Current behavior (important changes)

### Not hardcoded to 4 textures anymore
Older behavior used a fixed small texture list. Current behavior is folder-driven:
- scan `textures/` at startup,
- collect all `.ppm` files,
- sort into a catalog,
- expose as selectable characters.

### Supports up to 6 players
The multiplayer limit is now 6. Any connected player can choose any discovered character index.

### Lobby controls for selection
- `Q`: previous character
- `E`: next character
- `R`: ready toggle

---

## 3) Texture format requirements

### Required format
- **PPM P6** (binary RGB) files.

Header layout:
```text
P6
WIDTH HEIGHT
255
[binary RGB bytes...]
```

### Transparency behavior
- The internal texture system uses RGBA buffers for rendering.
- For PPM input, practical transparency comes from conversion/placeholder behavior in engine paths.
- If you require detailed alpha masks, extending loader support (PNG, etc.) is recommended.

---

## 4) How to add new playable characters

1. Create or convert sprite image to `.ppm`.
2. Put it inside `textures/`.
3. Restart game clients (catalog is loaded at init).
4. Use `Q`/`E` in lobby to select it.

No source code edits are required for normal additions.

---

## 5) Quick conversion examples

### ImageMagick: PNG/JPG to PPM
```bash
convert input.png -depth 8 textures/my_character.ppm
```

### ImageMagick: generate simple placeholder
```bash
convert -size 64x128 xc:purple textures/demo_fighter.ppm
```

### Python (Pillow)
```python
from PIL import Image

img = Image.open("my_source.png").convert("RGB").resize((64, 128))
img.save("textures/my_character.ppm")
```

---

## 6) Recommended art specs

For best visibility in current renderer:
- size around **64x128** (portrait),
- high-contrast silhouette,
- avoid very dark colors close to wall palette,
- keep a readable central figure shape (small distant sprites stay legible).

Larger images can work, but increase memory and upload cost.

---

## 7) Engine internals

### 7.1 Catalog loading
At startup, the client:
1. scans `textures/`,
2. filters `.ppm` files,
3. builds display names from filenames,
4. preloads base textures,
5. uses placeholder textures when file load fails.

### 7.2 Per-player assignment
- Each player has a selected character index.
- On selection change, a texture clone is assigned to that player sprite.
- Path info is retained for lobby display.

### 7.3 Lobby sync
- Client sends selection updates to server.
- Server rebroadcasts lobby state including `selected_character[]`.
- All clients apply remote selection updates.

---

## 8) Rendering path details

Sprite draw stage:
1. gather remote player world position,
2. compute distance and angle to camera,
3. reject outside FOV,
4. compute on-screen quad,
5. clip columns behind walls using ray depth buffer,
6. draw textured strips with blending.

This per-column clipping is what prevents “player visible through wall edges” artifacts.

---

## 9) Troubleshooting

### Character does not appear in lobby list
- Ensure extension is `.ppm`.
- Confirm file is readable and valid P6 format.
- Restart clients after adding files.

### Texture appears corrupted
- Re-export with 8-bit RGB.
- Verify header is exactly `P6`, valid dimensions, max value `255`.

### Selection updates not visible on other clients
- Confirm server is running and clients remained connected in lobby.
- Check if all clients are using the updated build.

### In-game sprite still not visible
- Ensure target player is connected and in FOV.
- Move to reduce wall occlusion (clipping now hides blocked parts by design).

---

## 10) Practical workflow for teams

1. Artists drop new `.ppm` files into `textures/`.
2. Developers restart local clients.
3. Test selection cycling in lobby (`Q`/`E`).
4. Validate visibility in-game with at least 2 players.

This keeps iteration simple without code churn.

---

## 11) Future extension ideas

- PNG/JPEG loader support.
- Animated sprite sheets.
- Team-based or role-based character filtering.
- Runtime hot-reload of texture catalog.
- Per-character metadata (hitbox scale, display name, rarity/theme tags).
