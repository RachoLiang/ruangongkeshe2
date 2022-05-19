#include <QDebug>
#include "backend/audioDecoder.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

//最小的SDL音频缓冲大小
#define SDL_AUDIO)MIN_BUFFER_SIZE 512

#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30


AudioDecoder::AudioDecoder(QObject* parent):
    QObject(parent),
    isStop(false),
    isPause(false),
    isReadFinished(false),
    totalTime(0),
    clock(0),
    volume(SDL_MIX_MAXVOLUME),
    audioDeviceFormat(AUDIO_F32SYS),
    aCovertCtx(NULL),
    sendReturn(0)
{

}

//打开音频
int AudioDecoder::open(AVFormatContext *formatCtx, int index){
    AVCodec* codec;
    SDL_AudioSpec suitSpec;
    int suitNbChannels;
    const char* env;

    //声道信息初始化
    int nextNbChannels[]   = {0, 0, 1, 6, 2, 6, 4, 6};
    int nextSampleRates[]  = {0, 44100, 48000, 96000, 192000};
    int nextSampleRateIdx = FF_ARRAY_ELEMS(nextSampleRates) - 1;

    //控制参数初始化
    isStop = false;
    isPause = false;
    isreadFinished = false;

    audioSrcFmt = AV_SAMPLE_FMT_NONE;
    audioSrcChannelLayout = 0;
    audioSrcFreq = 0;

    formatCtx->streams[index]->discard = AVDISCARD_DEFAULT;

    //解析的音频流
    stream = formatCtx->streams[index];

    //初始化解码上下文
    codeCtx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codeCtx,formatCtx->streams[index]->codecpar);

    //找到音频解析器
    if ((codec = (AVCodec*)avcodec_find_decoder(codecCtx->codec_id)) == NULL) {
        avcodec_free_context(&codecCtx);
        qDebug() << "Audio decoder not found.";
        return -1;
    }

    //打开音频解析器
    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        avcodec_free_context(&codecCtx);
        qDebug() << "Could not open audio decoder.";
        return -1;
    }

    //获取时长
    totalTime = formatCtx->duration;

    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    if(env){
        qDebug()<<"获取SDL音频通道数";
        suitNbChannels = atoi(env);
        audioDstChannelLayout = av_get_default_channel_layout(suitNbChannels);

    }

    suitNbChannels = codeCtx->channels;

    //对比SDL获取的参数和资源参数
    if(!audioDstChannelLayout ||
    (suitNbChannels != av_get_channel_layout_nb_channels(audioDstChannelLayout))){
        audioDstChannelLayout = av_get_default_channel_layout(suitNbChannels);
        audioDstChannelLayout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }

    //音频输出格式
    suitSpec.channels = av_get_channel_layout_nb_channels(audioDstChannelLayout);
    suitSpec.freq = codeCtx->sample_rate;

    //异常情况
    if(suitSpec.freq <= 0|| suitSpec.channels <= 0){
        avcodec_free_context(&codeCtx);
        qDebug()<<"无效的采样速率或通道数";
        return -1;
    }

    //找到合适的解析频率
    while(nextSampleRateIdx && nextSampleRates[nextSampleRateIdx] >= suitSpec.freq){
        nextSampleRateIdx--;
    }

    //初始化音频输出参数
    suitSpec.format = audioDeviceFormat;
    suitSpec.silence = 0;
    suitSpec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wantedSpec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
    //音频条函数
    suitSpec.callback = &AudioDecoder::audioCallBack;
    suitSpec.userdata = this;

    //打开音频设备
    while (1) {
        while (SDL_OpenAudio(&suitSpec,&spec) < 0){
        qDebug() << QString("SDL_OpenAudio (%1 channels, %2 Hz): %3")
                .arg(suitSpec.channels).arg(suitSpec.freq).arg(SDL_GetError());
        suitSpec.channels = nextNbChannels[FFMIN(7, suitSpec.channels)];
        if (!suitSpec.channels){
            suitSpec.freq = nextSampleRates[nextSampleRateIdx--];
            suitSpec.channels = suitNbChannels;
            if(!suitSpec.freq){
                avcodec_free_context(&codeCtx);
                qDebug()<<"没有合适的环境，音频打开失败";
                return -1;
            }
        }
        //重新获取channelLayout参数
        audioDstChannelLayout = av_get_default_channel_layout(suitSpec.channels);
    }

    //formt不一致
    if(spec.format != audioDeviceFormat){
        qDebug() << "SDL audio format: " << wantedSpec.format << " is not supported"
                 << ", set to advised audio format: " <<  spec.format;
        suitSpec.format = spec.format;
        audioDeviceFormat = spec.format;
        SDL_CloseAudio();
    }else {
    break;
    }
    }

    //channel不一致
    if(spec.channels != suitSpec.channels){
        audioDstChannelLayout = av_get_default_channel_layout(spec.channels);
        if(!audioDstChannelLayout){
            avcodec_free_context(&codeCtx);
            qDebug()<<"SDL建议的通道数："<<spec.channels<<" 不被支持";
            return -1;
        }
    }

    //设置采样格式
    switch(audioDeviceFormat){
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

    //开启声音
    SDL_PauseAudio(0);

    return 0;

}

