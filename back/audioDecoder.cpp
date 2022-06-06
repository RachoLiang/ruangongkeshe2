#include <QDebug>
#include "backend/audioDecoder.h"
#include <string>
#include <string.h>

extern "C"
{
#include <libavcodec/avcodec.h>
}

/* Minimum SDL audio buffer size, in samples. */
#define SDL_AUDIO_MIN_BUFFER_SIZE 512
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

const char *filter_descr = "atempo=2.0";

//int AudioDecoder::init_filters(const char *filters_descr,AVFilterGraph* filterGraph,AVFilterContext* filterSrcCtx,AVFilterContext* filterSinkCtx)
//{
//    //初始化filter
//    char args[512];
//    int ret = 0;
//    const AVFilter *abuffersrc  = avfilter_get_by_name("abuffer");
//    const AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
//    AVFilterInOut *outputs = avfilter_inout_alloc();
//    AVFilterInOut *inputs  = avfilter_inout_alloc();

//    static const enum AVSampleFormat out_sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
//    static const int64_t out_channel_layouts[] = { AV_CH_LAYOUT_STEREO, -1 };
//    static const int out_sample_rates[] = { codecCtx->sample_rate, -1 };
//    AVRational time_base = stream->time_base;


//    do
//    {
//        filterGraph = avfilter_graph_alloc();
//        if (!outputs || !inputs || !filterGraph)
//        {
//            ret = AVERROR(ENOMEM);
//            break;
//        }

//        /* buffer audio source: the decoded frames from the decoder will be inserted here. */
//        if (!codecCtx->channel_layout)
//            codecCtx->channel_layout = av_get_default_channel_layout(codecCtx->channels);
//        snprintf(args, sizeof(args),
//                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%x",
//                 time_base.num, time_base.den, codecCtx->sample_rate,
//                 av_get_sample_fmt_name(codecCtx->sample_fmt), codecCtx->channel_layout);
//        ret = avfilter_graph_create_filter(&filterSrcCtx, abuffersrc, "in",
//                                           args, NULL, filterGraph);

//        if (ret < 0)
//        {
//            printf("Cannot create audio buffer source\n");
//            break;
//        }

//        /* buffer audio sink: to terminate the filter chain. */
//        ret = avfilter_graph_create_filter(&filterSinkCtx, abuffersink, "out",
//                                           NULL, NULL, filterGraph);
//        if (ret < 0)
//        {
//            printf("Cannot create audio buffer sink\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx, "sample_fmts", out_sample_fmts, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output sample format\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx, "channel_layouts", out_channel_layouts, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output channel layout\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx, "sample_rates", out_sample_rates, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output sample rate\n");
//            break;
//        }

//        outputs->name       = av_strdup("in");
//        outputs->filter_ctx = filterSrcCtx;
//        outputs->pad_idx    = 0;
//        outputs->next       = NULL;

//        inputs->name       = av_strdup("out");
//        inputs->filter_ctx = filterSinkCtx;
//        inputs->pad_idx    = 0;
//        inputs->next       = NULL;


//        if ((ret = avfilter_graph_parse_ptr(filterGraph, filters_descr,
//                                            &inputs, &outputs, NULL)) < 0)
//            qDebug()<<"rey:"<<ret;
//            break;

//        if ((ret = avfilter_graph_config(filterGraph, NULL)) < 0)
//            qDebug()<<"rey1:"<<ret;
//            break;

//    }
//    while(0);

//    avfilter_inout_free(&inputs);
//    avfilter_inout_free(&outputs);

//    return ret;
//}

//int AudioDecoder::init_filters(const char *filters_descr,AVFilterGraph* filterGraph,AVFilterContext* filterSrcCtx,AVFilterContext* filterSinkCtx)
//{
//    //初始化filter
//    char args[512];
//    int ret = 0;
//    const AVFilter *abuffersrc  = avfilter_get_by_name("abuffer");
//    const AVFilter *abuffersink = avfilter_get_by_name("abuffersink");
//    AVFilterInOut *outputs = avfilter_inout_alloc();
//    AVFilterInOut *inputs  = avfilter_inout_alloc();

//    static const enum AVSampleFormat out_sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
//    static const int64_t out_channel_layouts[] = { AV_CH_LAYOUT_STEREO, -1 };
//    static const int out_sample_rates[] = { codecCtx->sample_rate, -1 };
//    AVRational time_base = stream->time_base;


