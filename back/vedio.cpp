#include "backend/vedio.h"

Video::Video(QString vedioPath){
    this->filePath = vedioPath;
    //加载文件
    if(this->player == nullptr){
        this->player = new QMediaPlayer;
    }
    this->player->setSource(vedioPath);
    //初始化媒体信息
    //将视频加载到缓存中|判断缓存中是否存在视频
}

void Video::setVideoOutput(QVideoWidget* output){
    if(this->player != nullptr){
        this->player->setVideoOutput(output);
    }
}
