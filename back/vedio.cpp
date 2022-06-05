#include "backend/vedio.h"


//void Video::setVideoOutput(QVideoWidget* output){
//    if(this->player != nullptr){
//        this->player->setVideoOutput(output);
//    }
//}

QString Video::getVideoBitRate() const
{
    return videoBitRate;
}

void Video::setVideoBitRate(QString newVideoBitRate)
{
    videoBitRate = newVideoBitRate;
}

QString Video::getVideoFrameRate() const
{
    return videoFrameRate;
}

void Video::setVideoFrameRate(QString newVideoFrameRate)
{
    videoFrameRate = newVideoFrameRate;
}


QString Video::getResolvingPower() const
{
    return resolvingPower;
}

void Video::setResolvingPower(QString newResolvingPower)
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
    this->resolvingPower = "";
}
