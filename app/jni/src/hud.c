#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "hud.h"

void hud_init(Hud *hud) {
    hud->font = NULL;
}

int hud_load(Hud *hud) {
    hud->font = TTF_OpenFont("fonts/font.ttf", 48);
    if (!hud->font) {
        SDL_Log("TTF_OpenFont failed for font.ttf: %s", TTF_GetError());
        return 0;
    }

    return 1;
}

void hud_destroy(Hud *hud) {
    if (hud->font) {
        TTF_CloseFont(hud->font);
        hud->font = NULL;
    }
}

void hud_render_score(SDL_Renderer *renderer, const Hud *hud, int score) {
    if (!renderer) {
        SDL_Log("hud_render_score: renderer is NULL");
        return;
    }

    if (!hud || !hud->font) {
        SDL_Log("hud_render_score: hud/font is NULL");
        return;
    }

    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);

    SDL_Color text_color = {255, 255, 255, 255};
    SDL_Surface *score_surface = TTF_RenderUTF8_Blended(hud->font, score_text, text_color);
    if (!score_surface) {
        SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
        return;
    }

    SDL_Texture *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    if (!score_texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        SDL_FreeSurface(score_surface);
        return;
    }

    SDL_Rect score_rect = {
            24,
            24,
            score_surface->w,
            score_surface->h
    };

    SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);

    SDL_DestroyTexture(score_texture);
    SDL_FreeSurface(score_surface);
}