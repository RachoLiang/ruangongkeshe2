#include "backend/videoShow.h"
#include <QPainter>

//框
int VideoShow::getHeight(){
    return nHeight;
}

int VideoShow::getWidth(){
    return nWidth;
}

void VideoShow::setSize(int width,int height){
    this->nWidth = width;
    this->nHeight = height;
}

void VideoShow::setWidth(int width){
    this->nWidth = width;
}

void VideoShow::setHeight(int height){
    this->nHeight = height;
}

void VideoShow::setSourPath(QString path){
    //mainDecoder内部有video对象，可以通过改变video对象的值来更换mainDecoder
    //解析的值
    this->sourPath = path;
//    if(maindecoder != nullptr){
//        maindecoder->setCurrentFile(path);
//    }
    //发信号给mainDecoder解析视频

}

QString VideoShow::getSourPath(){
    return sourPath;
}

//获取资源的媒体信息
Audio* VideoShow::getMediaObject(QString path){
    Audio* audio = nullptr;
    qDebug()<<"解析的路径:"<<path;
    getMeidaInfo(path,audio);
    qDebug()<<"解析完毕！";
    return audio;
}

//构造函数
VideoShow::VideoShow(QString path):nWidth(200),nHeight(400){
    sourPath = path;
    m_process = 0.0;
    qDebug()<<"创建了VideoShow对象";
    maindecoder = new MainDecoder();
    maindecoder->setCurrentFile(path);
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage*)),this,SLOT(slot_getOneFrame(QImage*)));

    //发信号给解析器解析视频
    maindecoder->decoderFile(path,"video");
}

VideoShow::VideoShow():nWidth(200),nHeight(400){
    //参数初始化
    lastVolume = 0;
    m_process = 0.0;

    maindecoder = new MainDecoder();
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage)),this,SLOT(slot_getOneFrame(QImage)));
}

//析构函数
VideoShow::~VideoShow(){
    qDebug()<<"销毁mainDecoder解析器";
    //销毁解析器的缓存数据
}

//信号槽函数
void VideoShow::slot_getOneFrame(QImage tmp){
//    if(image){
//        //复制图片
//        this->image = *image;
//        qDebug()<<"获取一张图片-----";
//        update();
//    }
    image = tmp.copy();
    update();
}

//绘制图片在组件上
void VideoShow::paint(QPainter * painter){
    //图片不为空
    if(!this->image.isNull()){
        painter->drawImage(QRect(0,0,nWidth,nHeight),image);
    }
}


//音量控制
int VideoShow::getVolume(){
    int volume = 0;
    if(maindecoder){
        volume = maindecoder->getVolume();
        if(volume < 0){
            volume = 0;
        }
    }
    qDebug()<<"调用了获取音量函数";
    return volume;
}

void VideoShow::setVolume(int volume){
    if(maindecoder){
        maindecoder->setVolume(volume);
    }
}

bool VideoShow::isSilence(){
    if(getVolume() <= 0){
        return true;
    }else {
        return false;
    }
}

void VideoShow::silence(){
    if(isSilence()){
        //恢复音量
        setVolume(lastVolume);
    }else{
        //静音，并记录上次的音量
        lastVolume = getVolume();
        setVolume(0);
    }
}

//暂停控制
bool VideoShow::isPaused(){
    if(maindecoder){
        return maindecoder->pauseState();
    }else {
       return true;
    }
}

void VideoShow::pause(){
    if(maindecoder){
        maindecoder->pauseVideo();
    }
}

//倍速播放
void VideoShow::setSpeed(double speed){
    if(maindecoder){
        maindecoder->setSpeed(speed);
    }
}

double VideoShow::getSpeed(){
    if(maindecoder){
        return maindecoder->getSpeed();
    }else{
        return 0;
    }
}

//快进
void VideoShow::seekFast(){
    if(maindecoder){
        maindecoder->seekFast();
    }
}

//快退
void VideoShow::seekSlow(){
    if(maindecoder){
        maindecoder->seekSlow();
    }
}

//调节播放进度
void VideoShow::setProcess(double process){
    if(maindecoder){
        maindecoder->seekProgress(process);
    }
}

//获取进度条信息
qint64 VideoShow::getNowProcess(){
    if(maindecoder){
        return maindecoder->getNowTime();
    }
}

qint64 VideoShow::getTotalProcess(){
    if(maindecoder){
        return maindecoder->getTotalTime();
    }
}

double VideoShow::getmProcess()const{
    return m_process;
}

void VideoShow::setmProcess(double process){
    this->m_process = process;
    emit processChanged(process);
}


//线程体
//更新实时的进度条百分比
int VideoShow::updateProcess(void *arg){
    //获取当前的videoShow对象
    VideoShow* videoShow = (VideoShow*)arg;

    //进入循环
    while (true){
        MainDecoder::PlayState state = videoShow->getPlayState();
        if(state == MainDecoder::STOP){
            qDebug()<<"当前状态是Stop";
//            break;
            SDL_Delay(100);
            continue;
        }

        //当Pause状态时，暂停更新
        if(state == MainDecoder::PAUSE){
            qDebug()<<"当前状态是暂停";
            SDL_Delay(100);
            continue;
        }

        //播放状态
        if(state == MainDecoder::PLAYING){
            //qDebug()<<"当前状态是播放";
            double nowtime = videoShow->getNowProcess();
            double totaltime = videoShow->getTotalProcess();
            double x = nowtime / totaltime;
            //qDebug()<<x*100;
            SDL_Delay(100);
            videoShow->setmProcess(x);
        }

        //播放完成状态
        if(state == MainDecoder::FINISH){
            videoShow->setmProcess(1.0);
        }
    }

    return 0;
}

//获取播放状态
MainDecoder::PlayState VideoShow::getPlayState(){
    if (maindecoder){
        return maindecoder->getPlayState();
    }
}

//播放
void VideoShow::show(QString path, QString type){
    sourPath = path;
    maindecoder->decoderFile(path,type);
    //暂时：启动进度条监测
     SDL_CreateThread(&VideoShow::updateProcess, "update_process", this);
}

//截图功能
void VideoShow::cutOff(){
    maindecoder->cutOff();
}

QString VideoShow::getCutPath(){
    if (maindecoder) {
        return maindecoder->getCutPath();
    }
}

void VideoShow::setCutPath(QString path){
    if (maindecoder) {
        maindecoder->setCutPath(path);
    }
}


//调整播放参数

void VideoShow::setArgs(double contrast_per, double brightness_per, double saturation_per){
    double contrast = contrast_per<0?-22:(-2 + 4 * contrast_per);  //-2.0~2.0
    double brightness = brightness_per<0?-22:(-1 + 2 * brightness_per);  //-1.0~1.0
    double saturation = saturation_per<0?-22:(3 * saturation_per);  //0~3.0

    maindecoder->setFilter(contrast,brightness,saturation);
}


