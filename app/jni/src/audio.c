#include <SDL.h>
#include <SDL_mixer.h>

#include "audio.h"

static Mix_Chunk* load_sound(const char *path) {
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if (!sound) {
        SDL_Log("Mix_LoadWAV failed for %s: %s", path, Mix_GetError());
    }
    return sound;
}

static void play_sound(Mix_Chunk *sound, const char *debug_name) {
    if (!sound) {
        SDL_Log("play_sound skipped: %s is NULL", debug_name);
        return;
    }

    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        SDL_Log("Mix_PlayChannel failed for %s: %s", debug_name, Mix_GetError());
    }
}

void audio_assets_init(AudioAssets *audio) {
    for (int i = 0; i < SOUND_COUNT; ++i) {
        audio->chunks[i] = NULL;
    }
}

int audio_assets_load(AudioAssets *audio) {
    static const char *sound_paths[SOUND_COUNT] = {
            [SOUND_COLLECT] = "sounds/collect.wav",
            [SOUND_HIT] = "sounds/hit.wav"
    };

    static const int sound_volumes[SOUND_COUNT] = {
            [SOUND_COLLECT] = 64,
            [SOUND_HIT] = 96
    };

    for (int i = 0; i < SOUND_COUNT; ++i) {
        audio->chunks[i] = load_sound(sound_paths[i]);
        if (!audio->chunks[i]) {
            audio_assets_destroy(audio);
            return 0;
        }

        Mix_VolumeChunk(audio->chunks[i], sound_volumes[i]);
    }

    return 1;
}

void audio_assets_destroy(AudioAssets *audio) {
    for (int i = 0; i < SOUND_COUNT; ++i) {
        if (audio->chunks[i]) {
            Mix_FreeChunk(audio->chunks[i]);
            audio->chunks[i] = NULL;
        }
    }
}

void audio_play(const AudioAssets *audio, SoundId sound_id) {
    static const char *sound_names[SOUND_COUNT] = {
            [SOUND_COLLECT] = "SOUND_COLLECT",
            [SOUND_HIT] = "SOUND_HIT"
    };

    if (sound_id < 0 || sound_id >= SOUND_COUNT) {
        SDL_Log("audio_play: invalid sound id %d", (int)sound_id);
        return;
    }

    play_sound(audio->chunks[sound_id], sound_names[sound_id]);
}