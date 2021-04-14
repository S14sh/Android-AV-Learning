#include "native-ffmpeg-lib.h"

/**
 * 获取ffmpeg版本号
 */
extern "C"
jstring Java_util_FfmpegUtil_getFfmpegVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(av_version_info());
}