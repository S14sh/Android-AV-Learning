#ifndef ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <jni.h>

#include <libavutil/avutil.h>

#define NATIVE_CLASS "util/FfmpegUtil"
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

jstring Android_JNI_getFfmpegVersion(JNIEnv *env, jobject thiz);

static JNINativeMethod NATIVE_METHOD[] = {
        //native 函数-------签名-------对象的函数
        {"", "", (void *) Android_JNI_getFfmpegVersion}
};

/**
 * System.loadLibrary 会执行
 * @param javaVM
 * @param pVoid
 * @return
 */
jint JNI_OnLoad(JavaVM *javaVM, void *pVoid);

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_NATIVE_FFMPEG_LIB_H
