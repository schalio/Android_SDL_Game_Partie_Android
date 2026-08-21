#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct Hud {
    TTF_Font *font;
    TTF_Font *title_font;
} Hud;

void hud_init(Hud *hud);
int hud_load(Hud *hud);
void hud_destroy(Hud *hud);

void hud_render_score(SDL_Renderer *renderer, const Hud *hud, int score);
void hud_render_lives(SDL_Renderer *renderer, const Hud *hud, int score);

void hud_render_game_over(
        SDL_Renderer *renderer,
        const Hud *hud,
        int screen_w,
        int screen_h,
        int score
);

#endif