//    do
//    {
//        filterGraph2 = avfilter_graph_alloc();
//        if (!outputs || !inputs || !filterGraph2)
//        {
//            ret = AVERROR(ENOMEM);
//            break;
//        }

//        /* buffer audio source: the decoded frames from the decoder will be inserted here. */
//        if (!codecCtx->channel_layout)
//            codecCtx->channel_layout = av_get_default_channel_layout(codecCtx->channels);
//        snprintf(args, sizeof(args),
//                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%x",
//                 time_base.num, time_base.den, codecCtx->sample_rate,
//                 av_get_sample_fmt_name(codecCtx->sample_fmt), codecCtx->channel_layout);
//        ret = avfilter_graph_create_filter(&filterSrcCtx2, abuffersrc, "in",
//                                           args, NULL, filterGraph2);

//        if (ret < 0)
//        {
//            printf("Cannot create audio buffer source\n");
//            break;
//        }

//        /* buffer audio sink: to terminate the filter chain. */
//        ret = avfilter_graph_create_filter(&filterSinkCtx2, abuffersink, "out",
//                                           NULL, NULL, filterGraph2);
//        if (ret < 0)
//        {
//            printf("Cannot create audio buffer sink\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx2, "sample_fmts", out_sample_fmts, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output sample format\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx2, "channel_layouts", out_channel_layouts, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output channel layout\n");
//            break;
//        }

//        ret = av_opt_set_int_list(filterSinkCtx2, "sample_rates", out_sample_rates, -1,
//                                  AV_OPT_SEARCH_CHILDREN);
//        if (ret < 0)
//        {
//            printf( "Cannot set output sample rate\n");
//            break;
//        }

//        outputs->name       = av_strdup("in");
//        outputs->filter_ctx = filterSrcCtx2;
//        outputs->pad_idx    = 0;
//        outputs->next       = NULL;

//        inputs->name       = av_strdup("out");
//        inputs->filter_ctx = filterSinkCtx2;
//        inputs->pad_idx    = 0;
//        inputs->next       = NULL;


//        qDebug()<<"parse:"<<filters_descr;
//        if ((ret = avfilter_graph_parse_ptr(filterGraph2, filters_descr,
//                                            &inputs, &outputs, NULL)) < 0)
//            qDebug()<<"rey:"<<ret;
//            break;

//        if ((ret = avfilter_graph_config(filterGraph2, NULL)) < 0)
//            qDebug()<<"rey1:"<<ret;
//            break;

//    }
//    while(0);

//    avfilter_inout_free(&inputs);
//    avfilter_inout_free(&outputs);

//    return ret;
//}

int AudioDecoder::init_atempo_filter(AVFilterGraph **pGraph, AVFilterContext **src, AVFilterContext **out,
                      const char *value) {

    std::string x = "sample_rate=";
    std::string temp = ":";
    std::string temp1 = "sample_fmt=";
    std::string temp2 = "channel_layout=";
    x += std::to_string(codecCtx->sample_rate) + temp;
    x += temp1 + std::to_string(codecCtx->sample_fmt) + temp;
    x += temp2 + std::to_string(audioDstChannelLayout);
    qDebug()<<"src-拼接字符串："<<x.c_str();
    //初始化采样位数
    bitFormat = codecCtx->sample_fmt;
    //"sample_rate=48000:sample_fmt=s16p:channel_layous=stereo"

    //初始化AVFilterGraph
    AVFilterGraph *graph = avfilter_graph_alloc();
    //获取abuffer用于接收输入端
    const AVFilter *abuffer = avfilter_get_by_name("abuffer");
    AVFilterContext *abuffer_ctx = avfilter_graph_alloc_filter(graph, abuffer, "src");
    //设置参数，这里需要匹配原始音频采样率、数据格式（位数）
    if (avfilter_init_str(abuffer_ctx, x.c_str()) <
        0) {
        qDebug()<<"error init abuffer filter";
        return -1;
    }
    //初始化 filter

    //初始化atempo filter
    const AVFilter *atempo = avfilter_get_by_name("atempo");
    AVFilterContext *atempo_ctx = avfilter_graph_alloc_filter(graph, atempo, "atempo");
    //这里采用av_dict_set设置参数
    AVDictionary *args = NULL;

    av_dict_set(&args, "tempo", value, 0);      //调节倍速播放
    if (avfilter_init_dict(atempo_ctx, &args) < 0) {
        qDebug()<<"error init volume filter";
        return -1;
    }

    const AVFilter *aformat = avfilter_get_by_name("aformat");
    AVFilterContext *aformat_ctx = avfilter_graph_alloc_filter(graph, aformat, "aformat");

    if (avfilter_init_str(aformat_ctx,
                          "sample_rates=48000:sample_fmts=s16p:channel_layouts=stereo") < 0) {
        qDebug()<<"error init aformat filter";
        return -1;
    }
    //初始化sink用于输出
    const AVFilter *sink = avfilter_get_by_name("abuffersink");
    AVFilterContext *sink_ctx = avfilter_graph_alloc_filter(graph, sink, "sink");
    if (avfilter_init_str(sink_ctx, NULL) < 0) {//无需参数
        qDebug()<<"error init sink filter";
        return -1;
    }
    //链接各个filter上下文
    if (avfilter_link(abuffer_ctx, 0, atempo_ctx, 0) != 0) {
        qDebug()<<"error link to volume filter";
        return -1;
    }
    if (avfilter_link(atempo_ctx, 0, aformat_ctx, 0) != 0) {
        qDebug()<<"error link to aformat filter";
        return -1;
    }
    if (avfilter_link(aformat_ctx, 0, sink_ctx, 0) != 0) {
        qDebug()<<"error link to sink filter";
        return -1;
    }
    if (avfilter_graph_config(graph, NULL) < 0) {
        qDebug()<<"error config filter graph";
        return -1;
    }
    *pGraph = graph;
    *src = abuffer_ctx;
    *out = sink_ctx;
    qDebug()<<"init filter success...";
    return 0;
}



