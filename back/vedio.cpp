#include "backend/vedio.h"


void Video::setVideoOutput(QVideoWidget* output){
    if(this->player != nullptr){
        this->player->setVideoOutput(output);
    }
}

//通过文件路径初始化
Video::Video(QString vedioPath):Audio(vedioPath)
{
    //判断资源是否可获取
    isValid();
    //加载文件
    //初始化媒体信息
}
