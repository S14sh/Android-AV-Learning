#ifndef ANDROID_AV_LEARNING_AACENCODER_H
#define ANDROID_AV_LEARNING_AACENCODER_H

#include <iostream>
#include <fstream>
#include <aacenc_lib.h>
#include <zconf.h>
#include <android/log.h>
#include <ADTSHead.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __ANDROID__
#define LOG_TAG "AACEncoder"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

/**
* 定义 AAC 编码格式
* LC-AAC:应用于中高码率场景的编码 （>= 80KBit/s）
* HE-AAC:主要应用于中低码率场景的编码 (<= 80KBit/s)
* HE-V2-AAC: 主要应用于低码率场景的编码 (<= 48KBit/s)
*/
typedef enum {
    LC_AAC = 2;
    HE_AAC = 5;
    LC_v2_AAC = 29;
} AACProfile;

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

class AACEncoder {
private:
    HANDLE_AACENCODER mAacEncoder;
    AACENC_InfoStruct mEncInfo;
    uint8_t *mInBuffer = nullptr;
    int mInBufferCursor;
    int mInputSizeFixed;
    //acc每帧采样1024个sample
    uint8_t mAacOutBuffer[1024 * 2 * 10];
    //设置编码Header
    bool isFlagGlobalHeader = false;
    std::ofstream outStream;
public:

    AACEncoder();

    virtual ~AACEncoder();

    /**
     * 初始化编码器
     * @param name 输出文件名，默认为fdkaac_encode
     * @param aacProfile profile参数，默认为LC_AAC
     * @param sampleRate 采样率，默认为44100kHz
     * @param channels 通道数，默认为单通道
     * @param bitRate 比特率，默认为128000
     * @param isOutputWithoutADTS 编码是否带ADTS头默认带ADTS头
     * @return 返回是否初始化成功
     */
    int init(const std::string &name = "fdkaac_encode", const AACProfile &aacProfile = LC_AAC,
             const int &sampleRate = 44100,
             const int &channels = 1,
             const int &bitRate = 128000, const bool &isOutputWithoutADTS = false);

    /**
     *
     * @param pData 原始数据(PCM)
     * @param dataByteSize 需要处理的字节数
     * @param outBuffer 输出数组
     * @return 返回编码成功的字节数
     */
    int encode(const Byte *const pData, int dataByteSize, char **outBuffer);

    void destory();

    /**
     * 使用FDKAAC进行编码
     * @return 返回编码成功的字节数
     */
    int fdkEncoderAudio();

    /**
     * 将ADTS头数据写入文件中
     * @param packetLen 后面的一帧AAC裸流长度
     * @param profile profile参数 0-Main profile, 1-AAC LC，2-SSR
     * @param freqIdx 采样率下标
     *              0: 96000 Hz
                    1: 88200 Hz
                    2: 64000 Hz
                    3: 48000 Hz
                    4: 44100 Hz
                    5: 32000 Hz
                    6: 24000 Hz
                    7: 22050 Hz
                    8: 16000 Hz
                    9: 12000 Hz
                    10: 11025 Hz
                    11: 8000 Hz
                    12: 7350 Hz
                    13: Reserved
                    14: Reserved
                    15: frequency is written explictly
     * @param chanCfg 通道数 2：L+R 3：C+L+R
     */
    void addADTS2Packet(const int &packetLen, const int &profile = 1, const int &freqIdx = 4,
                        const int &chanCfg = 1);

    /**
     * 将AAC数据写入文件
     * @param data AAC裸流
     * @param datalen AAC裸流数据字节数
     */
    void writeAACPacket2File(const uint8_t *const data, const int &datalen);

};

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_AACENCODER_H
