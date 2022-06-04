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

////获取资源的媒体信息
//Audio* VideoShow::getMediaObject(QString path){
//    Audio* audio = nullptr;
//    qDebug()<<"解析的路径:"<<path;
//    getMeidaInfo(path,audio);
//    qDebug()<<"解析完毕！";
//    return audio;
//}

//构造函数
VideoShow::VideoShow(QString path):nWidth(200),nHeight(400){
    sourPath = path;
    m_process = 0.0;
    m_playState = MainDecoder::STOP;
    m_leftTime = "00:00";
    m_rightTime = "00:00";
    qDebug()<<"创建了VideoShow对象";
    maindecoder = new MainDecoder();
    maindecoder->setCurrentFile(path);
    reversedecoder = nullptr;
    isReverse = false;
    updateProgressThread = nullptr;
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage*)),this,SLOT(slot_getOneFrame(QImage*)));
    //连接maindecoder -- videoShow: 播放状态改变
    connect(maindecoder,SIGNAL(sign_playStateChanged(MainDecoder::PlayState)),this,SLOT(slot_setPlayState(MainDecoder::PlayState)));
    //发信号给解析器解析视频
    maindecoder->decoderFile(path,"video");
}

VideoShow::VideoShow():nWidth(200),nHeight(400){
    //参数初始化
    lastVolume = 0;
    m_process = 0.0;
    m_playState = MainDecoder::STOP;
    isReverse = false;
    reversedecoder = nullptr;
    updateProgressThread = nullptr;
    maindecoder = new MainDecoder();
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage)),this,SLOT(slot_getOneFrame(QImage)));
    connect(maindecoder,SIGNAL(sign_playStateChanged(MainDecoder::PlayState)),this,SLOT(slot_setPlayState(MainDecoder::PlayState)));
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
bool VideoShow::isPaused() {
    if (!isReverse && maindecoder) {
        return maindecoder->pauseState();
    }
    else if (isReverse && reversedecoder)
    {
        return reversedecoder->getState() == ReverseDecoder::PAUSE;
    }
    else {
        return true;
    }
}

void VideoShow::pause() {
    if (!isReverse && maindecoder) {
        maindecoder->pauseVideo();
    }
    else if (isReverse && reversedecoder)
    {
        reversedecoder->pause();
    }
}

