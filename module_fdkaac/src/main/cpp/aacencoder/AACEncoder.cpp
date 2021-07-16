#include <AACEncoder.h>

AACEncoder::AACEncoder() {

}

AACEncoder::~AACEncoder() {
    //删除开辟的内存空间
    delete[] mInBuffer;
    //关闭文件
    outStream.close();
}

/**
 *  初始化函数
 * @param profile
 * @param sampleRate
 * @param channels
 * @param bitRate
 * @return 正确时返回AACENC_OK
 */
int AACEncoder::init(const std::string &name, const AACProfile &profile, const int &sampleRate,
                     const int &channels,
                     const int &bitRate, const bool &isOutputWithoutADTS) {
    AACENC_ERROR err;
    //打开编码器，如果非常需要节省内存则可以调整encModules
    if ((err = aacEncOpen(&this->mAacEncoder, 0x0, channels)) != AACENC_OK) {
        LOGE("Unable to open fdkaac encoder, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        return err;
    }
    //设置AAC标准格式
    if ((err = aacEncoder_SetParam(this->mAacEncoder, AACENC_AOT, profile)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_AOT, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return err;
    }
    //设置采样率
    if ((err = aacEncoder_SetParam(this->mAacEncoder, AACENC_SAMPLERATE, sampleRate)) !=
        AACENC_OK) {
        LOGE("Unable to set the SAMPLERATE, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return err;
    }
    //设置通道数
    if ((err = aacEncoder_SetParam(this->mAacEncoder, AACENC_CHANNELMODE, channels)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_CHANNELMODE, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return err;
    }
    //设置比特率
    if ((err = aacEncoder_SetParam(this->mAacEncoder, AACENC_BITRATE, bitRate)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_BITRATE, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return err;
    }
    //设置输出格式，AAC裸流(0)或者是ADTS流(2)
    //ADTS格式的封装
    int encode_mode = TT_MP4_ADTS;
    if (this->isFlagGlobalHeader)
        //没有任何格式的封装
        encode_mode = TT_MP4_RAW;
    //设置编码输出的格式数据是否带AAC ADTS头
    if ((err = aacEncoder_SetParam(this->mAacEncoder, AACENC_TRANSMUX, encode_mode)) != AACENC_OK) {
        LOGE("Unable to set the ADTS AACENC_TRANSMUX, ERR = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return err;
    }
    //初始化编码器
    if ((err = aacEncEncode(this->mAacEncoder, NULL, NULL, NULL, NULL)) != AACENC_OK) {
        LOGE("Unable to initialize the aacEncEncode, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return err;
    }

    this->isFlagGlobalHeader = isOutputWithoutADTS;

    //计算pcm帧长
    //每次传入进行处理的数据的长度,frameLength是每帧每个channel的采样点数,每个PCM采样点占16bit即是2byte,mInputSizeFixed此处代表byte总数
    this->mInputSizeFixed = channels * 2 * this->mEncInfo.frameLength;
    //偏移量
    this->mInBufferCursor = 0;
    this->mInBuffer = new uint8_t[this->mInputSizeFixed];
    //重新开辟内存空间，用以存储加入新内容后的数据
    memset(this->mInBuffer, 0, sizeof(this->mInBuffer));
    //如果是编码裸流，可以取出编码器的SpecInfo
    this->outStream.open(name + ".aac", std::ios::out | std::ios::trunc);
    LOGI("固定每次编码长度为: mInputSizeFixed = %d", this->mInputSizeFixed);
    return AACENC_OK;
}

/**
 * 编码
 * @param pData 需要编码的数据(PCM)
 * @param dataByteSize 编码数据的长度
 * @param outBuffer 输出的AAC裸流
 * @return 返回编码的AAC的长度
 */
int AACEncoder::encode(const Byte *const pData, int dataByteSize, char **outBuffer) {
    int pDataCursor = 0;
    *outBuffer = new char[dataByteSize];
    int packetSize = 0, cpySize = 0, aacPktSize = 0;
    while (dataByteSize > 0) {
        cpySize = 0;
        if (this->mInBufferCursor + dataByteSize >= this->mInputSizeFixed) {
            //可以进行编码，每次固定编码的长度为mInputSizeFixed
            cpySize = this->mInputSizeFixed - this->mInBufferCursor;
            memcpy(this->mInBuffer + this->mInBufferCursor, pData + pDataCursor, cpySize);
            aacPktSize = fdkEncoderAudio();
            if (aacPktSize > 0) {
                writeAACPacket2File(this->mAacOutBuffer, aacPktSize);
                memcpy(*outBuffer + packetSize, this->mAacOutBuffer, aacPktSize);
                packetSize += aacPktSize;
            }
            this->mInBufferCursor = 0;
            memset(this->mInBuffer, 0, sizeof(this->mInBuffer));
        } else {
            cpySize = dataByteSize;
            memcpy(this->mInBuffer + this->mInBufferCursor, pData + pDataCursor, cpySize);
            this->mInBufferCursor += cpySize;
        }
        dataByteSize -= cpySize;
        pDataCursor += cpySize;
    }
    return packetSize;
}

int AACEncoder::fdkEncoderAudio() {

    AACENC_BufDesc in_buf, out_buf;
    AACENC_InArgs in_args;
    AACENC_OutArgs out_args;

    int in_identifier = IN_AUDIO_DATA;
    int in_elem_size = 2;
    in_args.numInSamples = this->mInputSizeFixed / 2;
    in_buf.numBufs = 1;
    in_buf.bufs = (void **) (&this->mInBuffer);
    in_buf.bufferIdentifiers = &in_identifier;
    in_buf.bufSizes = &this->mInputSizeFixed;
    in_buf.bufElSizes = &in_elem_size;

    int out_identifier = OUT_BITSTREAM_DATA;
    void *out_ptr = this->mAacOutBuffer;
    int out_size = sizeof(this->mAacOutBuffer);
    int out_elem_size = 1;
    out_buf.numBufs = 1;
    out_buf.bufs = &out_ptr;
    out_buf.bufferIdentifiers = &out_identifier;
    out_buf.bufSizes = &out_size;
    out_buf.bufElSizes = &out_elem_size;

    //输出的是aac裸流
    if (aacEncEncode(this->mAacEncoder, &in_buf, &out_buf, &in_args, &out_args) != AACENC_OK) {
        LOGI("Encoding aac failed\n");
        return 0;
    }
    if (out_args.numOutBytes == 0)
        return 0;
    //编码后的aac数据存在outbuffer中，大小为out_args.numOutBytes
    return out_args.numOutBytes;
}

void AACEncoder::writeAACPacket2File(const uint8_t *const data, const int &datalen) {
    LOGI("After One Encode Size is : %d", datalen);
    //写入ADTS头
    if (isFlagGlobalHeader)
        //将ADTS头文件放到前面
        addADTS2Packet(datalen);
    this->outStream << data;
}

/**
 *
 * ADTS packet
 *
 * 参考ffmpeg源码中添加ADTS头部,信息可以参考:https://www.jianshu.com/p/5c770a22e8f8
 *
 * @param packetLen 该帧的长度，包括header的长度
 * @param profile  0-Main profile, 1-AAC LC，2-SSR
 * @param freqIdx    采样率
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
* @param chanCfg    通道
                    2：L+R
                    3：C+L+R
*/
void AACEncoder::addADTS2Packet(const int &packetLen, const int &profile, const int &freqIdx,
                                const int &chanCfg) {
    //profile默认为AAC LC, freqIdx默认为44.1KHz, chanCfg默认为单通道
    ADTSHead adtsHead;
    /* adts_fixed_header */
    adtsHead.setProfile(profile)
            .setSamplingFrequencyIndex(freqIdx)
            .setChannelConfiguration(chanCfg)
                    /* adts_variable_header */
            .setAacFrameLength(packetLen)
                    /* 将ADTS头写入文件中 */
            .writeBytes2File(this->outStream);
}

int AACEncoder::getEncoderInfo(const AACENC_InfoStruct *info) {
    AACENC_ERROR err;
    //获取编码器信息
    if ((err = aacEncInfo(this->mAacEncoder, &this->mEncInfo)) != AACENC_OK) {
        LOGE("Unable to get the aacEncInfo info, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return err;
    }
    info = &this->mEncInfo;
    return AACENC_OK;
}

void AACEncoder::destory() {
    LOGI("Enter AACEncoder Destroy");
    if (this->mAacEncoder != nullptr)
        aacEncClose(&this->mAacEncoder);
}


