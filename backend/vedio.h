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
    void setVideoOutput(QVideoWidget* output);
    void play();
    void pause();
    void adjustProgress(int);
private:
    //QMediaPlayer* player;
    QVideoSink* sink;
    int videoBitRate;     //视频码率
    int videoFrameRate;   //视频帧率
    QString videoType;    //视频类型
    int resolvingPower;   //分辨率

};

#endif // VEDIO_H

