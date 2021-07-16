#ifndef ANDROID_AV_LEARNING_AACDECODER_H
#define ANDROID_AV_LEARNING_AACDECODER_H

#include <aacdecoder_lib.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#define LOG_TAG "AACDecoder"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

class AACDecoder {
private:
    //AAC解码
    HANDLE_AACDECODER mDecoder;
    //pcm包的大小
    int mPkgSize = 0;
    unsigned char *mSpecInfo = nullptr;
    unsigned int mSpecInfoSize;
public:

    AACDecoder();

    virtual ~AACDecoder();

    int initWithRawFormat(unsigned char *specInfo, unsigned int specInfoSize);

    int initWithADTSFormat();

    int decode(unsigned char *pck, int len, unsigned char **outBuffer);

    void destroy();
private:

    void printAACInfo();

    int fdkDecoderAudio(INT_PCM *outBuffer, int *outSize, unsigned char *buffer, int size);

    void iniFrameSize();

};

#ifdef __cplusplus
}
#endif

#endif //ANDROID_AV_LEARNING_AACDECODER_H
