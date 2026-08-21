#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct Hud {
    TTF_Font *font;
} Hud;

void hud_init(Hud *hud);
int hud_load(Hud *hud);
void hud_destroy(Hud *hud);
void hud_render_score(SDL_Renderer *renderer, const Hud *hud, int score);

#endif