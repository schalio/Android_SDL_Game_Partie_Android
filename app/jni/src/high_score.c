#include "high_score.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

void save_high_score(int score) {
    char* path = SDL_GetPrefPath(NULL, "org.libsdl.app");  // adapte le nom
    if (!path) {
        SDL_Log("SDL_GetPrefPath failed: %s", SDL_GetError());
        return;
    }

    char filename[512];
    SDL_snprintf(filename, sizeof(filename), "%shighscore.txt", path);

    FILE* f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%d\n", score);
        fclose(f);
        SDL_Log("High score saved: %d", score);
    } else {
        SDL_Log("Failed to save high score");
    }

    SDL_free(path);
}

int load_high_score(void) {
    char* path = SDL_GetPrefPath(NULL, "org.libsdl.app");
    if (!path) {
        SDL_Log("SDL_GetPrefPath failed: %s", SDL_GetError());
        return 0;
    }

    char filename[512];
    SDL_snprintf(filename, sizeof(filename), "%shighscore.txt", path);

    FILE* f = fopen(filename, "r");
    int score = 0;

    if (f) {
        char line[64];
        if (fgets(line, sizeof(line), f) != NULL) {
            char* endptr;
            errno = 0;
            long val = strtol(line, &endptr, 10);

            if (errno == 0 && endptr != line && val >= 0 && val <= INT_MAX) {
                score = (int)val;
            } else {
                SDL_Log("Invalid high score value, using 0");
                score = 0;
            }
        }
        fclose(f);
        SDL_Log("High score loaded: %d", score);
    } else {
        SDL_Log("No high score file found, using 0");
    }

    SDL_free(path);
    return score;
}