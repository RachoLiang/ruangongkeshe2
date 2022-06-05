#ifndef VEDIO_H
#define VEDIO_H

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVideoSink>
#include "audio.h"
//视频类
class Video:public Audio,QVideoSink
{
public:
    Video(){}
    Video(QString vedioPath);
//    void setVideoOutput(QVideoWidget* output);
    void play();
    void pause();
    void adjustProgress(int);
    QString getVideoBitRate() const;
    void setVideoBitRate(QString newVideoBitRate);
    QString getVideoFrameRate() const;
    void setVideoFrameRate(QString newVideoFrameRate);
    QString getResolvingPower() const;
    void setResolvingPower(QString newResolvingPower);

private:
    //QMediaPlayer* player;
    QVideoSink* sink;
    QString videoBitRate;     //视频码率  xxxkbps 千位/s
    QString videoFrameRate;   //视频帧率  xxxfps
    QString resolvingPower;   //分辨率   1920x1080

};

#endif // VEDIO_H

