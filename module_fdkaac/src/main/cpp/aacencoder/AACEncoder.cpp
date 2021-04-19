#include "AACEncoder.h"
#include <android/log.h>

AACEncoder::AACEncoder() {

}

AACEncoder::~AACEncoder() {
    delete[] mInBuffer;
    outStream.close();
}

/**
 *  初始化函数
 * @param profile
 * @param sampleRate
 * @param channels
 * @param bitRate
 * @return 出现错误时返回0，否则返回1
 */
int AACEncoder::init(AACProfile profile, int sampleRate, int channels, int bitRate) {
    AACENC_ERROR err;
    //打开编码器，如果非常需要节省内存则可以调整encModules
    if ((err = aacEncOpen(&mAacEncoder, 0x0, channels)) != AACENC_OK) {
        LOGE("Unable to open fdkaac encoder, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        return 0;
    }
    //设置AAC标准格式
    if ((err = aacEncoder_SetParam(mAacEncoder, AACENC_AOT, profile)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_AOT, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return 0;
    }
    //设置采样率
    if ((err = aacEncoder_SetParam(mAacEncoder, AACENC_SAMPLERATE, sampleRate)) != AACENC_OK) {
        LOGE("Unable to set the SAMPLERATE, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return 0;
    }
    //设置通道数
    if ((err = aacEncoder_SetParam(mAacEncoder, AACENC_CHANNELMODE, channels)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_CHANNELMODE, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return 0;
    }
    //设置比特率
    if ((err = aacEncoder_SetParam(mAacEncoder, AACENC_BITRATE, bitRate)) != AACENC_OK) {
        LOGE("Unable to set the AACENC_BITRATE, err = 0x%x, error is %s\n", err, fdkaac_error(err));
        destory();
        return 0;
    }
    //设置输出格式，AAC裸流(0)或者是ADTS流(2)
    //ADTS格式的封装
    int encode_mode = TT_MP4_ADTS;
    if (isFlagGlobalHeader)
        //没有任何格式的封装
        encode_mode = TT_MP4_RAW;
    //设置编码输出的格式数据带AAC ADTS头
    if ((err = aacEncoder_SetParam(mAacEncoder, AACENC_TRANSMUX, encode_mode)) != AACENC_OK) {
        LOGE("Unable to set the ADTS AACENC_TRANSMUX, ERR = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return 0;
    }
    //初始化编码器
    if ((err = aacEncEncode(mAacEncoder, NULL, NULL, NULL, NULL)) != AACENC_OK) {
        LOGE("Unable to initialize the aacEncEncode, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return 0;
    }
    //获取编码器信息
    if ((err = aacEncInfo(mAacEncoder, &mEncInfo)) != AACENC_OK) {
        LOGE("Unable to get the aacEncInfo info, err = 0x%x, error is %s\n", err,
             fdkaac_error(err));
        destory();
        return 0;
    }
    //计算pcm帧长
    //frameLength是每帧每个channel的采样点数
    mInputSizeFixed = channels * 2 * mEncInfo.frameLength;
    mInBufferCursor = 0;
    mInBuffer = new uint8_t[mInputSizeFixed];
    memset(mInBuffer, 0, sizeof(mInBuffer));
    //如果是编码裸流，可以取出编码器的SpecInfo
    outStream.open("/sdcard/androidAVLearning/fdkaac_encode.aac", std::ios::out | std::ios::trunc);
    LOGI("固定每次编码长度为: mInputSizeFixed = %d", mInputSizeFixed);
    return 1;
}

/**
 * 编码
 * @param pData 需要编码的数据
 * @param dataByteSize 编码数据的长度
 * @param outBuffer 输出的AAC
 * @return 输出AAC的长度
 */
int AACEncoder::encode(Byte *pData, int dataByteSize, char **outBuffer) {
    int pDataCursor = 0;
    *outBuffer = new char[dataByteSize];
    int packetSize = 0, cpySize = 0, aacPktSize = 0;
    while (dataByteSize > 0) {
        cpySize = 0;
        if (mInBufferCursor + dataByteSize >= mInputSizeFixed) {
            //可以进行编码，每次固定编码的长度为mInputSizeFixed
            cpySize = mInputSizeFixed - mInBufferCursor;
            memcpy(mInBuffer + mInBufferCursor, pData + pDataCursor, cpySize);
            aacPktSize = fdkEncoderAudio();
            if (aacPktSize > 0) {
                writeAACPacket2File(mAacOutBuffer, aacPktSize);
                memcpy(*outBuffer + packetSize, mAacOutBuffer, aacPktSize);
                packetSize += aacPktSize;
            }
            mInBufferCursor = 0;
            memset(mInBuffer, 0, sizeof(mInBuffer));
        } else {
            cpySize = dataByteSize;
            memcpy(mInBuffer + mInBufferCursor, pData + pDataCursor, cpySize);
            mInBufferCursor += cpySize;
        }
        dataByteSize -= cpySize;
        pDataCursor += cpySize;
    }
    return packetSize;
}

/**
 * 进行编码
 * @return 返回编码的大小
 */
int AACEncoder::fdkEncoderAudio() {
    AACENC_BufDesc in_buf, out_buf;
    AACENC_InArgs in_args;
    AACENC_OutArgs out_args;
    int in_identifier = IN_AUDIO_DATA;
    int in_elem_size = 2;

    in_args.numInSamples = mInputSizeFixed / 2;
    in_buf.numBufs = 1;
    in_buf.bufs = (void **) (&in_identifier);
    in_buf.bufSizes = &mInputSizeFixed;
    in_buf.bufElSizes = &in_elem_size;

    int out_identifier = OUT_BITSTREAM_DATA;
    void *out_ptr = mAacOutBuffer;
    int out_size = sizeof(mAacOutBuffer);
    int out_elem_size = 1;
    out_buf.numBufs = 1;
    out_buf.bufs = &out_ptr;
    out_buf.bufferIdentifiers = &out_identifier;
    out_buf.bufSizes = &out_size;
    out_buf.bufElSizes = &out_elem_size;

    if ((aacEncEncode(mAacEncoder, &in_buf, &out_buf, &in_args, &out_args)) != AACENC_OK) {
        LOGI("Encoding aac failed\n");
        return 0;
    }
    if (out_args.numOutBytes == 0)
        return 0;
    //编码后的aac数据存在outbuffer中，大小为out_args.numOutBytes
    return out_args.numOutBytes;

}

void AACEncoder::writeAACPacket2File(uint8_t *data, int datalen) {
    LOGI("After One Encode Size is : %d", datalen);
    uint8_t *buffer = data;
    if (isFlagGlobalHeader) {//写入 ADTS
        datalen += 7;
        uint8_t *_buffer = new uint8_t[datalen];
        memset(_buffer, 0, datalen);
        memcpy(_buffer + 7, data, datalen - 7);
        addADTS2Packet(_buffer, datalen);
        outStream<<_buffer;
        delete[]_buffer;
        return;
    }
    //完整的流
    outStream<<buffer;
}

/**
 *
 * ADTS packet
 *
 * 参考ffmpeg源码中 添加 ADTS 头部
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
void AACEncoder::addADTS2Packet(uint8_t *packet, int packetLen) {
    int profile = 1;
    int freqIdx = 4; // 44.1KHz
    int chanCfg = 1; // 通道

    PutBitContext pb;
    init_put_bits(&pb, packet, ADTS_HEADER_SIZE);
    /* adts_fixed_header */
    put_bits(&pb, 12, 0xfff);   /* syncword ：代表一个ADTS帧的开始, 用于同步，解码器可通过0xFFF确定每个ADTS的开始位置*/
    put_bits(&pb, 1, 0);        /* ID ：MPEG Version: 0 for MPEG-4, 1 for MPEG-2*/
    put_bits(&pb, 2, 0);        /* layer always: '00'*/
    put_bits(&pb, 1, 1);        /* protection_absent */
    put_bits(&pb, 2, profile); /* profile_objecttype 表示使用哪个级别的AAC，如01 Low Complexity(LC) -- AAC LC*/
    put_bits(&pb, 4, freqIdx); //采样率的下标.
    put_bits(&pb, 1, 0);        /* private_bit */
    put_bits(&pb, 3, chanCfg); /* channel_configuration 声道数. 比如2表示立体声双声道.*/
    put_bits(&pb, 1, 0);        /* original_copy */
    put_bits(&pb, 1, 0);        /* home */

    /* adts_variable_header */
    put_bits(&pb, 1, 0);        /* copyright_identification_bit */
    put_bits(&pb, 1, 0);        /* copyright_identification_start */
    put_bits(&pb, 13, packetLen); /* aac_frame_length */
    put_bits(&pb, 11, 0x7ff);   /* adts_buffer_fullness */
    put_bits(&pb, 2, 0);        /* number_of_raw_data_blocks_in_frame */

    flush_put_bits(&pb);
}

void AACEncoder::destory() {
    LOGI("Enter AACEncoder Destroy");
    if (mAacEncoder != nullptr)
        aacEncClose(&mAacEncoder);
}


