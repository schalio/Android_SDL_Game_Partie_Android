#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_system.h>
#include <stdio.h>

#include "bridge.h"
#include "audio.h"
#include "android_utils.h"
#include "hud.h"

int SDL_main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) != 0) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Hud hud;
    hud_init(&hud);

    if (!hud_load(&hud)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    AudioAssets audio;
    audio_assets_init(&audio);

    if (!audio_assets_load(&audio)) {
        hud_destroy(&hud);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int screen_w = 0;
    int screen_h = 0;
    SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

    AppState* app = rust_app_create();
    if (!app) {
        SDL_Log("rust_app_create failed");
        audio_assets_destroy(&audio);
        hud_destroy(&hud);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    rust_app_set_screen_size(app, screen_w, screen_h);

    int running = 1;
    int game_over = 0;

    uint32_t previous = SDL_GetTicks();
    int32_t last_score = -1;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                case SDL_APP_TERMINATING:
                    running = 0;
                    break;

                case SDL_FINGERDOWN: {
                    if (game_over) {
                        rust_app_restart(app);
                        game_over = 0;
                        last_score = -1;
                        SDL_Log("Game restarted");
                    } else {
                        float px = event.tfinger.x * (float)screen_w;
                        float py = event.tfinger.y * (float)screen_h;
                        rust_app_on_touch(app, px, py);
                    }
                    break;
                }

                case SDL_FINGERMOTION: {
                    if (!game_over) {
                        float px = event.tfinger.x * (float) screen_w;
                        float py = event.tfinger.y * (float) screen_h;
                        rust_app_on_touch(app, px, py);
                    }
                    break;
                }

                default:
                    break;
            }
        }

        uint32_t now = SDL_GetTicks();
        uint32_t elapsed_ms = now - previous;
        previous = now;
        float dt = (float)elapsed_ms / 1000.0f;

        int32_t result = rust_app_update(app, dt);

        SceneData scene;
        if (!rust_app_get_scene(app, &scene)) {
            continue;
        }

        game_over = scene.game_over;

        if (scene.score != last_score) {
            SDL_Log("Score: %d", scene.score);
            last_score = scene.score;
        }

        if (result == 1) {
            SDL_Log("Target collected!");
            audio_play(&audio, SOUND_COLLECT);
        } else if (result == -1) {
            SDL_Log("Enemy hit!");
            audio_play(&audio, SOUND_HIT);
            android_vibrate_hit();
        }

//  Partie gérant l'affichage

        SDL_SetRenderDrawColor(renderer, 18, 18, 28, 255);
        SDL_RenderClear(renderer);

        SDL_Rect wall_rect = {
                scene.wall.x, scene.wall.y, scene.wall.w, scene.wall.h
        };
        SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
        SDL_RenderFillRect(renderer, &wall_rect);

        SDL_Rect target_rect = {
                scene.target.x, scene.target.y, scene.target.w, scene.target.h
        };
        SDL_SetRenderDrawColor(renderer, 70, 220, 120, 255);
        SDL_RenderFillRect(renderer, &target_rect);

        SDL_Rect enemy_rect = {
                scene.enemy.x, scene.enemy.y, scene.enemy.w, scene.enemy.h
        };
        SDL_SetRenderDrawColor(renderer, 220, 70, 70, 255);
        SDL_RenderFillRect(renderer, &enemy_rect);

        SDL_Rect player_rect = {
                scene.player.x, scene.player.y, scene.player.w, scene.player.h
        };

        if (scene.player_is_flashing) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 80, 190, 255, 255);
        }

        SDL_RenderFillRect(renderer, &player_rect);

        hud_render_score(renderer, &hud, scene.score);
        hud_render_lives(renderer, &hud, scene.lives);

        if (scene.game_over) {
            hud_render_game_over(
                    renderer,
                    &hud,
                    screen_w,
                    screen_h,
                    scene.score
            );
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    rust_app_destroy(app);

    audio_assets_destroy(&audio);
    hud_destroy(&hud);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}