#include "backend/audio.h"

//通过文件路径初始化
Audio::Audio(QString audioPath){
    this->filePath = audioPath;
    this->player = new QMediaPlayer;
    this->player->setSource(QUrl::fromLocalFile(this->filePath));
    //判断资源是否可获取
    isValid();
    //加载文件
    //初始化媒体信息
    this->duration = this->player->duration();
}

//通过数据库信息初始化
Audio::Audio(QString audioPath,QString fileName,int duration,int position,QDateTime modificationTime,bool isvalid){
    this->filePath = audioPath;
    this->player = new QMediaPlayer;
    //设置资源路径
    this->player->setSource(QUrl::fromLocalFile(this->filePath));

    this->fileName = fileName;
    this->duration = duration;
    this->position = position;
    this->modificationTime = modificationTime;
    //资源无效则置isValid为false
    this->isvalid = isValid() ? isvalid : false;
}

void Audio::setAudioOutput(QAudioOutput* output){
    if(this->player != nullptr){
        this->player->setAudioOutput(output);
    }
}

void Audio::setPosition(int value){
    if(value < 0){
        return;
    }
    this->position = value;
}

//切换视频播放状态
bool Audio::onPlay(){
    if(isPlaying()){    //播放->暂停
        this->player->pause();
    }else if(isPausing()){   //暂停->播放
        this->player->play();
    }
    return true;
}


//是否在播放
bool Audio::isPlaying(){
    //尚未初始化
    if(this->player == nullptr){
        return false;
    }
    if(this->player->PlayingState == QMediaPlayer::PlayingState){
        return true;
    }else{
        return false;
    }
}

//是否暂停
bool Audio::isPausing(){
    //尚未初始化
    if(this->player == nullptr){
        return false;
    }
    if(this->player->PausedState == QMediaPlayer::PausedState){
        return true;
    }else{
        return false;
    }
}

//资源是否有效
bool Audio::isValid(){
    if(this->player == nullptr){
        return false;
    }
    //判断资源是否有效
    int status = this->player->mediaStatus();
    //不存在资源文件
    if(status == 1){
        this->isvalid = false;
    }
    return this->isvalid;
}

//调节音量
void Audio::adjustVolume(int value){
    if(this->player == nullptr){
        return;
    }
    this->player->audioOutput()->setVolume(value);
}

//调节播放进度
void Audio::adjustProgress(int value){
    if(value < 0){
        return ;
    }
    this->position = value;
    //如果正在播放，则调整进度条
    if(isPlaying()){
        this->player->setPosition(value);
    }
}


//调节播放速率
void Audio::adjustRate(qreal value){
    if(this->player == nullptr){
        return;
    }
    this->player->setPlaybackRate(value);
}

//停止播放
bool Audio::onStop(){
    if(this->player == nullptr){
        return false;
    }
    this->player->stop();
    return true;
}

//获取player对象
QMediaPlayer* Audio::getPlayer(){
    return this->player;
}


