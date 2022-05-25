#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QThread>
#include <QDebug>
#include <QImage>

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

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}


class VideoDecoder:public QThread{
    Q_OBJECT

public:
    explicit VideoDecoder();
    ~VideoDecoder();

    //flags
    bool isGetFrame; //判读是否读帧

    QString currentFile;
    int videoIndex;
    qint64 total_time;
    qint64 pos;  //解封装解码的位置，即缩略图的位置

    void start_thread(); //用于开启线程
    void getFrame(double p); //用于设置flag和位置,参数是播放比例

private:
    int seekType;

    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVStream *videoStream;

    //filter
    AVFilterGraph   *filterGraph;
    AVFilterContext *filterSinkCxt;
    AVFilterContext *filterSrcCxt;

    int initFilter();
    void run();

signals:
    void sign_sendOneFrame(QImage image);
};

#endif // VIDEODECODER_H
