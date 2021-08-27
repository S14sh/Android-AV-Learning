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


jint JNI_OnLoad(JavaVM *javaVM, void *pVoid) {
    JNIEnv *jniEnv;
    if (javaVM->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass nativeClass = jniEnv->FindClass(NATIVE_CLASS);
    jniEnv->RegisterNatives(nativeClass, NATIVE_METHOD, NELEM(NATIVE_METHOD));
    jniEnv->DeleteGlobalRef(nativeClass);

    return JNI_VERSION_1_6;
}

#ifdef __cplusplus
}
#endif
