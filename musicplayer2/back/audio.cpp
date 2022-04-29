#include "backend/audio.h"

audio::audio(QString audioPath){
    this->audioPath = audioPath;
    //加载文件
    //初始化媒体信息
}

void audio::setAudioOutput(QAudioOutput* output){
    if(this->player != nullptr){
        this->player->setAudioOutput(output);
    }
}
