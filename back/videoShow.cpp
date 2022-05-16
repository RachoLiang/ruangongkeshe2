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
    if(maindecoder != nullptr){
        maindecoder->setCurrentFile(path);
    }
    //发信号给mainDecoder解析视频

}

//构造函数
VideoShow::VideoShow(QString path):nWidth(200),nHeight(400){
    sourPath = path;
    qDebug()<<"创建了VideoShow对象";
    maindecoder = new MainDecoder();
    maindecoder->setCurrentFile(path);
    //开始解析视频
    connect(maindecoder,&MainDecoder::sign_SendOneFrame,this,&VideoShow::slot_getOneFrame);
    //发信号给解析器解析视频
}

//析构函数
VideoShow::~VideoShow(){
    qDebug()<<"销毁mainDecoder解析器";
    //销毁解析器的缓存数据
}

//信号槽函数
void VideoShow::slot_getOneFrame(QImage *image){
    if(image){
        //复制图片
        this->image = image->copy();
        qDebug()<<"获取一张图片";
        delete image;
        update();
    }
}

//绘制图片在组件上
void VideoShow::paint(QPainter * painter){
    //图片不为空
    if(!this->image.isNull()){
        qDebug()<<"绘制图片";
        painter->drawImage(QRect(0,0,nWidth,nHeight),image);
    }
}



