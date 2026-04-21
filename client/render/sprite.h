#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "../core/game.h"

typedef struct {
    int player_id;
    Texture *texture;
    float width, height;  /* Billboard size in world units */
} PlayerSprite;

typedef struct {
    PlayerSprite sprites[MAX_PLAYERS];
    int count;
} SpriteManager;

extern SpriteManager g_sprite_manager;

void sprite_manager_init(void);
void sprite_manager_shutdown(void);

/* Set player sprite texture */
void sprite_set_texture(int player_id, Texture *tex, float width, float height);

/* Get sprite for player */
PlayerSprite *sprite_get(int player_id);

/* Draw all visible player sprites during raycasting */
void sprite_render_players(const PlayerState *camera);

#endif
