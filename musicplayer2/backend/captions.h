#ifndef CAPTIONS_H
#define CAPTIONS_H

#endif // CAPTIONS_H

#include <QString>
#include <QList>

class captions
{
public:
    captions() {}
    captions(QString captionsPath);
    bool readCaptions();     //读取字幕文件
    bool analyzeCaptions();  //解析字幕文本
    void setCaptionsPath(QString captionsPath);  //设置字幕文件路径
    void getCaptionsText();  //获取字幕文本
    void getCaptionsTime();  //获取字幕时间

private:
    QString captionsPath;    //字幕文件路径
    QList<QString> captionsText; //字幕文本
    QList<int> captionsTime;     //字幕对应的播放时间

};