//关闭音频
void AudioDecoder::close(){
    //清空数据
    clearDate();

    //关闭音频
    SDL_LockAudio();
    SDL_CloseAudio();
    SDL_UnlockAudio();

    //关闭解析器
    avcodec_close(codeCtx);
    //释放内存
    avcodec_free_context(&codeCtx);
}

//读取文件完毕-信号槽
voud AudioDecoder::slot_readFileFinished(){
    isReadFinished = true;
}

//暂停
void AudioDecoder::pause(bool pause){
    isPause = true;
}

//停止
void AudioDecoder::stop(){
    isStop = true;
}

//包入队列
void AudioDecoder::avpacketEnqueue(AVPacket *packet){

}

//清空数据
void AudioDecoder::clearDate(){
    //重置缓冲区信息
    audioBuf = nullptr;
    audioBufSize = 0;
    audioBufSize1 = 0;
    audioBufIndex = 0;

    clock = 0;
    sendReturn = 0;

    //清空包队列
    packetQueue.empty();
}

//获取音量
int AudioDecoder::getVolume(){
    return volume;
}

//设置音量
void AudioDecoder::setVolume(int volume){
    this->volume = volume;
}

//获取音频解析时钟信号
double AudioDecoder::getAudioClock(){
    if(codeCtx){
        //根据音频缓冲区数据大小来控制pts
        //还未播放的数据
        int resBufSize = audioBufSize - audioBufIndex;
        //每秒音频播放的字节数
        int bytePerSec = codeCtx->sample_rate * codeCtx->channels * audioDepth;
        clock = clock - static_cast<double>(resBufSize) / bytePerSec;
    }
}

//音频解析回调函数,负责将audioBuf中解码后的音频输出
void AudioDecoder::audioCallBack(void *userData, quint8 *stream, int SDL_AudioBufSize){
     AudioDecoder* decoder = (AudioDecoder*) userData;

     //已经解析的位置
     int hasDecodeIndex;

     //循环，直至取到指定大小的音频数据
     while(SDL_AudioBufSize > 0){
         //处理控制信息
         if(decoder->isStop){
             return;
         }
         if(decoder->isPause){
             //轮询
             SDL_Delay(10);
             continue;
         }
         //缓冲区没有数据
         if(decoder->audioBufIndex >= decoder->audioBufSize){
             hasDecodeIndex = decoder->decodeAudio();
             if(hasDecodeIndex < 0){
                 //没有解析的数据，则静音
                 decoder->audioBufSize = 1024;
                 decoder->audioBuf = nullptr;
             }else {
                 decoder->audioBufSize = hasDecodeIndex;
             }
             //重置当前获取数据的位置
             decoder->audioBufIndex = 0;
         }
         //已解码数据多于需要的数据,多余数据不能播放
         int left = decoder->audioBufSize - decoder->audioBufIndex;
         if(left > SDL_AudioBufSize){
             left = SDL_AudioBufSize;
         }

         if(decoder->audioBuf){
             memset(stream,0,left);
             //调整音量
             SDL_MixAudio(stream,decoder->audioBuf + decoder->audioBufIndex,left,decoder->volume);
         }

         SDL_AudioBufSize -= left;
         stream += left;
         decoder->audioBufIndex += left;
     }
}

