#!/bin/bash
# Create sample PPM player images

# Create textures directory
mkdir -p textures

# Function to create a simple PPM image
create_ppm() {
    local filename=$1
    local width=$2
    local height=$3
    local r=$4
    local g=$5
    local b=$6
    
    {
        echo "P6"
        echo "$width $height"
        echo "255"
        
        # Create a simple colored rectangle with a pattern
        for ((y=0; y<height; y++)); do
            for ((x=0; x<width; x++)); do
                # Create a gradient/pattern
                local shade=$((200 + (x * 55 / width) % 56))
                printf "\\x$(printf '%02x' $((r * shade / 255)))"
                printf "\\x$(printf '%02x' $((g * shade / 255)))"
                printf "\\x$(printf '%02x' $((b * shade / 255)))"
            done
        done
    } > "$filename"
}

# Create 4 player textures
echo "Creating player textures..."
create_ppm textures/player_red.ppm 64 128 255 100 100      # Red-ish
create_ppm textures/player_green.ppm 64 128 100 255 100    # Green-ish  
create_ppm textures/player_blue.ppm 64 128 100 100 255     # Blue-ish
create_ppm textures/player_yellow.ppm 64 128 255 255 100   # Yellow-ish

echo "Player textures created in textures/ directory"
echo "Usage: Load with texture_load_ppm(\"textures/player_red.ppm\")"
