#include "backend/records.h"

void records::setVedioPath(QString vedioPath){
    this->setVedioPath(vedioPath);
}
void records::setVedioText(QString vedioText){
    this->setVedioText(vedioText);
}
bool records::setProgress(double progress){
    if(progress > 0 && progress <= 1){
        this->setProgress(progress);
        return true;
    }else {
        return false;
    }
}
void records::setPlayTime(double playTime){
    this->setPlayTime(playTime);
}
