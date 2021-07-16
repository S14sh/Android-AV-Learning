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