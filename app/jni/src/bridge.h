#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOGICAL_W 720
#define LOGICAL_H 1280

typedef struct AppState AppState;

typedef struct Rect {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
} Rect;

typedef struct SceneData {
    Rect player;
    Rect target;
    Rect golden_target;
    int32_t golden_target_active;
    int32_t enemy_count;
    Rect enemy1;
    Rect enemy2;
    Rect enemy3;
    Rect wall;
    int32_t score;
    int32_t level;
    int32_t lives;
    int32_t game_over;
    int32_t game_started;
    int32_t player_is_flashing;
} SceneData;

extern AppState* rust_app_create(void);
extern void rust_app_set_screen_size(AppState* app, int32_t w, int32_t h);
extern void rust_app_on_touch(AppState* app, float x, float y);
extern int32_t rust_app_update(AppState* app, float dt);
extern int32_t rust_app_get_scene(const AppState* app, SceneData* out_scene);
extern void rust_app_restart(AppState* app);
extern void rust_app_destroy(AppState* app);

#ifdef __cplusplus
}
#endif

#endif