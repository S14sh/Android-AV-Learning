#ifndef ANDROID_AV_LEARNING_ADTSHEAD_H
#define ANDROID_AV_LEARNING_ADTSHEAD_H

#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif

//ADTS头默认长度为7字节
#define ADTS_HEADER_SIZE 7


//固定头基本都是一样的信息，可变头每帧中的各信息可能不同
class ADTSHead {
private:
    uint8_t *data;
public:

    ADTSHead();

    ADTSHead(uint8_t data[ADTS_HEADER_SIZE]);

    virtual ~ADTSHead();

    const uint8_t *const getData();

    //关键字SyncWord用于识别帧开头，长度为12bit(0xfff)
    void setSyncWord(unsigned int value = 0xfff);

    //关键字ID，MPEG Version: 0 for MPEG-4, 1 for MPEG-2,长度为1bit
    void setID(unsigned int value = 0);

    //关键字layer,长度2bit,总是00
    void setLayer(unsigned int value = 0);

    //关键字protection_absent,长度1bit(set to 1 if there is no CRC(循环冗余校验) and 0 if there is CRC)
    void setProtectionAbsent(unsigned int value = 1);

    //关键字profile,长度2bit,表示使用哪个级别的AAC
    void setProfile(unsigned int value = 0x3);

    //关键字sampling_frequency_index,长度为4bit,表示采样率的下标
    /*
                    0: 96000 Hz
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
     */
    void setSamplingFrequencyIndex(unsigned int value = 0x4);

    //关键字private_bit，长度1bit，私有位，编码时设置为0，解码时忽略
    void setPrivateBit(unsigned int value = 0x1);

    //关键字channel_configuration,长度3bit,表示声道数
    /*
        0: Defined in AOT Specifc Config
        1: 1 channel: front-center
        2: 2 channels: front-left, front-right
        3: 3 channels: front-center, front-left, front-right
        4: 4 channels: front-center, front-left, front-right, back-center
        5: 5 channels: front-center, front-left, front-right, back-left, back-right
        6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
        7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
        8-15: Reserved
     */
    void setChannelConfiguration(unsigned int value = 0x1);

    //关键字original_copy,长度1bit,编码时设置为0,解码时忽略
    void setOriginalCopy(unsigned int value = 0);

    //关键字home,长度1bit,编码时设置为0,解码时忽略
    void setHome(unsigned int value = 0);

    //关键字copyright_identification_bit,长度1bit,编码时设置为0,解码时忽略
    void setCopyrightIdentificationBit(unsigned int value = 0);

    //关键字copyright_identification_start,长度1bit,编码时设置为0,解码时忽略
    void setCopyrightIdentificationStart(unsigned int value = 0);

    //关键字aac_frame_length,长度13bit,一个ADTS帧的长度(包括ADTS头和AAC原始流)
    void setAacFrameLength(unsigned int value = 0);

    //关键字adts_buffer_fullness,长度11bit,默认值0x7FF说明是码率可变的码流
    void setAdtsBufferFullness(unsigned int value = 0x7ff);

    //关键字number_of_raw_data_blocks_in_frame,长度2bit,表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.
    //所以说number_of_raw_data_blocks_in_frame == 0 表示说ADTS帧中有一个AAC数据块。
    //(一个AAC原始帧包含一段时间内1024个采样及相关数据)
    void setNumberOfRawDataBlocksInFrame(unsigned int value = 0);

};

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_ADTSHEAD_H
