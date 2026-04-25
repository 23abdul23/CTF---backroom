#include "render.h"
#include "raycaster.h"
#include "sprite.h"

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "../config.h"
#include "../core/game.h"
#include "../simulation/level.h"

static void draw_text(float x, float y, void *font, const char *text) {
    glRasterPos2f(x, y);
    for (const char *c = text; *c; ++c) {
        glutBitmapCharacter(font, *c);
    }
}

static void draw_texture_preview(Texture *tex, float x, float y, float w, float h) {
    glColor3f(0.18f, 0.20f, 0.26f);
    glBegin(GL_QUADS);
    glVertex2f(x - 6.0f, y - 6.0f);
    glVertex2f(x + w + 6.0f, y - 6.0f);
    glVertex2f(x + w + 6.0f, y + h + 6.0f);
    glVertex2f(x - 6.0f, y + h + 6.0f);
    glEnd();

    if (!tex || !tex->pixels || tex->width <= 0 || tex->height <= 0) {
        glColor3f(0.75f, 0.30f, 0.30f);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
        return;
    }

    const float px_w = w / (float)tex->width;
    const float px_h = h / (float)tex->height;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int ty = 0; ty < tex->height; ++ty) {
        for (int tx = 0; tx < tex->width; ++tx) {
            unsigned char r, g, b, a;
            texture_get_pixel(tex, tx, ty, &r, &g, &b, &a);
            if (a < 8) {
                continue;
            }

            const float x1 = x + tx * px_w;
            const float y1 = y + ty * px_h;
            const float x2 = x1 + px_w;
            const float y2 = y1 + px_h;

            glColor4ub(r, g, b, a);
            glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
            glEnd();
        }
    }

    glDisable(GL_BLEND);
}

static void render_lobby_screen(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)WIDTH, (double)HEIGHT, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.10f, 0.13f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f((float)WIDTH, 0.0f);
    glVertex2f((float)WIDTH, (float)HEIGHT);
    glVertex2f(0.0f, (float)HEIGHT);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text(40.0f, 60.0f, GLUT_BITMAP_HELVETICA_18, "CGV Lobby");
    draw_text(40.0f, 90.0f, GLUT_BITMAP_HELVETICA_12, "Mortal Kombat style selection: Q/E to choose fighter, R to READY");

    PlayerSprite *local_sprite = sprite_get(g_game.local_player_id);
    draw_text(40.0f, 130.0f, GLUT_BITMAP_HELVETICA_12, "Your texture preview:");
    draw_texture_preview(local_sprite ? local_sprite->texture : NULL, 40.0f, 145.0f, 120.0f, 180.0f);

    {
        char texture_label[196];
        snprintf(texture_label, sizeof(texture_label), "Texture: %s", g_game.texture_files[g_game.local_player_id]);
        glColor3f(0.85f, 0.85f, 0.90f);
        draw_text(40.0f, 345.0f, GLUT_BITMAP_HELVETICA_12, texture_label);
    }

    {
        int local_selected = game_get_player_character(g_game.local_player_id);
        char selected_label[196];
        snprintf(selected_label, sizeof(selected_label), "Selected fighter: %s", game_get_character_name(local_selected));
        glColor3f(1.0f, 0.95f, 0.65f);
        draw_text(40.0f, 362.0f, GLUT_BITMAP_HELVETICA_12, selected_label);
    }

    glColor3f(0.85f, 0.90f, 1.0f);
    draw_text(40.0f, 390.0f, GLUT_BITMAP_HELVETICA_12, "Available fighters:");
    for (int i = 0; i < game_get_character_count(); ++i) {
        const int is_selected = (i == game_get_player_character(g_game.local_player_id));
        char fighter_line[128];
        snprintf(fighter_line, sizeof(fighter_line), "%s %s", is_selected ? ">" : " ", game_get_character_name(i));
        glColor3f(is_selected ? 1.0f : 0.8f, is_selected ? 0.95f : 0.8f, is_selected ? 0.45f : 0.85f);
        draw_text(40.0f, (float)(408 + i * 14), GLUT_BITMAP_HELVETICA_12, fighter_line);
    }

    int y = 145;
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (!game_get_connected_player(i)) {
            continue;
        }

        const int is_local = (i == g_game.local_player_id);
        const int is_ready = game_get_player_ready(i);
        const int selected = game_get_player_character(i);
        char line[128];
        snprintf(line, sizeof(line), "Player %d%s  -  %s  -  %s", i, is_local ? " (You)" : "", is_ready ? "READY" : "NOT READY", game_get_character_name(selected));

        if (is_ready) {
            glColor3f(0.4f, 1.0f, 0.5f);
        } else {
            glColor3f(1.0f, 0.55f, 0.45f);
        }
        draw_text(220.0f, (float)y, GLUT_BITMAP_HELVETICA_18, line);
        y += 30;
    }

    glColor3f(0.9f, 0.9f, 0.9f);
    if (game_get_local_ready()) {
        draw_text(40.0f, (float)(HEIGHT - 60), GLUT_BITMAP_HELVETICA_18, "You are READY. Waiting for others...");
    } else {
        draw_text(40.0f, (float)(HEIGHT - 60), GLUT_BITMAP_HELVETICA_18, "You are NOT READY. Press R when ready.");
    }
}

void render_init(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)WIDTH, 0.0, (double)HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.06f, 0.08f, 0.10f, 1.0f);
    
    /* Initialize level */
    level_init();
}

void render_scene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!game_has_started()) {
        render_lobby_screen();
    } else {
        /* Render first-person view using raycasting */
        raycaster_render(&g_game.local_player);
    }
    
    glutSwapBuffers();
}