#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>

typedef enum SoundId {
    SOUND_COLLECT = 0,
    SOUND_HIT,
    SOUND_GOLDEN_COLLECT,
    SOUND_COUNT
} SoundId;

typedef struct AudioAssets {
    Mix_Chunk *chunks[SOUND_COUNT];
} AudioAssets;

void audio_assets_init(AudioAssets *audio);
int audio_assets_load(AudioAssets *audio);
void audio_assets_destroy(AudioAssets *audio);

void audio_play(const AudioAssets *audio, SoundId sound_id);

#endif