#include "level.h"
#include <string.h>

Level g_level;

void level_init(void) {
    /* Create a larger, more complex maze */
    memset(&g_level, 0, sizeof(g_level));
    
    /* Outer walls */
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        g_level.grid[0][x] = WALL_BRICK;
        g_level.grid[LEVEL_HEIGHT-1][x] = WALL_BRICK;
    }
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        g_level.grid[y][0] = WALL_BRICK;
        g_level.grid[y][LEVEL_WIDTH-1] = WALL_BRICK;
    }
    
    /* Main corridors - create a maze with multiple interconnected rooms */
    
    /* Horizontal corridors at various heights */
    for (int y = 4; y < LEVEL_HEIGHT - 4; y += 8) {
        for (int x = 2; x < LEVEL_WIDTH - 2; x++) {
            g_level.grid[y][x] = WALL_STONE;
        }
        /* Add doorways to connect areas */
        g_level.grid[y][10] = WALL_NONE;
        g_level.grid[y][20] = WALL_NONE;
    }
    
    /* Vertical corridors at various widths */
    for (int x = 5; x < LEVEL_WIDTH - 5; x += 10) {
        for (int y = 2; y < LEVEL_HEIGHT - 2; y++) {
            g_level.grid[y][x] = WALL_WOOD;
        }
        /* Add doorways */
        g_level.grid[8][x] = WALL_NONE;
        g_level.grid[16][x] = WALL_NONE;
        g_level.grid[24][x] = WALL_NONE;
    }
    
    /* Add some room-like areas with walls */
    for (int x = 12; x < 18; x++) {
        for (int y = 12; y < 18; y++) {
            if (x == 12 || x == 17 || y == 12 || y == 17) {
                g_level.grid[y][x] = WALL_BRICK;
            }
        }
    }
    
    /* Add another room structure */
    for (int x = 22; x < 28; x++) {
        for (int y = 6; y < 10; y++) {
            if (x == 22 || x == 27 || y == 6 || y == 9) {
                g_level.grid[y][x] = WALL_STONE;
            }
        }
    }
}

int level_get_wall(int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return WALL_BRICK;
    }
    return g_level.grid[y][x];
}

int level_is_walkable(float x, float y) {
    int grid_x = (int)x;
    int grid_y = (int)y;
    
    if (grid_x < 1 || grid_x >= LEVEL_WIDTH - 1 || 
        grid_y < 1 || grid_y >= LEVEL_HEIGHT - 1) {
        return 0;
    }
    
    return g_level.grid[grid_y][grid_x] == WALL_NONE;
}
