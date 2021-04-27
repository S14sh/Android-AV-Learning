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
    delete[] this->data;
}

const uint8_t *const ADTSHead::getData() {
    return this->data;
}

void ADTSHead::setSyncWord(const unsigned int &value) {
    this->data[0] = ((value >> 4) & 0xff);
    this->data[1] |= ((value & 0xf) << 4);
}

void ADTSHead::setID(const unsigned int &value) {
    this->data[1] |= ((value & 1) << 3);
}

void ADTSHead::setLayer(const unsigned int &value) {
    this->data[1] |= ((value & 0x3) << 1);
}

void ADTSHead::setProtectionAbsent(const unsigned int &value) {
    this->data[1] |= (value & 1);
}

void ADTSHead::setProfile(const unsigned int &value) {
    this->data[2] |= ((value & 0x3) << 6);
}

void ADTSHead::setSamplingFrequencyIndex(const unsigned int &value) {
    this->data[2] |= ((value & 0xc) << 4);
    this->data[2] |= ((value & 0x3) << 2);
}

void ADTSHead::setPrivateBit(const unsigned int &value) {
    this->data[2] |= ((value & 1) << 1);
}

void ADTSHead::setChannelConfiguration(const unsigned int &value) {
    this->data[2] |= ((value & 0x4) >> 2);
    this->data[3] |= ((value & 0x3) << 6);
}

void ADTSHead::setOriginalCopy(const unsigned int &value) {
    this->data[3] |= ((value & 1) << 5);
}

void ADTSHead::setHome(const unsigned int &value) {
    this->data[3] |= ((value & 1) << 4);
}

void ADTSHead::setCopyrightIdentificationBit(const unsigned int &value) {
    this->data[3] |= ((value & 1) << 3);
}

void ADTSHead::setCopyrightIdentificationStart(const unsigned int &value) {
    this->data[3] |= ((value & 1) << 2);
}

void ADTSHead::setAacFrameLength(const unsigned int &value) {
    this->data[3] |= ((value >> 11) & 0x3);
    this->data[4] = (value & 0x7f8);
    this->data[5] |= ((value & 0x7) << 5);
}

void ADTSHead::setAdtsBufferFullness(const unsigned int &value) {
    this->data[5] |= ((value >> 6) & 0x1f);
    this->data[6] |= ((value & 0x3f) << 2);
}

void ADTSHead::setNumberOfRawDataBlocksInFrame(const unsigned int &value) {
    this->data[6] |= (value & 0x3);
}

void ADTSHead::writeBytes2File(std::ofstream &outStream) {
    outStream << this->data;
}


