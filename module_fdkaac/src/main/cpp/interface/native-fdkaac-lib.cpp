#include "native-fdkaac-lib.h"
#include <string>
#ifdef __cplusplus
extern "C" {
#endif

jstring Java_util_FdkAacUtil_getFdkAacVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(std::to_string(LIB_VERSION(1, 2, 3)).c_str());
}

#ifdef __cplusplus
}
#endif
