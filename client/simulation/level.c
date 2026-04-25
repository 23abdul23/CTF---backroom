#include "level.h"
#include <string.h>

Level g_level;

static void fill_rect(int x1, int y1, int x2, int y2, int wall_type) {
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2) {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= LEVEL_WIDTH) x2 = LEVEL_WIDTH - 1;
    if (y2 >= LEVEL_HEIGHT) y2 = LEVEL_HEIGHT - 1;

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            g_level.grid[y][x] = wall_type;
        }
    }
}

static void carve_rect(int x1, int y1, int x2, int y2) {
    fill_rect(x1, y1, x2, y2, WALL_NONE);
}

static void build_wide_map(void) {
    /* Outer shell */
    fill_rect(0, 0, LEVEL_WIDTH - 1, 0, WALL_BRICK);
    fill_rect(0, LEVEL_HEIGHT - 1, LEVEL_WIDTH - 1, LEVEL_HEIGHT - 1, WALL_BRICK);
    fill_rect(0, 0, 0, LEVEL_HEIGHT - 1, WALL_BRICK);
    fill_rect(LEVEL_WIDTH - 1, 0, LEVEL_WIDTH - 1, LEVEL_HEIGHT - 1, WALL_BRICK);

    /* Broad horizontal barriers with wide gaps. */
    fill_rect(2, 8, LEVEL_WIDTH - 3, 8, WALL_STONE);
    carve_rect(6, 7, 10, 9);
    carve_rect(18, 7, 23, 9);
    carve_rect(30, 7, 35, 9);

    fill_rect(2, 20, LEVEL_WIDTH - 3, 20, WALL_STONE);
    carve_rect(6, 19, 10, 21);
    carve_rect(18, 19, 23, 21);
    carve_rect(30, 19, 35, 21);

    /* Broad vertical barriers with wide openings. */
    fill_rect(11, 2, 11, LEVEL_HEIGHT - 3, WALL_WOOD);
    carve_rect(10, 5, 12, 9);
    carve_rect(10, 14, 12, 17);
    carve_rect(10, 24, 12, 27);

    fill_rect(27, 2, 27, LEVEL_HEIGHT - 3, WALL_WOOD);
    carve_rect(26, 5, 28, 9);
    carve_rect(26, 14, 28, 17);
    carve_rect(26, 24, 28, 27);

    /* Sparse blockers to keep some structure without crowding lanes. */
    fill_rect(15, 4, 17, 6, WALL_BRICK);
    fill_rect(31, 12, 34, 14, WALL_BRICK);
    fill_rect(4, 14, 6, 16, WALL_STONE);
    fill_rect(18, 24, 21, 26, WALL_STONE);

    /* Open up the southeast section into a wider wing. */
    carve_rect(33, 2, LEVEL_WIDTH - 2, LEVEL_HEIGHT - 2);
    fill_rect(34, 6, 38, 6, WALL_BRICK);
    fill_rect(34, 24, 38, 24, WALL_BRICK);
    carve_rect(35, 6, 37, 7);
    carve_rect(35, 23, 37, 24);
}

void level_init(void) {
    memset(&g_level, 0, sizeof(g_level));
    build_wide_map();

    /* Keep spawn zones open even if the tile art changes later. */
    const int spawns[][2] = {
        {6, 6}, {23, 23}, {26, 6}, {6, 26}
    };
    for (int i = 0; i < 4; ++i) {
        int sx = spawns[i][0];
        int sy = spawns[i][1];
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int x = sx + dx;
                int y = sy + dy;
                if (x > 0 && x < LEVEL_WIDTH - 1 && y > 0 && y < LEVEL_HEIGHT - 1) {
                    g_level.grid[y][x] = WALL_NONE;
                }
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
