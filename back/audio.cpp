#include "backend/audio.h"

Audio::Audio(QString audioPath){
    this->filePath = audioPath;
    //加载文件
    //初始化媒体信息
}

void Audio::setAudioOutput(QAudioOutput* output){
    if(this->player != nullptr){
        this->player->setAudioOutput(output);
    }
}
