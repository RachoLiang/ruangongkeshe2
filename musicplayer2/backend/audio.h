#ifndef AUDIO_H
#define AUDIO_H

#endif // AUDIO_H
#include <QMediaPlayer>
#include "caption.h"
#include <QString>
#include <QList>
#include <QAudioOutput>

//音频类
class audio:public caption
{
public:
    audio() {}
    void setAudioOutput(QAudioOutput ouput);
    audio(QString audioPath);

private:
    QMediaPlayer* player;
    QString audioPath;  //文件路径
    int duration;   //音频时间长度
    QString audioType;  //音频类型

};
