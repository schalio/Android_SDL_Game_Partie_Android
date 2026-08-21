#include <SDL.h>
#include <SDL_system.h>
#include <jni.h>

#include "android_utils.h"

void android_vibrate_hit(void) {
#if defined(__ANDROID__)
    JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
    if (!env) {
        SDL_Log("android_vibrate_hit: SDL_AndroidGetJNIEnv failed");
        return;
    }

    jobject activity = (jobject) SDL_AndroidGetActivity();
    if (!activity) {
        SDL_Log("android_vibrate_hit: SDL_AndroidGetActivity failed");
        return;
    }

    jclass activity_class = (*env)->GetObjectClass(env, activity);
    if (!activity_class) {
        SDL_Log("android_vibrate_hit: GetObjectClass failed");
        (*env)->DeleteLocalRef(env, activity);
        return;
    }

    jmethodID method_id = (*env)->GetMethodID(env, activity_class, "vibrateHit", "()V");
    if (!method_id) {
        SDL_Log("android_vibrate_hit: GetMethodID failed for vibrateHit");
        (*env)->DeleteLocalRef(env, activity_class);
        (*env)->DeleteLocalRef(env, activity);
        return;
    }

    (*env)->CallVoidMethod(env, activity, method_id);

    if ((*env)->ExceptionCheck(env)) {
        SDL_Log("android_vibrate_hit: Java exception during CallVoidMethod");
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
    }

    (*env)->DeleteLocalRef(env, activity_class);
    (*env)->DeleteLocalRef(env, activity);
#endif
}
