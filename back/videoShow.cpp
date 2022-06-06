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
    m_process = 0.0;
    m_playState = MainDecoder::STOP;
    m_leftTime = "00:00";
    m_rightTime = "00:00";
    m_title = "";
    m_album = "";
    m_artist = "";
    m_imagePath = "";

    pointNum = 10;
    shakeList = {0,0,0,0,0,0,0,0,0,0};
    barList = {0,0,0,0,0,0,0,0,0,0};
    qDebug()<<"创建了VideoShow对象";
    maindecoder = new MainDecoder();
    maindecoder->setCurrentFile(path);
    reversedecoder = nullptr;
    isReverse = false;
    updateProgressThread = nullptr;
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage*)),this,SLOT(slot_getOneFrame(QImage*)));
    //连接maindecoder -- videoShow: 播放状态改变
    connect(maindecoder,SIGNAL(sign_playStateChanged(MainDecoder::PlayState)),this,SLOT(slot_setPlayState(MainDecoder::PlayState))
            );
    //专辑信息
    connect(maindecoder,SIGNAL(sign_sendAlbumImage(QString)),this,SLOT(slot_getAlbumImage(QString)));
    //发信号给解析器解析视频
    maindecoder->decoderFile(path,"video");
}

VideoShow::VideoShow():nWidth(200),nHeight(400){
    //参数初始化
    lastVolume = 0;
    m_process = 0.0;
    pointNum = 10;
    shakeList = {0,0,0,0,0,0,0,0,0,0};
    barList = {0,0,0,0,0,0,0,0,0,0};
    m_playState = MainDecoder::STOP;
    m_title = "";
    m_album = "";
    m_artist = "";
    m_imagePath = "";

    isReverse = false;
    reversedecoder = nullptr;
    updateProgressThread = nullptr;
    maindecoder = new MainDecoder();
    //开始解析视频
    connect(maindecoder,SIGNAL(sign_sendOneFrame(QImage)),this,SLOT(slot_getOneFrame(QImage)));
    connect(maindecoder,SIGNAL(sign_playStateChanged(MainDecoder::PlayState)),this,SLOT(slot_setPlayState(MainDecoder::PlayState)));
    connect(maindecoder,SIGNAL(sign_sendAlbumImage(QString)),this,SLOT(slot_getAlbumImage(QString)));
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

    int second_left = 0;
    int minute_left = 0;
    int hour_left = 0;
    int second_right = 0;
    int minute_right = 0;
    int hour_right = 0;

    QString zero = "0";

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
            SDL_Delay(100);
            continue;
        }

        //播放状态
        if ((!videoShow->isReverse && state == MainDecoder::PLAYING) || (videoShow->isReverse && videoShow->reversedecoder->getState() == ReverseDecoder::PLAYING)) {
            double nowtime = videoShow->getNowProcess();
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

            //计算时间的秒、分、时
            second_left = nowTime_int % 60;
            minute_left = int(nowTime_int / 60) % 60;
            hour_left = int(nowTime_int / (60 * 60));
            second_right = totalTime_int % 60;
            minute_right = int(totalTime_int / 60) % 60;
            hour_right = int(totalTime_int / (60 * 60));


            QPoint point(5,1);
            QString str_left;
            QString str_right;
            QTextStream(&str_left) << (hour_left < 10 ? "0" : "") << hour_left << ":" << (minute_left < 10 ? "0" : "") << minute_left << ":" << (second_left < 10 ? "0" : "") << second_left;
            QTextStream(&str_right) << (hour_right < 10 ? "0" : "") << hour_right << ":" << (minute_right < 10 ? "0" : "") << minute_right << ":" << (second_right < 10 ? "0" : "") << second_right;


//            QString leftTime = QVariant(ceil(nowTime_int / 60)).toString() + QVariant(":").toString()+QVariant(nowTime_int % 60).toString();
//            QString rightTime = QVariant(ceil(totalTime_int / 60)).toString() + QVariant(":").toString()+QVariant(totalTime_int % 60).toString();
            videoShow->setLeftTime(str_left);
            videoShow->setRightTime(str_right);
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

//获取专辑图片路径
void VideoShow::slot_getAlbumImage(QString imagePath){
    setTitle(maindecoder->title);
    setArtist(maindecoder->artist);
    setAlbum(maindecoder->album);
    imagePath = QString("file:///") + imagePath;
    setImagePath(imagePath);
    //发出信号
    emit titleChanged(m_title);
    emit albumChanged(m_album);
    emit artistChanged(m_artist);
    emit imagePathChanged(imagePath);
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
            av_usleep(100000);  //sleep 100ms
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

//获取波形图数据
QList<int> VideoShow::getBarList(){
    if(maindecoder == nullptr){
        return barList;
    }
    int nowPoint = maindecoder->getAudioDb();
    //队头出列
    barList.removeFirst();
    //数据入列
    barList.append(nowPoint);
    return barList;
}

QList<int> VideoShow::getShakeList(){
    if(maindecoder == nullptr){
        return shakeList;
    }
    int nowPoint = maindecoder->getAudioDb();
    //队头出列
    shakeList.removeFirst();
    //数据入列
    shakeList.append(nowPoint);
    return shakeList;
}

int VideoShow::getPointNum(){
    return pointNum;
}

//专辑信息
QString VideoShow::getTitle(){
    return m_title;
}

QString VideoShow::getAlbum(){
    return m_album;
}

QString VideoShow::getArtist(){
    return m_artist;
}

QString VideoShow::getImagePath(){
    return m_imagePath;
}

void VideoShow::setTitle(QString title){
    m_title = title;
}

void VideoShow::setAlbum(QString album){
    m_album = album;
}

void VideoShow::setArtist(QString artist){
    m_artist = artist;
}

void VideoShow::setImagePath(QString imagePath){
    m_imagePath = imagePath;
}

//清空专辑信息
void VideoShow::clearAlbum(){
    setTitle("");
    setArtist("");
    setAlbum("");
    setImagePath("");
    //发出信号
    emit titleChanged(m_title);
    emit albumChanged(m_album);
    emit artistChanged(m_artist);
    emit imagePathChanged("");
}
/*
功能：启动倒放模块
参数：视频文件名
*/
void VideoShow::reverse(QString filename)
{
    isReverse = false;      //先设置成false，否则控制进度条的线程会因为reversedecoder被设置成nullptr而报错

    //释放上一次倒放的资源
    if (reversedecoder)
    {
        reversedecoder->stop();

        //所有线程退出再delete
        while (!(reversedecoder->isRunFinished() && reversedecoder->isPlayThreadFinished()))    
            av_usleep(100000);  //sleep 100ms
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
