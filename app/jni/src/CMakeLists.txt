cmake_minimum_required(VERSION 3.6)

project(MY_APP)

add_library(rustcore STATIC IMPORTED)
set_target_properties(rustcore PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_CURRENT_LIST_DIR}/../../src/main/jniLibs/${ANDROID_ABI}/librustcore.a
)

add_library(main SHARED
        bridge.c
        audio.c
        android_utils.c
        hud.c
        game_textures.c
)

target_sources(main PRIVATE bridge.c)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../SDL/include
        ${CMAKE_CURRENT_LIST_DIR}/../SDL_mixer
        ${CMAKE_CURRENT_LIST_DIR}/../SDL_ttf
)

target_link_libraries(main
        SDL2
        SDL2_mixer
        SDL2_ttf
        SDL2_image
        rustcore
        log
)