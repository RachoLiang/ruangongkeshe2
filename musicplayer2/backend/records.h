#ifndef RECORDS_H
#define RECORDS_H

#endif // RECORDS_H

#include <QString>
#include <QList>

class records   //音视频字幕类
{
public:
    records() {}
    records(QString recordsPath);   //初始化字幕文件路径
    bool readRecords();     //读取字幕文件
    bool analyzeRecords();  //解析字幕文本
    void setRecordsPath();  //设置字幕文件路径
    void getRecordsText();  //获取字幕文本
    void getRecordsTime();  //获取字幕时间
private:
    QString recordsPath;    //字幕文件路径
    QList<QString> recordsText; //字幕文本
    QList<int> recordsTime;     //字幕对应的播放时间
};