//Stop控制
bool VideoShow::isStop() {
    if (!isReverse && maindecoder) {
        if (maindecoder->getPlayState() == MainDecoder::STOP || maindecoder->getPlayState() == MainDecoder::FINISH) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (isReverse && reversedecoder)
    {
        return reversedecoder->getState() == ReverseDecoder::STOP;
    }
    else {
        return true;
    }
}

//Finish控制
bool VideoShow::isFinish(){
    if (maindecoder){
        if(maindecoder->getPlayState() == MainDecoder::FINISH){
            return true;
        }else {
            return false;
        }
    }else {
        return false;
    }
}

void VideoShow::stopPlay(){
    if(maindecoder){
        maindecoder->stopVideo();
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
void VideoShow::setProcess(double process) {
    if (!isReverse && maindecoder) {
        maindecoder->seekProgress(process);
    }
    else if (isReverse && reversedecoder)
    {
        reversedecoder->seekBySlider(process);
    }
}

//获取进度条信息
qint64 VideoShow::getNowProcess() {
    if (!isReverse && maindecoder) {
        return maindecoder->getNowTime();
    }
    else if (isReverse && reversedecoder)
    {
        return reversedecoder->getCurrTime();
    }
}

qint64 VideoShow::getTotalProcess() {
    if (!isReverse && maindecoder) {
        return maindecoder->getTotalTime();
    }
    else if (isReverse && reversedecoder)
    {
        return reversedecoder->getTotalTime();
    }
}

double VideoShow::getmProcess()const{
    return m_process;
}

void VideoShow::setmProcess(double process){
    this->m_process = process;
    emit processChanged(process);
}

//进度条时间
QString VideoShow::getLeftTime(){
    return m_leftTime;
}

QString VideoShow::getRightTime(){
    return m_rightTime;
}

void VideoShow::setLeftTime(QString leftTime){
    this->m_leftTime = leftTime;
    emit leftTimeChanged(leftTime);
}

void VideoShow::setRightTime(QString rightTime){
    this->m_rightTime = rightTime;
    emit rightTimeChanged(rightTime);
}


//线程体
//更新实时的进度条百分比
int VideoShow::updateProcess(void *arg){
    //获取当前的videoShow对象
    VideoShow* videoShow = (VideoShow*)arg;
    double last_time = 0.0;

    //进入循环
    while (true){
        MainDecoder::PlayState state = videoShow->getPlayState();
        if ((!videoShow->isReverse && state == MainDecoder::STOP) || (videoShow->isReverse && videoShow->reversedecoder->getState() == ReverseDecoder::STOP)) {
            qDebug()<<"当前状态是Stop";
            //重置参数
            videoShow->setLeftTime("00:00");
            videoShow->setRightTime("00:00");
            //自动调用下一首播放
            SDL_Delay(100);
            continue;
        }

        //当Pause状态时，暂停更新
        if ((!videoShow->isReverse && state == MainDecoder::PAUSE) || (videoShow->isReverse && videoShow->reversedecoder->getState() == ReverseDecoder::PAUSE)) {
//            qDebug()<<"当前状态是暂停";
            SDL_Delay(100);
            continue;
        }

        //播放状态
        if ((!videoShow->isReverse && state == MainDecoder::PLAYING) || (videoShow->isReverse && videoShow->reversedecoder->getState() == ReverseDecoder::PLAYING)) {
            double nowtime = videoShow->getNowProcess();
//            if(nowtime - last_time >  * 1000 * 1000){
//                SDL_Delay(50);
//                continue;
//            }else{
//                if(nowtime>0){
//                    last_time = nowtime;
//                }
//            }
            if(nowtime <= 0){
                SDL_Delay(100);
                continue;
            }else {
                last_time = nowtime;
            }
            double totaltime = videoShow->getTotalProcess();
            double x = nowtime / totaltime;
            //设置进度条未知
            videoShow->setmProcess(x);
            //设置进度条时间
            int nowTime_int = nowtime / 1000000;
            int totalTime_int = totaltime / 1000000;
            QString leftTime = QVariant(ceil(nowTime_int / 60)).toString() + QVariant(":").toString()+QVariant(nowTime_int % 60).toString();
            QString rightTime = QVariant(ceil(totalTime_int / 60)).toString() + QVariant(":").toString()+QVariant(totalTime_int % 60).toString();
            videoShow->setLeftTime(leftTime);
            videoShow->setRightTime(rightTime);
            SDL_Delay(300);
        }

        //播放完成状态
        if(state == MainDecoder::FINISH){
            qDebug()<<"当前状态是finish";
            //重置参数
            videoShow->setLeftTime("00:00");
            videoShow->setRightTime("00:00");
            //自动调用下一首播放
            SDL_Delay(500);
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

MainDecoder::PlayState VideoShow::getmPlayState()const{
    return m_playState;
}

//maindecode发送信号的槽函数，本身不会被前端调用
void VideoShow::slot_setPlayState(MainDecoder::PlayState playState){
    qDebug()<<"videoShow:setPlayState信号槽被调用";
    this->m_playState = playState;
    emit playStateChanged(playState);
}

//播放
void VideoShow::show(QString path, QString type) {
    //设置为正放模式
    if (isReverse)
        isReverse = false;

    //释放倒放模块的资源
    if (reversedecoder)
    {
        reversedecoder->stop();
        while (!(reversedecoder->isRunFinished() && reversedecoder->isPlayThreadFinished()))    //所有线程退出再delete
            ;
        delete reversedecoder;
        reversedecoder = nullptr;
    }

    //清空上次播放的缓存
    if (maindecoder->getPlayState() != MainDecoder::STOP && maindecoder->getPlayState() != MainDecoder::FINISH)
    {
        maindecoder->stopVideo();
        qDebug() << "videoShow的stop";
    }
    sourPath = path;
    maindecoder->decoderFile(path, type);
    //暂时：启动进度条监测
    if (!updateProgressThread)
        updateProgressThread = SDL_CreateThread(&VideoShow::updateProcess, "update_process", this);
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

//启动倒放模块
void VideoShow::reverse(QString filename)
{
    isReverse = false;      //先设置成false，否则控制进度条的线程会因为reversedecoder被设置成nullptr而报错

    //清除上一次倒放的资源
    if (reversedecoder)
    {
        reversedecoder->stop();
        while (!(reversedecoder->isRunFinished() && reversedecoder->isPlayThreadFinished()))    //所有线程退出再delete
            ;
        delete reversedecoder;
        reversedecoder = nullptr;
    }
    stopPlay();
    reversedecoder = new ReverseDecoder(filename);
    isReverse = true;       //当新的reversedecoder准备好后再设置成true
    connect(reversedecoder, &ReverseDecoder::sendOneFrame, this, &VideoShow::slot_getOneFrame);
    if (!updateProgressThread)
        updateProgressThread = SDL_CreateThread(&VideoShow::updateProcess, "update_process", this);
}