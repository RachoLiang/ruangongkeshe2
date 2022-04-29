#ifndef RECORDS_H
#define RECORDS_H

#endif // RECORDS_H

#include <QString>
#include <QList>

//播放历史记录类
class records
{
public:
    records() {}
    void setVedioPath(QString vedioPath);
    void setVedioText(QString vedioText);
    bool setProgress(double progress);
    void setPlayTime(double playTime);
private:
    QString vedioPath;  //播放文件路径
    QString vedioText; //音视频标题
    double progress;   //播放进度百分比
    long playTime;    //播放日期的时间戳
};
