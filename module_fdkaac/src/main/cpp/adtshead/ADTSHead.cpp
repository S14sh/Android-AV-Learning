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
    data[2] |= ((value & 0x3) << 2);
}

void ADTSHead::setSamplingFrequencyIndex(unsigned int value) {
    data[2] |= ((value & 0xc) >> 2);
    data[3] |= ((value & 0x3) << 2);
}

