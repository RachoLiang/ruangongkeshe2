#pragma once
#include <qthread.h>
#include <qdebug.h>
#include <QImage>
#include <qqueue.h>
#include "SDL2/SDL.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

struct Frame
{
    QImage frame;		//某一帧
    double timestamp;	//该帧的时间戳，单位：秒
};

/*
功能：完成视频的倒放
调用方法：点击“倒放”按钮后，使用视频文件名创建一个ReverseDecoder对象
*/
class ReverseDecoder : public QThread
{
    Q_OBJECT
public:
    ReverseDecoder(QString);
    ~ReverseDecoder();

    //文件名
    void setFilename(QString);
    QString getFilename();

    //状态
    enum { PLAYING, PAUSE, STOP };	//播放状态enum
    int getState();                 //获取播放状态
    void pause();                   //设置为PAUSE/PLAYING
    void stop();                    //设置为STOP

    qint64 getCurrTime();   //获取当前帧时间戳，单位：秒
    void setCurrTime(double);     //设置当前帧的时间，单位：秒
    qint64 getTotalTime();  //获取总时长，单位：秒

    void seekBySlider(double);  //点击进度条，跳转

    bool isRunFinished();
    bool isPlayThreadFinished();

private:
    int init();
    void freeRAM();
    void run();
    static int playThread(void*);
    void frameList2playList();

    char filename[1024];	//视频文件名
    int state;	//播放状态：PLAYING, PAUSE, STOP

    AVFormatContext* formatCtx;
    AVStream* videoStream;
    AVCodecContext* codecCtx;
    AVCodec* codec;
    SwsContext* convertCtx;	//转码所需context


    int video_stream_index;	//视频流index
    int video_width;	//视频宽度
    int video_height;	//视频高度

    AVFrame* srcFrame;		//解码packet后得到的frame
    AVFrame* RGBFrame;		//srcFrame转为RGB后得到的frame
    uint8_t* RGB_buffer;	//存放RGB帧的buffer

    QList<Frame> frameList;	//保存解码后的帧及其时间戳
    QList<Frame> playList;	//保存待播放的帧
    SDL_mutex* frameListMutex;  //控制frameList的互斥锁
    SDL_mutex* playListMutex;   //控制playList的互斥锁

    //单位：秒
    double seekStart;		//一段解码的起始位置
    double seekEnd;			//一段解码的结束位置
    double currTime;        //当前帧时间戳
    double duration;        //总时长
    double interval;         //两帧时间差

    bool seekEvent;			//是否需要seek
    bool afterSeek;			//是否seek完

    bool decodeFinished;	//解码是否已经完成

    int frameRate;			//帧率

    bool runFinished;       //线程结束
    bool playThreadFinished;//playThread线程结束

signals:
    void sendOneFrame(QImage);
};
