#ifndef ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
#define ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <jni.h>
#include <FDK_audio.h>

JNIEXPORT jstring JNICALL Java_util_FdkAacUtil_getFdkAacVersion(JNIEnv *env, jobject thiz);


/**
 * 调用System.loadLibrary时会执行，动态注册
 * @param javaVm
 * @param pVoid
 * @return
 */
jint JNI_OnLoad(JavaVM *javaVm, void *pVoid){
    JNIEnv *jniEnv;
    if(javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_NATIVE_FDKAAC_LIB_H
