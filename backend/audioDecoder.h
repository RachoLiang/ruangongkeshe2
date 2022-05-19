#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include <QObject>
#include "avPacketQueue.h"

extern "C"
{
    #include <libswresample/swresample.h>
}



class AudioDecoder : public QObject
{
    Q_OBJECT
public:
    explicit AudioDecoder() {}
    virtual ~AudioDecoder() {}

    //音频打开、暂停、关闭等控制
    int open(AVFormatContext* formatCtx,int index);
    void close();
    void pause();
    void stop();
    int getVolume();
    void setVolume(int volume); //同时也设置audio对象中的音量等信息，供前端同步显示
    double getAudioClock(); //获取时钟信息
    void avpacketEnqueue(AVPacket* packet); //包入队列
    void clearDate();   //清空数据
    void setTotalTime(qint64 time); //设置播放总时长

private:
    int decodeAudio();  //解析传入的音频流
    static void audioCallBack(void* userData,quint8* stream,int SDL_AudioBufSize);  //数据回调函数

    //标志位
    bool isStop;
    bool isPause;
    bool isReadFinished;

    //控制信息
    qint64 totalTime;
    double clock;
    int volume;

    //资源信息
    AVStream* stream;

    quint8* audioBuf;
    quint audioBufSize;
    DECLARE_ALIGNED(16,quint8,audioBuf1) [192000];
    quint32 audioBufSize1;
    quint32 audioBufIndex;

    SDL_AudioSpec spec; //音频信息

    quint32 audioDeviceFormat; //音频解析器格式
    quint8 audioDepth;
    struct SwrContext* aCovertCtx;
    qint64 audioDstChannelLayout;
    enum AVSampleFormat audioDstFmt;    //音频解析采样格式

    qint64 audioSrcChannelLayout;
    int audioSrcChannels;   //音频资源通道数
    enum AVSampleFormat audioSrcFmt;
    int audioSrcFreq;

    AVCodecContext* codeCtx;    //解析器上下文

    AvPacketQueue packetQueue;  //包队列

    AVPacket packet;

    int sendReturn;

signals:
    void sign_playFinished();

public slots:
    void slot_readFileFinished();


};

#endif // AUDIODECODER_H
