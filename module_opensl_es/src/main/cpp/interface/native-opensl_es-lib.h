//
// Created by Slash.R.Wei on 2021/8/25.
//

#ifndef ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H

#include <jni.h>
#include <fstream>
#include "Opensl_es.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BUFFER_SIZE 2048

static Opensl_es *player = nullptr;

static std::ifstream pcmFile;

static bool isPlaying = false;

static void *playThreadFunc(void *arg);

JNIEXPORT void JNICALL
Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativePlayPCM(JNIEnv *env, jobject thiz,
                                                              jstring path);

JNIEXPORT void JNICALL
Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativeStopPCM(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H
