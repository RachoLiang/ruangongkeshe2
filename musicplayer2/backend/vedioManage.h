#ifndef VEDIOMANAGE_H
#define VEDIOMANAGE_H

#endif // VEDIOMANAGE_H

#include "audio.h"
#include "vedio.h"
#include <QString>
#include <QList>
#include <QThread>

//视频列表管理类
class vedioManage:public vedio
{
private:
    QList<vedio> vedioList;     //音视频列表
    int nowIndex;     //当前播放的视频序号
public:
    vedioManage() {}
};
