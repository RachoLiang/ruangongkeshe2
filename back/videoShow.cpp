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

//构造函数
VideoShow::VideoShow(QString path):nWidth(200),nHeight(400){
    sourPath = path;
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

    maindecoder = new MainDecoder();
    sourPath = "C:\\Users\\xgy\\Desktop\\mp3_test\\test.mp4";
    //maindecoder->setCurrentFile(sourPath);
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage)),this,SLOT(slot_getOneFrame(QImage)));
    //发信号给解析器解析视频
    maindecoder->decoderFile(sourPath,"video");
//    image.load("D:\\mediaPicture\\0.png");
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