AudioDecoder::AudioDecoder(QObject *parent) :
    QObject(parent),
    isStop(false),
    isPause(false),
    isreadFinished(false),
    totalTime(0),
    clock(0),
    volume(SDL_MIX_MAXVOLUME),
    audioDeviceFormat(AUDIO_F32SYS),
    aCovertCtx(NULL),
    sendReturn(0),
    speed(1.0),
    speedChanged(true),
    init_falg(false),
    bitFormat(8)
{

}

int AudioDecoder::openAudio(AVFormatContext *pFormatCtx, int index)
{
    AVCodec *codec;
    SDL_AudioSpec wantedSpec;
    int wantedNbChannels;
    const char *env;

    /*  soundtrack array use to adjust */
    int nextNbChannels[]   = {0, 0, 1, 6, 2, 6, 4, 6};
    int nextSampleRates[]  = {0, 44100, 48000, 96000, 192000};
    int nextSampleRateIdx = FF_ARRAY_ELEMS(nextSampleRates) - 1;

    isStop = false;
    isPause = false;
    isreadFinished = false;

    audioSrcFmt = AV_SAMPLE_FMT_NONE;
    audioSrcChannelLayout = 0;
    audioSrcFreq = 0;

    pFormatCtx->streams[index]->discard = AVDISCARD_DEFAULT;

    stream = pFormatCtx->streams[index];

    codecCtx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codecCtx, pFormatCtx->streams[index]->codecpar);

    /* find audio decoder */
    if ((codec = (AVCodec*)avcodec_find_decoder(codecCtx->codec_id)) == NULL) {
        avcodec_free_context(&codecCtx);
        qDebug() << "Audio decoder not found.";
        return -1;
    }

    /* open audio decoder */
    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        avcodec_free_context(&codecCtx);
        qDebug() << "Could not open audio decoder.";
        return -1;
    }

    totalTime = pFormatCtx->duration;


    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    if (env) {
        qDebug() << "SDL audio channels";
        wantedNbChannels = atoi(env);
        audioDstChannelLayout = av_get_default_channel_layout(wantedNbChannels);
    }

    wantedNbChannels = codecCtx->channels;
    if (!audioDstChannelLayout ||
        (wantedNbChannels != av_get_channel_layout_nb_channels(audioDstChannelLayout))) {
        audioDstChannelLayout = av_get_default_channel_layout(wantedNbChannels);
        audioDstChannelLayout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }

    wantedSpec.channels    = av_get_channel_layout_nb_channels(audioDstChannelLayout);
    wantedSpec.freq        = codecCtx->sample_rate;
    if (wantedSpec.freq <= 0 || wantedSpec.channels <= 0) {
        avcodec_free_context(&codecCtx);
        qDebug() << "Invalid sample rate or channel count, freq: " << wantedSpec.freq << " channels: " << wantedSpec.channels;
        return -1;
    }

    while (nextSampleRateIdx && nextSampleRates[nextSampleRateIdx] >= wantedSpec.freq) {
        nextSampleRateIdx--;
    }

    wantedSpec.format      = audioDeviceFormat;
    wantedSpec.silence     = 0;
    wantedSpec.samples     = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wantedSpec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
    //音频回调函数：最重要
    wantedSpec.callback    = &AudioDecoder::audioCallback;
    wantedSpec.userdata    = this;

    /* This function opens the audio device with the desired parameters, placing
     * the actual hardware parameters in the structure pointed to spec.
     */
    while (1) {
        while (SDL_OpenAudio(&wantedSpec, &spec) < 0) {
            qDebug() << QString("SDL_OpenAudio (%1 channels, %2 Hz): %3")
                    .arg(wantedSpec.channels).arg(wantedSpec.freq).arg(SDL_GetError());
            wantedSpec.channels = nextNbChannels[FFMIN(7, wantedSpec.channels)];
            if (!wantedSpec.channels) {
                wantedSpec.freq = nextSampleRates[nextSampleRateIdx--];
                wantedSpec.channels = wantedNbChannels;
                if (!wantedSpec.freq) {
                    avcodec_free_context(&codecCtx);
                    qDebug() << "No more combinations to try, audio open failed";
                    return -1;
                }
            }
            audioDstChannelLayout = av_get_default_channel_layout(wantedSpec.channels);
        }

        if (spec.format != audioDeviceFormat) {
            qDebug() << "SDL audio format: " << wantedSpec.format << " is not supported"
                     << ", set to advised audio format: " <<  spec.format;
            wantedSpec.format = spec.format;
            audioDeviceFormat = spec.format;
            SDL_CloseAudio();
        } else {
            break;
        }
    }

    if (spec.channels != wantedSpec.channels) {
        audioDstChannelLayout = av_get_default_channel_layout(spec.channels);
        if (!audioDstChannelLayout) {
            avcodec_free_context(&codecCtx);
            qDebug() << "SDL advised channel count " << spec.channels << " is not supported!";
            return -1;
        }
    }

    /* set sample format */
    qDebug()<<"打开音频格式："<<audioDeviceFormat;
    switch (audioDeviceFormat) {
    case AUDIO_U8:
        audioDstFmt    = AV_SAMPLE_FMT_U8;
        audioDepth = 1;
        break;

    case AUDIO_S16SYS:
        audioDstFmt    = AV_SAMPLE_FMT_S16;
        audioDepth = 2;
        break;

    case AUDIO_S32SYS:
        audioDstFmt    = AV_SAMPLE_FMT_S32;
        audioDepth = 4;
        break;

    case AUDIO_F32SYS:
        audioDstFmt    = AV_SAMPLE_FMT_FLT;
        audioDepth = 4;
        break;

    default:
        audioDstFmt    = AV_SAMPLE_FMT_S16;
        audioDepth = 2;
        break;
    }

    /* open sound */
    SDL_PauseAudio(0);


    return 0;
}

