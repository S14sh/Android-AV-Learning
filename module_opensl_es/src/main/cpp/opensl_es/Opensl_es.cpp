//
// Created by Slash.R.Wei on 2021/8/25.
//

#include "Opensl_es.h"

void Opensl_es::createEngine() {
    //创建引擎对象：SLObjectItf mEngineObject
    SLresult result = slCreateEngine(&mEngineObject, 0, nullptr, 0, nullptr, nullptr);
    //创建失败
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine create failed: %d", result);
        return;
    }
    //初始化引擎
    result = (*mEngineObject)->Realize(mEngineObject, SL_BOOLEAN_FALSE);
    if (result != SL_BOOLEAN_FALSE) {
        LOGE("Engine realize failed: %d", result);
        return;
    }
    //获取引擎接口 SLEngineItf mEngineInterface
    result = (*mEngineObject)->GetInterface(mEngineObject, SL_IID_ENGINE, &mEngineInterface);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine GetInterface failed: %d", result);
        return;
    }
    //设置混音器
    //创建混音器 SLObjectItf mOutputMixObject
    result = (*mEngineInterface)->CreateOutputMix(mEngineInterface, &mOutputMixObject, 0, nullptr,
                                                  nullptr);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Engine CreateOutputMix failed: %d", result);
        return;
    }
    //初始化混音器
    result = (*mOutputMixObject)->Realize(mOutputMixObject, SL_BOOLEAN_FALSE);
    if (result != SL_BOOLEAN_FALSE) {
        LOGE("Engine mOutputMixObject Realize failed: %d", result);
        return;
    }
}

Opensl_es::Opensl_es() : mEngineObject(nullptr), mEngineInterface(nullptr),
                         mOutputMixObject(nullptr), mIndex(0), mBufferSize(0) {
    mMutex = PTHREAD_MUTEX_INITIALIZER;
    mBuffers[0] = nullptr;
    mBuffers[1] = nullptr;
    createEngine();
}

void Opensl_es::releaseEngine() {
    if (mOutputMixObject != nullptr) {
        (*mOutputMixObject)->Destroy(mOutputMixObject);
        mOutputMixObject = nullptr;
    }
    if (mEngineObject != nullptr) {
        (*mEngineObject)->Destroy(mEngineObject);
        mEngineObject = nullptr;
        mEngineInterface = nullptr;
    }
}

void Opensl_es::releasePlayer() {
    pthread_mutex_lock(&mMutex);
    if (mPlayerObject) {
        (*mPlayerObject)->Destroy(mPlayerObject);
        mPlayerObject = nullptr;
        mPlayer = nullptr;
        mBufferQueue = nullptr;
        mEffectSend = nullptr;
        mVolume = nullptr;
    }

    if (mBuffers[0] != nullptr) {
        delete[] mBuffers[0];
        mBuffers[0] = nullptr;
    }

    if (mBuffers[1] != nullptr) {
        delete[] mBuffers[1];
        mBuffers[1] = nullptr;
    }

    pthread_mutex_unlock(&mMutex);
    pthread_mutex_destroy(&mMutex);
}

Opensl_es::~Opensl_es() {
    releaseEngine();
    releasePlayer();
}

SLresult
Opensl_es::createPCMPlayer(SLuint32 formatType, SLuint32 numChannels, SLuint32 samplesPerSec,
                           SLuint32 bitsPerSample, SLuint32 containerSize,
                           SLuint32 channelMask, SLuint32 endianness, SLuint32 bufferNum) {
    //创建播放器
    //配置输入声音类型
    //创建2个buffer缓冲类型的队列
    SLDataLocator_AndroidSimpleBufferQueue buffers = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                      bufferNum};
    if (numChannels == 2) {
        channelMask = 0;
    }
    //PCM数据格式
    SLDataFormat_PCM formatPcm = {formatType, numChannels, samplesPerSec, bitsPerSample,
                                  containerSize, channelMask, endianness};

    /*
    * Enable Fast Audio when possible:  once we set the same rate to be the native, fast audio path
    * will be triggered
    */
    if (samplesPerSec) {
        formatPcm.samplesPerSec = samplesPerSec;
    }

    // 数据源 将上述配置信息放到这个数据源中
    SLDataSource audioSrc = {&buffers, &formatPcm};
    //配置音轨
    //设置混音器
    SLDataLocator_OutputMix locOutpuMix = {SL_DATALOCATOR_OUTPUTMIX, mOutputMixObject};
    SLDataSink audioSink = {&locOutpuMix, nullptr};
    /*
     * create audio mPlayer:
     *     fast audio does not support when SL_IID_EFFECTSEND is required, skip it
     *     for fast audio case
     */
    //  需要的接口 操作队列的接口
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_EFFECTSEND};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    //创建播放器

    SLresult result = (*mEngineInterface)->CreateAudioPlayer(mEngineInterface, &mPlayerObject,
                                                             &audioSrc, &audioSink,
                                                             samplesPerSec ? 2 : 3, ids, req);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer failed: %d", result);
        return result;
    }
    //初始化播放器
    result = (*mPlayerObject)->Realize(mPlayerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObject Realize failed: %d", result);
        return result;
    }
    //获取播放器接口 SLPlayItf mPlayer
    result = (*mPlayerObject)->GetInterface(mPlayerObject, SL_IID_PLAY, &mPlayer);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObject GetInterface failed: %d", result);
        return result;
    }
    //设置播放回调函数
    //获取播放器队列接口：SLAndroidSimpleBufferQueueItf mBufferQueue

    result = (*mPlayerObject)->GetInterface(mPlayerObject, SL_IID_BUFFERQUEUE, &mBufferQueue);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObject GetInterface failed: %d", result);
        return result;
    }
    //设置回调 void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
    result = (*mBufferQueue)->RegisterCallback(mBufferQueue, playerCallback, this);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mBufferQueue RegisterCallback failed: %d", result);
        return result;
    }
    mEffectSend = nullptr;
    if (samplesPerSec == 0) {
        result = (*mPlayerObject)->GetInterface(mPlayerObject, SL_IID_EFFECTSEND, &mEffectSend);
        if (result != SL_RESULT_SUCCESS) {
            LOGE("mPlayerObj GetInterface failed: %d", result);
            return false;
        }
    }

    result = (*mPlayerObject)->GetInterface(mPlayerObject, SL_IID_VOLUME, &mVolume);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj GetInterface failed: %d", result);
        return false;
    }

    //设置播放器状态为播放状态
    result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("mPlayerObj SetPlayState failed: %d", result);
        return result;
    }
    return SL_RESULT_SUCCESS;
}

SLresult Opensl_es::playPCM(const void * const data, const size_t &length) {
    // 每次播放一帧, 必须等待一帧音频播放完毕后才可以 Enqueue 第二帧音频
    pthread_mutex_lock(&mMutex);
    if (mBufferSize < length) {
        mBufferSize = length;
        if (mBuffers[0] != nullptr) {
            delete[] mBuffers[0];
        }
        if (mBuffers[1] != nullptr) {
            delete[] mBuffers[1];
        }
        mBuffers[0] = new uint8_t[mBufferSize];
        mBuffers[1] = new uint8_t[mBufferSize];
    }
    memcpy(mBuffers[mIndex], data, length);
    SLresult result = (*mBufferQueue)->Enqueue(mBufferQueue, mBuffers[mIndex], length);
    mIndex = 1 - mIndex;
    return result;
}

// 一帧音频播放完毕后就会回调这个函数
void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    Opensl_es *player = (Opensl_es *) context;
    pthread_mutex_unlock(&player->mMutex);
}