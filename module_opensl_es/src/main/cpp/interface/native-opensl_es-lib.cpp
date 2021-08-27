//
// Created by Slash.R.Wei on 2021/8/25.
//

#include "native-opensl_es-lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

void Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativePlayPCM(JNIEnv *env, jobject thiz,
                                                                   jstring path) {
    //将Java传递进来的String转换成C++的string
    const std::string pcmPath = env->GetStringUTFChars(path, nullptr);

}

void Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativeStopPCM(JNIEnv *env, jobject thiz) {

}

#ifdef __cplusplus
}
#endif