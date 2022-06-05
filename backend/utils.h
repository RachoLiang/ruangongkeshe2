#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDateTime>
#include <QMediaPlayer>
#include "vedio.h"
#include <QMediaMetaData>

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

//从文件路径中获取文件名字
QString getNameFromPath(QString path);

//提取路径前缀
QString getPathByName(QString name);

//从文件路径中提取相关的媒体信息

//获取带有时间戳的文件名字
QString getNameByTime(QString filename);

//获取当前时间的字符串
QString getNowTimeStr();

//获取视频媒体类型信息
Video* getVideoInfo(QString path);

//获取音频媒体类型信息
Audio* getAudioInfo(QString path);


#endif // UTILS_H
