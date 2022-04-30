#include "backend/vedio.h"

Video::Video(QString vedioPath){
    this->filePath = vedioPath;
    //加载文件
    //初始化媒体信息
}

void Video::setVideoOutput(QVideoWidget output){
    if(this->player != nullptr){
        this->player->setVideoOutput(output);
    }
}
