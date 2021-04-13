#ifndef ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H

#include <jni.h>

extern "C" {
#include <libavutil/avutil.h>
}

const char *getFFmpegVer();

extern "C"
JNIEXPORT jstring JNICALL
Java_util_FfmpegUtil_getFfmpegVersion(JNIEnv *env, jobject thiz);

#endif //ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
