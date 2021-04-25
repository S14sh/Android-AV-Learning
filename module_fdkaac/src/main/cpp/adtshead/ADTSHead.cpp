#include "ADTSHead.h"

ADTSHead::ADTSHead() {
    //开辟空间
    data = new uint8_t[ADTS_HEADER_SIZE];
    //初始化
    memset(data, 0, sizeof(data));
    //使用默认值初始化ADTS头
    setSyncWord();
    setID();
    setLayer();
    setProtectionAbsent();
    setProfile();
    setSamplingFrequencyIndex();
    setPrivateBit();
    setChannelConfiguration();
    setOriginalCopy();
    setHome();
    setCopyrightIdentificationBit();
    setCopyrightIdentificationStart();
    setAacFrameLength();
    setAdtsBufferFullness();
    setNumberOfRawDataBlocksInFrame();
}

ADTSHead::ADTSHead(uint8_t data[ADTS_HEADER_SIZE]) {
    this->data = data;
}

ADTSHead::~ADTSHead() {
    delete[] data;
}

const uint8_t *const ADTSHead::getData() {
    return data;
}

void ADTSHead::setSyncWord(unsigned int value) {
    data[0] = ((value >> 4) & 0xff);
    data[1] |= ((value & 0xf) << 4);
}

void ADTSHead::setID(unsigned int value) {
    data[1] |= ((value & 1) << 3);
}

void ADTSHead::setLayer(unsigned int value) {
    data[1] |= ((value & 0x3) << 1);
}

void ADTSHead::setProtectionAbsent(unsigned int value) {
    data[1] |= (value & 1);
}

void ADTSHead::setProfile(unsigned int value) {
    data[2] |= ((value & 0x3) << 6);
}

void ADTSHead::setSamplingFrequencyIndex(unsigned int value) {
    data[2] |= ((value & 0xc) << 4);
    data[2] |= ((value & 0x3) << 2);
}

void ADTSHead::setPrivateBit(unsigned int value) {
    data[2] |= ((value & 1) << 1);
}

void ADTSHead::setChannelConfiguration(unsigned int value) {
    data[2] |= ((value & 0x4) >> 2);
    data[3] |= ((value & 0x3) << 6);
}

void ADTSHead::setOriginalCopy(unsigned int value) {
    data[3] |= ((value & 1) << 5);
}

void ADTSHead::setHome(unsigned int value) {
    data[3] |= ((value & 1) << 4);
}

void ADTSHead::setCopyrightIdentificationBit(unsigned int value) {
    data[3] |= ((value & 1) << 3);
}

void ADTSHead::setCopyrightIdentificationStart(unsigned int value) {
    data[3] |= ((value & 1) << 2);
}

void ADTSHead::setAacFrameLength(unsigned int value) {
    data[3] |= ((value >> 11) & 0x3);
    data[4] = (value & 0x7f8);
    data[5] |= ((value & 0x7) << 5);
}

void ADTSHead::setAdtsBufferFullness(unsigned int value) {
    data[5] |= ((value >> 6) & 0x1f);
    data[6] |= ((value & 0x3f) << 2);
}

void ADTSHead::setNumberOfRawDataBlocksInFrame(unsigned int value) {
    data[6] |= (value & 0x3);
}


