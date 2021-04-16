#ifndef ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <jni.h>

#include <libavutil/avutil.h>

JNIEXPORT jstring JNICALL Java_util_FfmpegUtil_getFfmpegVersion(JNIEnv *env, jobject thiz);

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
