#ifndef MAINDECODER_H
#define MAINDECODER_H

#include <QThread>
#include <QImage>
#include <QDebug>

extern "C"
{
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

#include "backend/audioDecoder.h"


class MainDecoder : public QThread{
    Q_OBJECT

public:
    explicit MainDecoder();
    ~MainDecoder();
    void setCurrentFile(QString);

private:
    QString currentFile;  //文件路径
    QString currentType;  //文件类型
    qint64 timeTotal; //音视频总时间

    //进度调整
    AVPacket seekPacket;
    qint64 seekPos;
    double seekTime;

    //解协议，解码
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVStream *videoStream;
    AvPacketQueue videoQueue;

     //过滤器
    AVFilterGraph *filterGraph;
    AVFilterContext *filterSinkCxt;
    AVFilterContext *filterSrcCxt;

    int videoIndex;
    int audioIndex;
    int subtitleIndex;

    //标志位
    bool isStop;
    bool isPause;
    bool isSeek;
    bool isReadFinished;

    //同步
    double videoClk; //frame pts

    void run();  //线程执行体
    void clearData(); //清空数据
    void displayVideo(QImage image); //
    int initFilter(); //初始化过滤器
    static int vedioThread(void *arg); //解析vedio的线程函数
    double sync(AVFrame *frame, double pts); //同步函数

public slots:
     void decodeFile(QString,QString);  //读入一个音频文件，开始处理

signals:
     void sign_sendOneFrame(QImage image);
};



#endif // MAINDECODER_H
