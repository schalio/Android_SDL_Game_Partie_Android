#ifndef GAME_TEXTURES_H
#define GAME_TEXTURES_H

#include <SDL.h>

typedef enum TextureId {
    TEX_TARGET = 0,
    TEX_GOLDEN_TARGET,
    TEX_PLAYER,
    TEX_PLAYER_FLASH,
    TEX_ENEMY,
    TEX_WALL,
    TEX_COUNT
} TextureId;

typedef struct {
    SDL_Texture* textures[TEX_COUNT];
} GameTextures;

// Initialiser les textures
int game_textures_init(GameTextures* textures);

// Charger les textures
int game_textures_load(GameTextures* textures, SDL_Renderer* renderer);

// Détruire les textures
void game_textures_destroy(GameTextures* textures);

// Fonction utilitaire pour charger une texture
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);

#endif // GAME_TEXTURES_H