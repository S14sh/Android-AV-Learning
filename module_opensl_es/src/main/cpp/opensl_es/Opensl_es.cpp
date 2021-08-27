//
// Created by Slash.R.Wei on 2021/8/25.
//
#include "Opensl_es.h"

Opensl_es::Opensl_es() {
    //创建引擎对象：SLObjectItf engineObject
    SLresult result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    //创建失败
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine create failed: %d", result);
        return;
    }
    //初始化引擎
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_BOOLEAN_FALSE) {
        LOGE("Engine realize failed: %d", result);
        return;
    }
    //获取引擎接口 SLEngineItf engineInterface
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine GetInterface failed: %d", result);
        return;
    }
    //设置混音器
    //创建混音器 SLObjectItf outputMixObject
    result = (*engineInterface)->CreateOutputMix(engineInterface, &outputMixObject, 0, nullptr,
                                                 nullptr);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine CreateOutputMix failed: %d", result);
        return;
    }
    //初始化混音器
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (result != SL_BOOLEAN_FALSE) {
        LOGE("Engine outputMixObject Realize failed: %d", result);
        return;
    }
}

Opensl_es::~Opensl_es() {

}

SLresult
Opensl_es::createPCMPlayer(SLuint32 formatType, SLuint32 numChannels, SLuint32 samplesPerSec,
                           SLuint32 bitsPerSample, SLuint32 containerSize,
                           SLuint32 channelMask, SLuint32 endianness, SLuint32 bufferNum) {
    //创建播放器
    //配置输入声音类型
    //创建2个buffer缓冲类型的队列
    buffers = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, bufferNum};
    //PCM数据格式
    SLDataFormat_PCM formatPcm = {formatType, numChannels, samplesPerSec, bitsPerSample,
                                  containerSize, channelMask,
                                  endianness};

    /*
    * Enable Fast Audio when possible:  once we set the same rate to be the native, fast audio path
    * will be triggered
    */
    if (!samplesPerSec) {
        formatPcm.samplesPerSec = samplesPerSec;
    }

    // 数据源 将上述配置信息放到这个数据源中
    SLDataSource audioSrc = {&buffers, &formatPcm};
    //配置音轨
    //设置混音器
    SLDataLocator_OutputMix locOutpuMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&locOutpuMix, nullptr};
    /*
     * create audio player:
     *     fast audio does not support when SL_IID_EFFECTSEND is required, skip it
     *     for fast audio case
     */
    //  需要的接口 操作队列的接口
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_EFFECTSEND};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    //创建播放器
    SLObjectItf playerObject;
    SLresult result = (*engineInterface)->CreateAudioPlayer(engineInterface, &playerObject,
                                                            &audioSrc, &audioSink,
                                                            samplesPerSec ? 2 : 3, ids, req);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer failed: %d", result);
        return result;
    }
    //初始化播放器
    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("playerObject Realize failed: %d", result);
        return result;
    }
    //获取播放器接口 SLPlayItf player
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &player);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("playerObject GetInterface failed: %d", result);
        return result;
    }
    //设置播放回调函数
    //获取播放器队列接口：SLAndroidSimpleBufferQueueItf mBufferQueue
    SLAndroidSimpleBufferQueueItf bufferQueue;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &bufferQueue);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("playerObject GetInterface failed: %d", result);
        return result;
    }
    //设置回调 void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
    slAndroidSimpleBufferQueueCallback playerCallback;
    result = (*bufferQueue)->RegisterCallback(bufferQueue, playerCallback, this);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("bufferQueue RegisterCallback failed: %d", result);
        return result;
    }
    SLEffectSendItf effectSend = nullptr;
    if (samplesPerSec == 0) {
        result = (*playerObject)->GetInterface(playerObject, SL_IID_EFFECTSEND, &effectSend);
        if (result != SL_RESULT_SUCCESS) {
            LOGE("mPlayerObj GetInterface failed: %d", result);
            return false;
        }
    }
    SLVolumeItf volume = nullptr;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_VOLUME, &volume);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj GetInterface failed: %d", result);
        return false;
    }
}

SLresult Opensl_es::playPCM() {
    //设置播放器状态为播放状态
    SLresult result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj SetPlayState failed: %d", result);
        return false;
    }
}