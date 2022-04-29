#ifndef VEDIO_H
#define VEDIO_H

#endif // VEDIO_H

#include <QMediaPlayer>
#include "caption.h"
#include <QVideoWidget>
#include <QVideoSink>

//视频类
class vedio:public caption,QVideoSink
{
public:
    vedio(){}
    vedio(QString vedioPath);
    bool setVideoOutput(QVideoWidget);
private:
    QMediaPlayer* player;
    QVideoSink* sink;
    QString vedioPath;  //文件路径
    int duration;   //视频时间长度
    QString vedioType;  //视频媒体类型

};
