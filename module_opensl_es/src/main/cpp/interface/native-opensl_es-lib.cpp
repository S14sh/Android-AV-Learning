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
    int size = 0;
    while (isPlaying && !pcmFile.eof()) {
        for (int i = 0; i < BUFFER_SIZE && !pcmFile.eof(); ++i) {
            pcmFile >> buffer[i];
            ++size;
        }
        if (player != nullptr) {
            player->playPCM(buffer, size);
        }
        size = 0;
    }
    return nullptr;
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

    if (pcmFile.fail()) {
        LOGE("open file failed!!!");
        return;
    }

    isPlaying = true;

    pthread_t playThread;
    //在子线程中执行

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