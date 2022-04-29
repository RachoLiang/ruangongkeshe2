#include "backend/vedio.h"

vedio::vedio(QString vedioPath){
    this->vedioPath = vedioPath;
    //加载文件
    //初始化媒体信息
}

void vedio::setVideoOutput(QVideoWidget output){
    if(this->player != nullptr){
        this->player->setVideoOutput(output);
    }
}