void AudioDecoder::closeAudio()
{
    emptyAudioData();

    SDL_LockAudio();
    SDL_CloseAudio();
    SDL_UnlockAudio();

    avcodec_close(codecCtx);
    avcodec_free_context(&codecCtx);
}

void AudioDecoder::readFileFinished()
{
    qDebug()<<"设置为true";
    isreadFinished = true;
}

void AudioDecoder::pauseAudio(bool pause)
{
    isPause = pause;
}

void AudioDecoder::stopAudio()
{
    isStop = true;
}

void AudioDecoder::packetEnqueue(AVPacket *packet)
{
    packetQueue.enqueue(packet);
}

void AudioDecoder::emptyAudioData()
{
    audioBuf = nullptr;

    audioBufIndex = 0;
    audioBufSize = 0;
    audioBufSize1 = 0;

    clock = 0;

    sendReturn = 0;

    packetQueue.empty();
}

int AudioDecoder::getVolume()
{
    return volume;
}

void AudioDecoder::setVolume(int volume)
{
    this->volume = volume;
}

//倍速播放
void AudioDecoder::setSpeed(double speed){
    speedChanged = true;
    this->speed = speed;
}

double AudioDecoder::getSpeed(){
    return speed;
}



double AudioDecoder::getAudioClock()
{
    if (codecCtx) {
        /* control audio pts according to audio buffer data size */
        int hwBufSize   = audioBufSize - audioBufIndex;
        int bytesPerSec = codecCtx->sample_rate * codecCtx->channels * audioDepth;


        clock -= static_cast<double>(hwBufSize) / bytesPerSec;
    }
    return clock;
}

