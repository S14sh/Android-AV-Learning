//
// Created by Slash.R.Wei on 2021/8/25.
//

#ifndef ANDROID_AV_LEARNING_OPENSL_ES_H
#define ANDROID_AV_LEARNING_OPENSL_ES_H

// 这是标准的OpenSL ES库
#include <SLES/OpenSLES.h>
// 这里是针对安卓的扩展，如果要垮平台则需要注意
#include <SLES/OpenSLES_Android.h>
#include <android/log.h>
#include <pthread.h>
#include <sys/types.h>
#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#define LOG_TAG "AACEncoder"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#define BUFFERS_SIZE 2

void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

class Opensl_es {
private:
    /**
     *引擎对象
     */
    SLObjectItf mEngineObject;
    /**
     * 引擎接口
     */
    SLEngineItf mEngineInterface;
    /**
     * 混音器
     */
    SLObjectItf mOutputMixObject;
    /**
     * 播放缓冲队列
     */
    uint8_t *mBuffers[BUFFERS_SIZE];
    SLuint32 mBufferSize;

    int mIndex;

    /**
     * 播放器
     */
    SLObjectItf mPlayerObject;

    /**
     * 播放器接口
     */
    SLPlayItf mPlayer;

    SLAndroidSimpleBufferQueueItf mBufferQueue;

    SLEffectSendItf mEffectSend;

    SLVolumeItf mVolume;

    pthread_mutex_t mMutex;

    void createEngine();

    void releaseEngine();

    void releasePlayer();

public:

    Opensl_es();

    virtual ~Opensl_es();

    /**
     * 创建PCM播放器
     * @param formatType 数据格式
     * @param numChannels 声道
     * @param samplesPerSec 采样率
     * @param bitsPerSample 采样格式
     * @param containerSize 数据大小
     * @param channelMask 左右声道或别的效果
     * @param endianness 大/小端模式
     * @param bufferNum 缓冲队列数量
     */
    SLresult createPCMPlayer(SLuint32 formatType = SL_DATAFORMAT_PCM, SLuint32 numChannels = 2,
                             SLuint32 samplesPerSec = SL_SAMPLINGRATE_44_1,
                             SLuint32 bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16,
                             SLuint32 containerSize = SL_PCMSAMPLEFORMAT_FIXED_16,
                             SLuint32 channelMask = SL_SPEAKER_FRONT_CENTER,
                             SLuint32 endianness = SL_BYTEORDER_LITTLEENDIAN,
                             SLuint32 bufferNum = 2);

    SLresult playPCM(const void * const data, const size_t &length);

    friend void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

};

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_OPENSL_ES_H
