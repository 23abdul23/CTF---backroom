#ifndef LEVEL_H
#define LEVEL_H

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 32

/* Wall types */
#define WALL_NONE 0
#define WALL_BRICK 1
#define WALL_STONE 2
#define WALL_WOOD 3

typedef struct {
    int grid[LEVEL_HEIGHT][LEVEL_WIDTH];
} Level;

extern Level g_level;

void level_init(void);
int level_get_wall(int x, int y);
int level_is_walkable(float x, float y);

#endif