void AudioDecoder::audioCallback(void *userdata, quint8 *stream, int SDL_AudioBufSize)
{
    AudioDecoder *decoder = (AudioDecoder *)userdata;

    int decodedSize;
    /* SDL_BufSize means audio play buffer left size
     * while it greater than 0, means counld fill data to it
     */
    while (SDL_AudioBufSize > 0) {
        if (decoder->isStop) {
            qDebug()<<"音频回调函数：stop返回";
            return ;
        }

        if (decoder->isPause) {
            SDL_Delay(10);
            continue;
        }

        /* no data in buffer */
        if (decoder->audioBufIndex >= decoder->audioBufSize) {

            decodedSize = decoder->decodeAudio();
            /* if error, just output silence */
            if (decodedSize < 0) {
                /* if not decoded data, just output silence */
                decoder->audioBufSize = 1024;
                decoder->audioBuf = nullptr;
            } else {
                decoder->audioBufSize = decodedSize;
            }
            decoder->audioBufIndex = 0;
        }

        /* calculate number of data that haven't play */
        int left = decoder->audioBufSize - decoder->audioBufIndex;
        if (left > SDL_AudioBufSize) {
            left = SDL_AudioBufSize;
        }

        if (decoder->audioBuf) {
            memset(stream, 0, left);
            SDL_MixAudio(stream, decoder->audioBuf + decoder->audioBufIndex, left, decoder->volume);
        }

        SDL_AudioBufSize -= left;
        stream += left;
        decoder->audioBufIndex += left;
    }
}

//获取pcm的db值
int AudioDecoder::getPcmDb(){
    int size = audioBufIndex;
    unsigned char* pcmData = audioBuf;
    //参数校验
    if(pcmData == NULL){
        return 0;
    }
    if(size <= 0){
        return 0;
    }

    int sumDb = 0;
    //累加分贝值
    for (int i = 0;i < size; i++){
        sumDb += pcmData[i];
    }
    return sumDb/size;
//    return (int)(20.0*log10(sumDb/size));
}

