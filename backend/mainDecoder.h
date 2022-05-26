#ifndef MAINDECODER_H
#define MAINDECODER_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include "audioDecoder.h"

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
#include "audio.h"


class MainDecoder : public QThread
{
    Q_OBJECT

public:
    enum PlayState {
        STOP,
        PAUSE,
        PLAYING,
        FINISH
    };

    explicit MainDecoder();
    ~MainDecoder();

    double getCurrentTime();
    void seekProgress(double pos);
    int getVolume();
    void setVolume(int volume);
    void setCurrentFile(QString);
    void pauseVideo();
    bool pauseState();
    double getSpeed();
    void setSpeed(double);
    void setSeekTime(qint8 time);   //设置快进时间段
    qint8 getSeekTime();    //获取快进时间段
    void seekFast();    //快进
    void seekSlow();    //快退
    void cutOff();  //截图
    qint64 getTotalTime();  //获取总时长
    qint64 getNowTime();    //获取当前播放时长
    QString getCutPath();   //获取当前截图路径
    void setCutPath(QString cutPath);   //设置当前截图路径
    PlayState getPlayState();   //获取当前播放状态


    double contrast;
    double brightness;
    double saturation;

    void setFilter(double contrast,double brightness,double saturation);
    int initFilter();

    int keyNum;

private:
    void run();
    void clearData();
    void setPlayState(MainDecoder::PlayState state);
    void displayVideo(QImage image);
    static int videoThread(void *arg);
    double synchronize(AVFrame *frame, double pts);
    bool isRealtime(AVFormatContext *pFormatCtx);

    QString srcPath;    //当前播放的文件路径
    Audio* audio;   //audio对象

    int fileType;

    int videoIndex;
    int audioIndex;
    int subtitleIndex;

    QString currentFile;
    QString currentType;

    qint64 timeTotal;
    qint64 seekTime;    //一次快进|快退跳转的时间段，微秒数为单位
    int seekFrames;     //快进|快退多少帧
    qint64 nowTime; //当前播放的时间

    AVPacket seekPacket;
    qint64 seekPos;

    //播放控制信息
    PlayState playState;
    bool isStop;
    bool gotStop;
    bool isPause;
    bool isSeek;
    bool isReadFinished;
    bool isDecodeFinished;
    bool isFast;    //是否快进
    bool isSlow;    //是否快退
    bool isCut;     //是否截图
    bool isFilterChanged;


    AVFormatContext *pFormatCtx;

    AVCodecContext *pCodecCtx;          // video codec context

    AvPacketQueue videoQueue;
    AvPacketQueue subtitleQueue;

    AVStream *videoStream;

    double videoClk;    // video frame timestamp
    QString filePath;   //文件路径
    QString cutPath;    //截图保存路径，暂时写死

    AudioDecoder *audioDecoder;

    AVFilterGraph   *filterGraph;
    AVFilterContext *filterSinkCxt;
    AVFilterContext *filterSrcCxt;

    AVFilterInOut *out;
    AVFilterInOut *in;

    int seekType;

public slots:
    void decoderFile(QString file, QString type);
    void stopVideo();
    void audioFinished();

signals:
    void sign_sendOneFrame(QImage image);
    void readFinished();
    void gotVideo(QImage image);
    void gotVideoTime(qint64 time);
    void playStateChanged(MainDecoder::PlayState state);

};



#endif // MAINDECODER_H
