#include "native-fdkaac-lib.h"
#include <string>
#ifdef __cplusplus
extern "C" {
#endif

jstring Java_util_FdkAacUtil_getFdkAacVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(std::to_string(LIB_VERSION(1, 2, 3)).c_str());
}


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
