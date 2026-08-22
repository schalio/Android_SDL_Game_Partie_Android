#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "hud.h"

static void hud_render_text(
        SDL_Renderer *renderer,
        TTF_Font *font,
        const char *text,
        SDL_Color text_color,
        int x,
        int y
) {
    if (!renderer) {
        SDL_Log("hud_render_text: renderer is NULL");
        return;
    }

    if (!font) {
        SDL_Log("hud_render_text: font is NULL");
        return;
    }

    SDL_Surface *text_surface =
            TTF_RenderUTF8_Blended(font, text, text_color);

    if (!text_surface) {
        SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
        return;
    }

    SDL_Texture *text_texture =
            SDL_CreateTextureFromSurface(renderer, text_surface);

    if (!text_texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        SDL_FreeSurface(text_surface);
        return;
    }

    SDL_Rect text_rect = {
            x,
            y,
            text_surface->w,
            text_surface->h
    };

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void hud_init(Hud *hud) {
    hud->font = NULL;
    hud->title_font = NULL;
}

int hud_load(Hud *hud) {
    hud->font = TTF_OpenFont("fonts/font.ttf", 48);

    if (!hud->font) {
        SDL_Log("TTF_OpenFont failed for font.ttf: %s", TTF_GetError());
        return 0;
    }

    hud->title_font = TTF_OpenFont("fonts/font.ttf", 72);

    if (!hud->title_font) {
        SDL_Log("TTF_OpenFont failed for title font: %s", TTF_GetError());
        TTF_CloseFont(hud->font);
        hud->font = NULL;
        return 0;
    }

    return 1;
}

void hud_destroy(Hud *hud) {
    if (!hud) {
        return;
    }

    if (hud->title_font) {
        TTF_CloseFont(hud->title_font);
        hud->title_font = NULL;
    }

    if (hud->font) {
        TTF_CloseFont(hud->font);
        hud->font = NULL;
    }
}

void hud_render_score(SDL_Renderer *renderer, const Hud *hud, int score) {
    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);

    SDL_Color text_color = {255, 255, 255, 255};

    hud_render_text(
            renderer,
            hud->font,
            score_text,
            text_color,
            24,
            24
    );
}

void hud_render_level(SDL_Renderer *renderer, const Hud *hud, int level) {
    if (!renderer) {
        SDL_Log("hud_render_level: renderer is NULL");
        return;
    }

    if (!hud || !hud->font) {
        SDL_Log("hud_render_level: hud/font is NULL");
        return;
    }

    char level_text[64];
    snprintf(level_text, sizeof(level_text), "Niveau: %d", level);

    SDL_Color text_color = {255, 255, 255, 255};

    hud_render_text(
            renderer,
            hud->font,
            level_text,
            text_color,
            24,
            84
    );
}

void hud_render_lives(SDL_Renderer *renderer, const Hud *hud, int lives) {
    char lives_text[64];
    snprintf(lives_text, sizeof(lives_text), "Vies: %d", lives);

    SDL_Color text_color = {255, 255, 255, 255};

    hud_render_text(
            renderer,
            hud->font,
            lives_text,
            text_color,
            24,
            144
    );
}

void hud_render_game_over(
        SDL_Renderer *renderer,
        const Hud *hud,
        int screen_w,
        int screen_h,
        int score
) {
    if (!renderer) {
        SDL_Log("hud_render_game_over: renderer is NULL");
        return;
    }

    if (!hud || !hud->font) {
        SDL_Log("hud_render_game_over: hud/font is NULL");
        return;
    }

    const char *game_over_text = "GAME OVER";
    const char *restart_text = "Touchez pour recommencer";

    char final_score_text[64];
    snprintf(final_score_text, sizeof(final_score_text), "Score final : %d", score);

    SDL_Color title_color = {255, 80, 80, 255};
    SDL_Color body_color = {255, 255, 255, 255};

    int title_w = 0;
    int title_h = 0;
    int score_w = 0;
    int score_h = 0;
    int restart_w = 0;
    int restart_h = 0;

    if (TTF_SizeUTF8(hud->title_font, game_over_text, &title_w, &title_h) != 0) {
        SDL_Log("TTF_SizeUTF8 failed for GAME OVER: %s", TTF_GetError());
        return;
    }

    if (TTF_SizeUTF8(hud->font, final_score_text, &score_w, &score_h) != 0) {
        SDL_Log("TTF_SizeUTF8 failed for final score: %s", TTF_GetError());
        return;
    }

    if (TTF_SizeUTF8(hud->font, restart_text, &restart_w, &restart_h) != 0) {
        SDL_Log("TTF_SizeUTF8 failed for restart text: %s", TTF_GetError());
        return;
    }

    int spacing = 24;

    int total_height =
            title_h +
            spacing +
            score_h +
            spacing +
            restart_h;

    int top_y = (screen_h - total_height) / 2;

    hud_render_text(
            renderer,
            hud->title_font,
            game_over_text,
            title_color,
            (screen_w - title_w) / 2,
            top_y
    );

    hud_render_text(
            renderer,
            hud->font,
            final_score_text,
            body_color,
            (screen_w - score_w) / 2,
            top_y + title_h + spacing
    );

    hud_render_text(
            renderer,
            hud->font,
            restart_text,
            body_color,
            (screen_w - restart_w) / 2,
            top_y + title_h + spacing + score_h + spacing
    );
}

void hud_render_start_screen(
        SDL_Renderer *renderer,
        const Hud *hud,
        int screen_w,
        int screen_h
) {
    if (!renderer) {
        SDL_Log("hud_render_start_screen: renderer is NULL");
        return;
    }

    if (!hud || !hud->font || !hud->title_font) {
        SDL_Log("hud_render_start_screen: hud/font/title_font is NULL");
        return;
    }

    const char *title_text = "TOUCHEZ";
    const char *subtitle_text = "POUR JOUER";

    SDL_Color title_color = {70, 220, 120, 255};
    SDL_Color subtitle_color = {255, 255, 255, 255};

    int title_w = 0;
    int title_h = 0;
    int subtitle_w = 0;
    int subtitle_h = 0;

    if (TTF_SizeUTF8(hud->title_font, title_text, &title_w, &title_h) != 0) {
        SDL_Log("TTF_SizeUTF8 failed for start title: %s", TTF_GetError());
        return;
    }

    if (TTF_SizeUTF8(hud->font, subtitle_text, &subtitle_w, &subtitle_h) != 0) {
        SDL_Log("TTF_SizeUTF8 failed for start subtitle: %s", TTF_GetError());
        return;
    }

    int spacing = 24;
    int total_height = title_h + spacing + subtitle_h;
    int top_y = (screen_h - total_height) / 2;

    hud_render_text(
            renderer,
            hud->title_font,
            title_text,
            title_color,
            (screen_w - title_w) / 2,
            top_y
    );

    hud_render_text(
            renderer,
            hud->font,
            subtitle_text,
            subtitle_color,
            (screen_w - subtitle_w) / 2,
            top_y + title_h + spacing
    );
}