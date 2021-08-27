//
// Created by Slash.R.Wei on 2021/8/25.
//

#ifndef ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H

#include <jni.h>
#include <string>
#include "Opensl_es.h"

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT void JNICALL
Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativePlayPCM(JNIEnv *env, jobject thiz,
                                                              jstring path);

JNIEXPORT void JNICALL
Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativeStopPCM(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_NATIVE_OPENSL_ES_LIB_H
