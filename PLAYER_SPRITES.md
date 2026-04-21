# Custom Player Sprites Guide

## Overview

Your CGV FPS now supports custom player sprites! Instead of boring boxes, each player is rendered as a billboarded flat image that always faces the camera.

## Using the Built-in Textures

The game comes with 4 pre-generated colored player textures:
- `textures/player_red.ppm` - Player 0 (Red)
- `textures/player_green.ppm` - Player 1 (Green)
- `textures/player_blue.ppm` - Player 2 (Blue)
- `textures/player_yellow.ppm` - Player 3 (Yellow)

Just run the game and you'll see other players as colored flat images!

## Creating Your Own Player Sprites

### Format
Player sprites must be in **PPM format (P6 - binary RGB)**:
```
P6
WIDTH HEIGHT
255
[RGB pixel data...]
```

### Easy Method: Use ImageMagick

Convert any image to PPM:
```bash
convert your_image.png -depth 8 custom_player.ppm
```

Or generate from scratch:
```bash
convert -size 64x128 xc:red player_sprite.ppm
```

### Manual PPM Creation

Create a PPM file with your favorite tool:

**Python example:**
```python
from PIL import Image

# Create or load image
img = Image.open('player_face.png')
img = img.convert('RGB')
img = img.resize((64, 128))

# Save as PPM
img.save('player_sprite.ppm')
```

**Bash example:**
```bash
# Create 64x128 red image
{
    echo "P6"
    echo "64 128"
    echo "255"
    for i in {1..8192}; do
        printf '\xff\x00\x00'  # RGB for red
    done
} > player_red.ppm
```

### Using Your Custom Sprites

1. Create your PPM file and place it in `textures/` directory
2. Edit [client/core/game.c](client/core/game.c) around line 28:
   ```c
   const char *texture_files[] = {
       "textures/your_player_0.ppm",
       "textures/your_player_1.ppm",
       "textures/your_player_2.ppm",
       "textures/your_player_3.ppm"
   };
   ```
3. Rebuild: `make`
4. Run: `make run-localhost`

## Technical Details

### How It Works

1. **Texture Loading** - [client/render/texture.c](client/render/texture.c)
   - Loads PPM files into memory
   - Converts RGB to RGBA format
   - Stores pixel data

2. **Sprite System** - [client/render/sprite.c](client/render/sprite.c)
   - Manages 4 player sprites
   - Detects when other players are in your FOV
   - Calculates screen position and size based on distance
   - Renders pixel-by-pixel with perspective correction

3. **Billboard Rendering** - [client/render/raycaster.c](client/render/raycaster.c)
   - Sprites always face the camera
   - Distance-based FOV checks
   - Alpha blending for transparency
   - Proper depth sorting with walls

### Image Recommendations

For best results:
- **Size**: 64x128 pixels (portrait orientation)
- **Format**: PPM P6 (binary RGB)
- **Transparency**: Use low alpha values for see-through areas
- **Style**: High contrast, visible from distance
- **Color**: Avoid similar colors to walls (brick, stone are brown/gray)

## Examples

### Create a Simple Sprite with ImageMagick

**Solid color with number:**
```bash
convert -size 64x128 xc:blue \
  -fill white -pointsize 48 -gravity center -annotate +0+0 '1' \
  player_1.ppm
```

**Gradient:**
```bash
convert -size 64x128 gradient:blue-cyan player_gradient.ppm
```

**Pattern:**
```bash
convert -size 64x128 \
  \( -size 64x64 xc:red \) \
  \( -size 64x64 xc:blue \) \
  -append \
  player_bicolor.ppm
```

## Debugging

If sprites aren't showing:

1. Check textures load: Look for messages like `Loaded texture: textures/player_red.ppm (64x128)`
2. Verify PPM format: `file textures/player_red.ppm` should show it's PPM P6
3. Test visibility: Move around - sprites only render when other players are in your FOV

## Future Enhancements

Could add:
- PNG/JPEG support via external library
- Sprite atlas for animations
- Rotation/animation based on player direction
- Per-client custom texture selection
- Network streaming of custom textures
