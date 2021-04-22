#ifndef ANDROID_AV_LEARNING_ADTSHEAD_H
#define ANDROID_AV_LEARNING_ADTSHEAD_H

#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif

//ADTS头默认长度为7字节
#define ADTS_HEADER_SIZE 7

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

};

#ifdef __cplusplus
}
#endif
#endif //ANDROID_AV_LEARNING_ADTSHEAD_H
