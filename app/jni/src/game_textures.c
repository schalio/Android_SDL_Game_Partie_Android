#include "game_textures.h"
#include <SDL_image.h>
#include <stdio.h>


int game_textures_init(GameTextures* textures) {
    memset(textures->textures, 0, sizeof(textures->textures));
    return 1;
}

int game_textures_load(GameTextures* textures, SDL_Renderer* renderer) {
    // Tableau des chemins de fichiers, dans le même ordre que TextureId
    static const char* paths[] = {
            "sprites/ruppy.png",        // TEX_TARGET
            "sprites/diamond.png", // TEX_GOLDEN_TARGET
            "sprites/player.png",        // TEX_PLAYER
            "sprites/player_flash.png",  // TEX_PLAYER_FLASH
            "sprites/enemy.png",         // TEX_ENEMY
            "sprites/planet.png",          // TEX_WALL
    };

    for (int i = 0; i < TEX_COUNT; i++) {
        textures->textures[i] = load_texture(renderer, paths[i]);
        if (!textures->textures[i]) {
            SDL_Log("Failed to load %s", paths[i]);
            return 0;
        }
    }

    return 1;
}

void game_textures_destroy(GameTextures* textures) {
    for (int i = 0; i < TEX_COUNT; i++) {
        if (textures->textures[i]) {
            SDL_DestroyTexture(textures->textures[i]);
            textures->textures[i] = NULL;
        }
    }
}



SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("IMG_Load failed: %s", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return NULL;
    }

    return texture;
}