//解析音频
int AudioDecoder::decodeAudio(){
    int ret;
    AVFrame* frame = av_frame_alloc();
    //已经重采样的数据大小
    int hasResampltSize;

    if(!frame){
        qDebug()<<"获取解析的音频帧失败";
        return -1;
    }

    if(isStop){
        return -1;
    }

    //待解码的包队列
    if(packetQueue.queueSize() <= 0){
        //文件读取完毕
        if(isReadFinished){
            isStop = true;
            SDL_Delay(100);
            emit sign_playFinished();
        }
        return -1;
    }

    //当上一个包完成解码后，获取新的包
    if(sendReturn != AVERROR(EAGAIN)){
        packetQueue.dequeue(&packet,true);
    }

    if(!strcmp((char*)packet.data,"FLUSH")){
        avcodec_flush_buffers(codeCtx);
        //引用次数减一
        av_packet_unref(&packet);
        av_frame_free(&frame);
        sendReturn = 0;
        qDebug()>>"跳过该帧音频";
        return -1;
    }

    //当上一个包还未完成解析
    sendReturn = avcodec_send_packet(codeCtx,&packet);
    if((sendReturn < 0) && (sendReturn != AVERROR(EAGAIN)) && (sendReturn != AVERROR_EOF)) {
        av_packet_unref(&packet);
        av_frame_free(&frame);
        qDebug()<<"发生音频解码失败，错误码："<<sendReturn;
        return sendReturn;
    }

    ret = avcodec_receive_frame(codeCtx,frame);
    if((ret < 0) && (ret != AVERROR(EAGAIN))) {
        av_packet_unref(&packet);
        av_frame_free(&frame);
        qDebug()<<"音频解码失败，错误码:"<< ret;
        return ret;
    }

    //pts无效，该如何处理？
    if(frame->pts != AV_NOPTS_VALUE){
        //计算该帧在整个音视频中的播放时间
        clock = av_p2d(stream->time_base) * frame->pts;

    }

    //获取音频的通道数
    qint64 inChannelLayout = (frame->channel_layout && frame->channels == av_get_channel_layout_nb_channels(frame->channel_layout)) ?
              frame->channel_layout : av_get_default_channel_layout(frame->channels);

    //frame采样格式不一致
    if(frame->format != audioSrcFmt ||
            inChannelLayout != audioSrcChannelLayout ||
            frame->sample_rate != audioSrcFreq ||
            !aCovertCtx){
        if(aCovertCtx){
            swr_free(&aCovertCtx);
        }

        //初始化重采样结构体
        aCovertCtx = swr_alloc_set_opts(nullptr,audioDstChannelLayout,audioDstFmt,spec.freq,
                                        inChannelLayout,(AVSampleFormat)frame->format,frame->sample_rate,0,NULL);
        //初始化aCoverCtx失败
        if(!aCovertCtx || (swr_init(aCovertCtx) < 0)) {
            av_packet_free(&packet);
            av_frame_free(&frame);
            return -1;
        }

        //重置重采样参数
        audioSrcFmt = (AVSampleFormat)frame->format;
        audioSrcChannelLayout = inChannelLayout;
        audioSrcFreq = frame->sample_rate;
        audioSrcChannels = frame->channels;

        if(aCovertCtx){
            const quint8 ** in = (const quint8 **)frame->extended_data;
            uint8_t* out[]={audioBuf1};
            int outCount = sizeof(audioBuf1) / (spec.channels * av_get_bytes_per_sample(audioDstFmt));

            //输出格式转换
            int sampleSize = swr_convert(aCovertCtx,out,outCount,in,out);
            if(sampleSize < 0) {
                av_packet_unref(&packet);
                av_frame_free(&frame);
                return -1;
            }

            if(sampleSize == outCount){
                qDebug()<<"音频缓冲区可能太小";
                if(swr_init(aCovertCtx) < 0) {
                    swr_free(&aCovertCtx);
                }
            }

            audioBuf = audioBuf1;
            hasResampltSize = sampleSize * spec.channels / av_get_bytes_per_sample(audioDstFmt);
        }else {
            //不需要格式转化
            audioBuf = frame->data[0];
            hasResampltSize = av_samples_get_buffer_size(NULL,frame->channels,frame->nb_samples,static_cast<AVSampleFormat>(frame->format),1);
        }

        //更新时钟
        clock += static_cast<double>hasResampltSize/(audioDepth * codeCtx->channels * codeCtx->sample_rate);

        if(sendReturn != AVERROR(EAGAIN)){
            //引用次数减一
            av_packet_unref(&packet);
        }

        av_frame_free(&frame);

        return hasResampltSize;
    }
}
