#include "backend/vedio.h"


void Video::setVideoOutput(QVideoWidget* output){
    if(this->player != nullptr){
        this->player->setVideoOutput(output);
    }
}

int Video::getVideoBitRate() const
{
    return videoBitRate;
}

void Video::setVideoBitRate(int newVideoBitRate)
{
    videoBitRate = newVideoBitRate;
}

int Video::getVideoFrameRate() const
{
    return videoFrameRate;
}

void Video::setVideoFrameRate(int newVideoFrameRate)
{
    videoFrameRate = newVideoFrameRate;
}

const QString &Video::getVideoType() const
{
    return videoType;
}

void Video::setVideoType(const QString &newVideoType)
{
    videoType = newVideoType;
}

int Video::getResolvingPower() const
{
    return resolvingPower;
}

void Video::setResolvingPower(int newResolvingPower)
{
    resolvingPower = newResolvingPower;
}

//通过文件路径初始化
Video::Video(QString vedioPath):Audio(vedioPath)
{
    //判断资源是否可获取
    isValid();
    //加载文件
    //初始化媒体信息
}
