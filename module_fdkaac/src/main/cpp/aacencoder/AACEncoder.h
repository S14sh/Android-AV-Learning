#ifndef ANDROID_AV_LEARNING_AACENCODER_H
#define ANDROID_AV_LEARNING_AACENCODER_H

#include <iostream>
#include <fstream>
#include <aacenc_lib.h>
#include <zconf.h>

#ifdef __ANDROID__
#define LOG_TAG "AACEncoder"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

const char *fdkaac_error(AACENC_ERROR erraac) {
    switch (erraac) {
        case AACENC_OK:
            return "No error";
        case AACENC_INVALID_HANDLE:
            return "Invalid handle";
        case AACENC_MEMORY_ERROR:
            return "Memory allocation error";
        case AACENC_UNSUPPORTED_PARAMETER:
            return "Unsupported parameter";
        case AACENC_INVALID_CONFIG:
            return "Invalid config";
        case AACENC_INIT_ERROR:
            return "Initialization error";
        case AACENC_INIT_AAC_ERROR:
            return "AAC library initialization error";
        case AACENC_INIT_SBR_ERROR:
            return "SBR library initialization error";
        case AACENC_INIT_TP_ERROR:
            return "Transport library initialization error";
        case AACENC_INIT_META_ERROR:
            return "Metadata library initialization error";
        case AACENC_ENCODE_ERROR:
            return "Encoding error";
        case AACENC_ENCODE_EOF:
            return "End of file";
        default:
            return "Unknown error";
    }
}

/**
 * 定义 AAC 编码格式
 * LC-AAC:应用于中高码率场景的编码 （>= 80Kbit/s）
 * HE-AAC:主要应用于中低码率场景的编码 (<= 80kit/s)
 * HE-V2-AAC: 主要应用于低码率场景的编码 (<= 48Kbit/s)
 */
typedef enum {
    LC_AAC = 2;
    HE_AAC = 5;
    LC_v2_AAC = 29;
} AACProfile;

class AACEncoder {
private:
    HANDLE_AACENCODER mAacEncoder;
    AACENC_InfoStruct mEncInfo;
    uint8_t *mInBuffer = nullptr;
    int mInBufferCursor;
    int mInputSizeFixed;
    uint8_t mAacOutBuffer[1024 * 2 * 10];
    //设置编码Header
    bool isFlagGlobalHeader = false;
    std::ofstream outStream;
public:
    AACEncoder();

    virtual ~AACEncoder();

    int init(AACProfile aacProfile, int sampleRate, int channels, int bitRate);

    int encode(Byte *pData, int dataByteSize, char **outBuffer);

    void destory();

    int fdkEncoderAudio();

    void addADTS2Packet(uint8_t *packet, int packetLen);

    void writeAACPacket2File(uint8_t *data, int datalen);

};

#endif //ANDROID_AV_LEARNING_AACENCODER_H
