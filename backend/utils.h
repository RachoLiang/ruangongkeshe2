#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDateTime>
#include <QMediaPlayer>
#include "vedio.h"
#include <QMediaMetaData>

//从文件路径中获取文件名字
QString getNameFromPath(QString path);

//从文件路径中提取相关的媒体信息

//获取带有时间戳的文件名字
QString getNameByTime(QString filename);

//获取当前时间的字符串
QString getNowTimeStr();

//获取视频媒体类型信息
void getMeidaInfo(QString path,Audio* audio);


#endif // UTILS_H
