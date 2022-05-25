#include "backend/thumbnailShow.h"
#include <QPainter>


ThumbnailShow::ThumbnailShow(){
    this->videoDecoder = new VideoDecoder();
    connect(videoDecoder,SIGNAL(sign_sendOneFrame(QImage)),this,SLOT(slot_getOneFrame(QImage)));
    videoDecoder->start_thread(); //线程开始
    setPathAndStart("C:\\Users\\YYg\\Desktop\\test2.mp4");
}

ThumbnailShow::~ThumbnailShow(){

}


void ThumbnailShow::slot_getOneFrame(QImage tmp){
    image = tmp.copy();
    update();
}

void ThumbnailShow::paint(QPainter * painter){
    //图片不为空
    if(!this->image.isNull()){
        painter->drawImage(QRect(0,0,nWidth,nHeight),image);
    }
}

void ThumbnailShow::setPathAndStart(QString path){
    videoDecoder->currentFile = path;
    videoDecoder->start_thread();
}

void ThumbnailShow::getFrame(double percent){
    videoDecoder->getFrame(percent);
}

//框
int ThumbnailShow::getHeight(){
    return nHeight;
}

int ThumbnailShow::getWidth(){
    return nWidth;
}


void ThumbnailShow::setWidth(int width){
    this->nWidth = width;
}

void ThumbnailShow::setHeight(int height){
    this->nHeight = height;
}
