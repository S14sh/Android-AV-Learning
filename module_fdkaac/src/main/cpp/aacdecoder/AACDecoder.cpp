#include <AACDecoder.h>

AACDecoder::AACDecoder() : mDecoder(nullptr), mSpecInfoSize(64), mPkgSize(-1), mSpecInfo(nullptr) {

}

AACDecoder::~AACDecoder() {

}

int AACDecoder::initWithRawFormat(unsigned char *specInfo, unsigned int specInfoSize) {
    LOGI("initWithRawFormat");

    int nrOfLayers = 1;
    //打开AAC编码器
    mDecoder = aacDecoder_Open(TT_MP4_RAW, nrOfLayers);
    //配置编码器信息
    int ret = aacDecoder_ConfigRaw(mDecoder, &specInfo, &mSpecInfoSize);
    if (ret != AAC_DEC_OK) {
        LOGE("unable to set configRaw\n");
        aacDecoder_Close(mDecoder);
        return ret;
    }

    iniFrameSize();
    printAACInfo();

    return AAC_DEC_OK;
}

int AACDecoder::initWithADTSFormat() {
    LOGI("initWithRawFormat");

    int nrOfLayers = 1;

    //打开AAC解码器
    mDecoder = aacDecoder_Open(TT_MP4_ADTS, nrOfLayers);

    printAACInfo();

    return AAC_DEC_OK;
}

int AACDecoder::decode(unsigned char *pck, int len, unsigned char **outBuffer) {
    LOGI("Enter AACDecoder Decode");

    int threshold = (FDK_MAX_AUDIO_FRAME_SIZE * 3) / 2;
    uint8_t pcm_buf[threshold];

    int pcm_buf_index = 0;
    int pcm_buf_size = 0;

    int data_size = 0, len1 = 0;
    while(len > 0){
        data_size = threshold;
        len1 = this->fdkDecoderAudio((INT_PCM *) (pcm_buf + pcm_buf_index), &data_size, pck, len);
        if(len1 < 0){
            //如果当前帧错误，跳过
            len = 0;
            break;
        }
        pck += len1;
        len -= len1;
        if(data_size <= 0)
            //当前没有数据，获取更多帧
            break;
        pcm_buf_index += data_size;
        pcm_buf_size += data_size;
    }
    if(pcm_buf_size > 0){
        *outBuffer = new unsigned char[pcm_buf_size];
        memcpy(*outBuffer, pcm_buf, pcm_buf_size);
    }
    return pcm_buf_size;
}

void AACDecoder::destroy() {
    LOGI("Enter AACDecoder Destroy");
    if(mDecoder != nullptr)
        aacDecoder_Close(mDecoder);
}

int AACDecoder::fdkDecoderAudio(INT_PCM *outBuffer, int *outSize, unsigned char *buffer,
                                int size) {
    printAACInfo();
    int ret = 0;
    unsigned int pkt_size = size;
    unsigned int valid_size = size;
    unsigned char *input_buf[1] = {buffer};

    //1.
    ret = aacDecoder_Fill(mDecoder, input_buf, &pkt_size, &valid_size);
    if(ret != AAC_DEC_OK){
        fprintf(stderr, "Fill failed: %x\n", ret);
        *outSize = 0;
        return ret;
    }

    int buf_size = mPkgSize;
    if(mPkgSize <= 0)
        buf_size = 10 * 1024;

    //2.
    int fdk_flags = 0;
    ret = aacDecoder_DecodeFrame(mDecoder, outBuffer, buf_size, fdk_flags);
    if(ret == AAC_DEC_NOT_ENOUGH_BITS){
        fprintf(stderr, "not enough\n");
        *outSize = 0;
        return (size - valid_size);
    }
    if(ret != AAC_DEC_OK){
        fprintf(stderr, "aacDecoder_DecodeFrame : 0x%x\n", ret);
        *outSize = 0;
        return ret;
    }
    if(mPkgSize <= 0)
        iniFrameSize();
    *outSize = mPkgSize;
    return (size - valid_size);

}

void AACDecoder::iniFrameSize() {
    CStreamInfo *aac_stream_info = aacDecoder_GetStreamInfo(mDecoder);
    mPkgSize = aac_stream_info->channelConfig * 2 * aac_stream_info->aacSamplesPerFrame;
}

void AACDecoder::printAACInfo() {
    CStreamInfo *aac_stream_info = aacDecoder_GetStreamInfo(mDecoder);
    if (aac_stream_info == nullptr) {
        LOGI("aacDecoder_GetStreamInfo failed!\n");
        return;
    }
    LOGI("> stream info: channel = %d\tsample_rate = %d\tframe_size = %d\taot = %d\tbitrate = %d\n",
         aac_stream_info->channelConfig,
         aac_stream_info->sampleRate,
         aac_stream_info->aacSamplesPerFrame,
         aac_stream_info->aot,
         aac_stream_info->bitRate);
}