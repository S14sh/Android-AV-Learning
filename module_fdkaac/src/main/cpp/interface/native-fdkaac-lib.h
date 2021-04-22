#ifndef ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <jni.h>
#include <FDK_audio.h>

JNIEXPORT jstring JNICALL Java_util_FdkAacUtil_getFdkAacVersion(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
