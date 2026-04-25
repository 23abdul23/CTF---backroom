#include "level.h"
#include <string.h>

Level g_level;

static int tile_to_wall(char tile) {
    switch (tile) {
        case '#': return WALL_BRICK;
        case '=': return WALL_STONE;
        case '+': return WALL_WOOD;
        default: return WALL_NONE;
    }
}

static void apply_tile_map(void) {
    static const char *const map[LEVEL_HEIGHT] = {
        "################################",
        "#..............==..............#",
        "#..####....####====####....####.#",
        "#..#..#....#..#....#..#....#..#.#",
        "#..#..#....#..#....#..#....#..#.#",
        "#..####....#..#....#..#....####.#",
        "#........==.#..####..#..==........#",
        "#........==.#..#..#..#..==........#",
        "#..####....#..#..#..#..#....####.#",
        "#..#..#....#..#..#..#..#....#..#.#",
        "#..#..#====#..#..#..#..#====#..#.#",
        "#..####....#..#..#..#..#....####.#",
        "#...........#..####..#...........#",
        "#..+++++....#..#..#..#....+++++..#",
        "#..+...+....#..#..#..#....+...+..#",
        "#..+...+====#..####..#====+...+..#",
        "#..+...+....#........#....+...+..#",
        "#..+++++....#..####..#....+++++..#",
        "#...........#..#..#..#...........#",
        "#..####....#..#..#..#..#....####.#",
        "#..#..#....#..#..#..#..#....#..#.#",
        "#..#..#====#..#..#..#..#====#..#.#",
        "#..####....#..#..#..#..#....####.#",
        "#........==.#..####..#..==........#",
        "#........==.#..#..#..#..==........#",
        "#..####....#..#..#..#..#....####.#",
        "#..#..#....#..#..#..#..#....#..#.#",
        "#..#..#....#..#....#..#....#..#.#",
        "#..####....####====####....####.#",
        "#..............==..............#",
        "#..............==..............#",
        "################################"
    };

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            char tile = map[y][x];
            if (tile == '\0') {
                break;
            }
            g_level.grid[y][x] = tile_to_wall(tile);
        }
    }
}

static void widen_corridors(void) {
    int snapshot[LEVEL_HEIGHT][LEVEL_WIDTH];
    memcpy(snapshot, g_level.grid, sizeof(snapshot));

    for (int y = 1; y < LEVEL_HEIGHT - 1; ++y) {
        for (int x = 1; x < LEVEL_WIDTH - 1; ++x) {
            if (snapshot[y][x] != WALL_NONE) {
                continue;
            }

            if (snapshot[y][x - 1] != WALL_NONE && snapshot[y][x + 1] != WALL_NONE) {
                g_level.grid[y][x - 1] = WALL_NONE;
                g_level.grid[y][x + 1] = WALL_NONE;
            }

            if (snapshot[y - 1][x] != WALL_NONE && snapshot[y + 1][x] != WALL_NONE) {
                g_level.grid[y - 1][x] = WALL_NONE;
                g_level.grid[y + 1][x] = WALL_NONE;
            }
        }
    }
}

void level_init(void) {
    memset(&g_level, 0, sizeof(g_level));
    apply_tile_map();
    widen_corridors();

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
