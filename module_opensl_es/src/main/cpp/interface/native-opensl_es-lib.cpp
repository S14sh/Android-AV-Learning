//
// Created by Slash.R.Wei on 2021/8/25.
//

#include "native-opensl_es-lib.h"

#ifdef __cplusplus
extern "C"
{
#endif

static void *playThreadFunc(void *arg) {
    short buffer[BUFFER_SIZE];
    while (isPlaying && !pcmFile.eof()) {
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            pcmFile >> buffer[i];
        }
        if (player != nullptr) {
            player->playPCM(buffer, BUFFER_SIZE);
        }
    }
    return 0;
}

void Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativePlayPCM(JNIEnv *env, jobject thiz,
                                                                   jstring path) {
    //将Java传递进来的String转换成C++的string
    const char *pcmPath = env->GetStringUTFChars(path, nullptr);
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    //如果已经实例化了，就先释放资源
    if (player != nullptr) {
        delete player;
    }
    player = new Opensl_es();
    player->createPCMPlayer();

    //如果之前打开过文件
    if (pcmFile.is_open()) {
        pcmFile.close();
    }
    pcmFile.open(pcmPath);

    isPlaying = true;
    pthread_t playThread;
    pthread_create(&playThread, nullptr, playThreadFunc, 0);

    env->ReleaseStringUTFChars(path, pcmPath);
}

void Java_com_module_1opensl_1es_util_Opensl_1esUtil_nativeStopPCM(JNIEnv *env, jobject thiz) {
    isPlaying = false;
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    if (pcmFile.is_open()) {
        pcmFile.close();
    }
}

#ifdef __cplusplus
}
#endif