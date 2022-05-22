#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include <QObject>
#include "avPacketQueue.h"
#include "audio.h"

extern "C"
{
    #include <libswresample/swresample.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/opt.h>
    #include <libavcodec/avfft.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/time.h>
}



class AudioDecoder : public QObject
{
    Q_OBJECT
public:
    explicit AudioDecoder(QObject *parent = nullptr);

    int openAudio(AVFormatContext *pFormatCtx, int index);
    void closeAudio();
    void pauseAudio(bool pause);
    void stopAudio();
    int getVolume();
    void setVolume(int volume);
    double getAudioClock();
    void packetEnqueue(AVPacket *packet);
    void emptyAudioData();
    void setTotalTime(qint64 time);

    void setSpeed(double speed);    //调整音频播放倍速
    double getSpeed();  //获取当前倍速

    //初始化音频的过滤器函数
    int init_atempo_filter(AVFilterGraph **pGraph, AVFilterContext **src, AVFilterContext **out,
                          const char *value);

private:
    int decodeAudio();
    static void audioCallback(void *userdata, quint8 *stream, int SDL_AudioBufSize);

    bool isStop;
    bool isPause;
    bool isreadFinished;

    qint64 totalTime;
    double clock;
    int volume;

    AVStream *stream;

    Audio* audio;   //audio对象

    quint8 *audioBuf;
    quint32 audioBufSize;
    DECLARE_ALIGNED(16, quint8, audioBuf1) [192000];
    quint32 audioBufSize1;
    quint32 audioBufIndex;

    SDL_AudioSpec spec;

    quint32 audioDeviceFormat;  // audio device sample format
    quint8 audioDepth;
    struct SwrContext *aCovertCtx;
    qint64 audioDstChannelLayout;
    enum AVSampleFormat audioDstFmt;   // audio decode sample format

    qint64 audioSrcChannelLayout;
    int audioSrcChannels;
    enum AVSampleFormat audioSrcFmt;
    int audioSrcFreq;

    AVCodecContext *codecCtx;          // audio codec context

    AvPacketQueue packetQueue;

    AVPacket packet;

    int sendReturn;

    //倍速播放filter信息
    AVFilterGraph* filterGraph;
    AVFilterContext* filterSrcCtx;
    AVFilterContext* filterSinkCtx;

    //倍速播放
    double speed;   //播放速度
    bool speedChanged;  //是否改变播放速度


    bool init_falg;


signals:
    void playFinished();

public slots:
    void readFileFinished();


};

#endif // AUDIODECODER_H
