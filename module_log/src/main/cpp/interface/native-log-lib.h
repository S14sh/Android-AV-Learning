//
// Created by Slash.R.Wei on 2021/8/23.
//

#ifndef ANDROID_AV_LEARNING_NATIVE_LOG_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_LOG_LIB_H

#include <jni.h>
#include <android/log.h>
#include <sys/types.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#define LOG_TAG "NativeCatcher"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

JNIEXPORT void JNICALL
Java_com_android_module_1log_util_NativeCatcherUtil_catchNativeException(JNIEnv *env, jobject thiz,
                                                                         jobject listener);

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_NATIVE_LOG_LIB_H