int AudioDecoder::decodeAudio()
{

    int ret;
    AVFrame *frame = av_frame_alloc();
    int resampledDataSize;

    if (!frame) {
        qDebug() << "Decode audio frame alloc failed.";
        return -1;
    }

    if (isStop) {
        qDebug()<<"音频解码:stop返回";
        return -1;
    }


    if (packetQueue.queueSize() <= 0) {
        if (isreadFinished) {
            isStop = true;
            SDL_Delay(100);
            qDebug()<<"播放结束了";
            emit playFinished();
            return -1;
        }

    }else{
        if (isreadFinished) {
            isStop = true;
            SDL_Delay(100);
            qDebug()<<"队列不为空时，播放结束了";
            qDebug()<<"此时队列长度："<<packetQueue.queueSize();
            packetQueue.empty();
            qDebug()<<"清空后队列长度："<<packetQueue.queueSize();
            emit playFinished();
            return -1;
        }
    }

    /* get new packet whiel last packet all has been resolved */
    if (sendReturn != AVERROR(EAGAIN)) {
        packetQueue.dequeue(&packet, true);
    }

    if (packet.data != nullptr &&!strcmp((char*)packet.data, "FLUSH")) {
        avcodec_flush_buffers(codecCtx);
        av_packet_unref(&packet);
        av_frame_free(&frame);
        sendReturn = 0;
        qDebug() << "seek audio";
        return -1;
    }

    /* while return -11 means packet have data not resolved,
     * this packet cannot be unref
     */
    sendReturn = avcodec_send_packet(codecCtx, &packet);
    if ((sendReturn < 0) && (sendReturn != AVERROR(EAGAIN)) && (sendReturn != AVERROR_EOF)) {
        av_packet_unref(&packet);
        av_frame_free(&frame);
        qDebug() << "Audio send to decoder failed, error code: " << sendReturn;
        return sendReturn;
    }

    ret = avcodec_receive_frame(codecCtx, frame);

    //判断是否需要改变倍速
    if(speedChanged){
        //初始化filter
        qDebug()<<"改成倍速："<<speed;
        qDebug()<<"初始化滤波器情况："<<init_atempo_filter(&filterGraph,&filterSrcCtx,&filterSinkCtx,std::to_string(speed).c_str());
        speedChanged = ! speedChanged;
    }


    if(ret >= 0){
        //记录进入滤波器前的参数
        double pts = frame->pts;
        double pos = frame->pkt_pos;

        //更新时间
        nowTime = frame->best_effort_timestamp * av_q2d(stream->time_base);

        int flag ;

        if ((flag = av_buffersrc_add_frame_flags(filterSrcCtx, frame,AV_BUFFERSRC_FLAG_KEEP_REF)) < 0) {//将frame放入输入filter上下文
                            qDebug()<<"error add frame:"<<flag;

                        }
        av_buffersink_get_frame(filterSinkCtx, frame);

        frame->pts = pts;
        frame->pkt_pos = pos;

        //记录播放时长
        nowTime = frame->best_effort_timestamp * av_q2d(stream->time_base) * AV_TIME_BASE;


    }

    if ((ret < 0) && (ret != AVERROR(EAGAIN))) {
        av_packet_unref(&packet);
        av_frame_free(&frame);
        qDebug() << "Audio frame decode failed, error code: " << ret;
        return ret;
    }

    if (frame->pts != AV_NOPTS_VALUE) {
        clock = av_q2d(stream->time_base) * frame->pts;
    }

    /* get audio channels */
    qint64 inChannelLayout = (frame->channel_layout && frame->channels == av_get_channel_layout_nb_channels(frame->channel_layout)) ?
                frame->channel_layout : av_get_default_channel_layout(frame->channels);

    //重采样上下文
    if (frame->format       != audioSrcFmt              ||
        inChannelLayout     != audioSrcChannelLayout    ||
        frame->sample_rate  != audioSrcFreq             ||
        !aCovertCtx) {
        if (aCovertCtx) {
            swr_free(&aCovertCtx);
            qDebug()<<"释放了swr上下文";
        }

        /* init swr audio convert context */
        aCovertCtx = swr_alloc_set_opts(nullptr, audioDstChannelLayout, audioDstFmt, spec.freq,
                inChannelLayout, (AVSampleFormat)frame->format , frame->sample_rate, 0, NULL);
        if (!aCovertCtx || (swr_init(aCovertCtx) < 0)) {
            qDebug()<<"重采样上下文初始化失败";
            av_packet_unref(&packet);
            av_frame_free(&frame);
            return -1;
        }

        audioSrcFmt             = (AVSampleFormat)frame->format;
        audioSrcChannelLayout   = inChannelLayout;
        audioSrcFreq            = frame->sample_rate;
        audioSrcChannels        = frame->channels;
    }


    if (aCovertCtx) {
        const quint8 **in   = (const quint8 **)frame->extended_data;
        uint8_t *out[] = {audioBuf1};

        int outCount = sizeof(audioBuf1) / spec.channels / av_get_bytes_per_sample(audioDstFmt);

        int sampleSize = swr_convert(aCovertCtx, out, outCount, in, frame->nb_samples);
        if (sampleSize < 0) {
            ///qDebug() << "swr convert failed";
            av_packet_unref(&packet);
            av_frame_free(&frame);
            return -1;
        }

        if (sampleSize == outCount) {
            qDebug() << "audio buffer is probably too small";
            if (swr_init(aCovertCtx) < 0) {
                swr_free(&aCovertCtx);
            }
        }

        audioBuf = audioBuf1;
        resampledDataSize = sampleSize * spec.channels * av_get_bytes_per_sample(audioDstFmt);
    } else {
        audioBuf = frame->data[0];
        resampledDataSize = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, static_cast<AVSampleFormat>(frame->format), 1);
    }

    //计算分贝值
//    qDebug()<<"buf的size："<<audioBufSize;
//    qDebug()<<"buf的indeX:"<<audioBufIndex;

    clock += static_cast<double>(resampledDataSize) / (audioDepth * codecCtx->channels * codecCtx->sample_rate);

    if (sendReturn != AVERROR(EAGAIN)) {
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);

    return resampledDataSize;
}

AVRational AudioDecoder::getTimeBase(){
    if(codecCtx){
        return codecCtx->time_base;
    }
}
