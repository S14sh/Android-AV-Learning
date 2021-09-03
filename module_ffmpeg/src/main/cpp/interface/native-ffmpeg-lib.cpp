#include "native-ffmpeg-lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 获取ffmpeg版本号
 */
jstring Java_util_FfmpegUtil_getFfmpegVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(av_version_info());
}


#ifdef __cplusplus
}
#endif
