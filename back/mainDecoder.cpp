#include "backend/mainDecoder.h"
#include <QCoreApplication>

MainDecoder::MainDecoder() :
    timeTotal(0),
    playState(STOP),
    isStop(false),
    isPause(false),
    isSeek(false),
    isReadFinished(false),
    isFast(false),
    isSlow(false),
    isCut(false),
    isFilterChanged(false),
    audioDecoder(new AudioDecoder),
    filterGraph(NULL),
    seekTime(5 * AV_TIME_BASE),
    seekFrames(5),
    seekPos(0),
    seekType(AVSEEK_FLAG_BACKWARD),
    keyNum(0),
    contrast(1),
    gotStop(false),
    brightness(0),
    saturation(1),
    album(""),
    title(""),
    artist("")
{
    av_init_packet(&seekPacket);
    seekPacket.data = (uint8_t *)"FLUSH";

    QString rootPath = QCoreApplication::applicationDirPath();

    //设置截图路径和缓存路径，默认值
    cutPath = rootPath + "\\cutPicture";
    cachePath = rootPath + "\\cache";
    imgFormat = ".png";

    connect(audioDecoder, SIGNAL(playFinished()), this, SLOT(audioFinished()));
    connect(this, SIGNAL(readFinished()), audioDecoder, SLOT(readFileFinished()));
}

MainDecoder::~MainDecoder()
{

}

QString MainDecoder::getCutPath(){
    return cutPath;
}
void MainDecoder::setCutPath(QString cutPath){
    this->cutPath = cutPath;
}

QString MainDecoder::getImgFmt(){
    return imgFormat;
}

void MainDecoder::setImgFmt(QString imgFmt){
    this->imgFormat = imgFmt;
}

//设置文件路径-->后续改为设置Audio对象
void MainDecoder::setCurrentFile(QString path){
    srcPath = path;
    //后续解码操作
}

void MainDecoder::clearData()
{
    videoIndex = -1,
    audioIndex = -1,
    subtitleIndex = -1,

    timeTotal = 0;

    isStop  = false;
    isPause = false;
    isSeek  = false;
    isReadFinished      = false;
    isDecodeFinished    = false;

    videoQueue.empty();

    audioDecoder->emptyAudioData();

    videoClk = 0;
}

void MainDecoder::setPlayState(MainDecoder::PlayState state)
{
    qDebug() << "Set state: " << state;
    emit sign_playStateChanged(state);
    playState = state;
}

bool MainDecoder::isRealtime(AVFormatContext *pFormatCtx)
{
    if (!strcmp(pFormatCtx->iformat->name, "rtp")
        || !strcmp(pFormatCtx->iformat->name, "rtsp")
        || !strcmp(pFormatCtx->iformat->name, "sdp")) {
         return true;
    }

    if(pFormatCtx->pb && (!strncmp(pFormatCtx->url, "rtp:", 4)
        || !strncmp(pFormatCtx->url, "udp:", 4)
        )) {
        return true;
    }

    return false;
}

int MainDecoder::initFilter()
{
    int ret;

    out = avfilter_inout_alloc();
    in = avfilter_inout_alloc();
    /* output format */
    enum AVPixelFormat pixFmts[] = {AV_PIX_FMT_RGB32, AV_PIX_FMT_NONE};

    /* free last graph */
    if (filterGraph) {
        avfilter_graph_free(&filterGraph);
    }

    filterGraph = avfilter_graph_alloc();

    /* just add filter ouptut format rgb32,
     * use for function avfilter_graph_parse_ptr()
     */
    QString filter = QString("pp=hb/vb/dr/al,eq=contrast=%1:brightness=%2:saturation=%3").arg(this->contrast).arg(this->brightness).arg(this->saturation);
    qDebug()<<"filter"<<filter;

    QString args = QString("video_size=%1x%2:pix_fmt=%3:time_base=%4/%5:pixel_aspect=%6/%7")
            .arg(pCodecCtx->width).arg(pCodecCtx->height).arg(pCodecCtx->pix_fmt)
            .arg(videoStream->time_base.num).arg(videoStream->time_base.den)
            .arg(pCodecCtx->sample_aspect_ratio.num).arg(pCodecCtx->sample_aspect_ratio.den);

    /* create source filter */
    ret = avfilter_graph_create_filter(&filterSrcCxt, avfilter_get_by_name("buffer"), "in", args.toLocal8Bit().data(), NULL, filterGraph);
    if (ret < 0) {
        qDebug() << "avfilter graph create filter failed, ret:" << ret;
        avfilter_graph_free(&filterGraph);
        goto out;
    }

    /* create sink filter */
    ret = avfilter_graph_create_filter(&filterSinkCxt, avfilter_get_by_name("buffersink"), "out", NULL, NULL, filterGraph);
    if (ret < 0) {
        qDebug() << "avfilter graph create filter failed, ret:" << ret;
        avfilter_graph_free(&filterGraph);
        goto out;
    }

    /* set sink filter ouput format */
    ret = av_opt_set_int_list(filterSinkCxt, "pix_fmts", pixFmts, AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        qDebug() << "av opt set int list failed, ret:" << ret;
        avfilter_graph_free(&filterGraph);
        goto out;
    }

    out->name       = av_strdup("in");
    out->filter_ctx = filterSrcCxt;
    out->pad_idx    = 0;
    out->next       = NULL;

    in->name       = av_strdup("out");
    in->filter_ctx = filterSinkCxt;
    in->pad_idx    = 0;
    in->next       = NULL;

    if (filter.isEmpty() || filter.isNull()) {
        /* if no filter to add, just link source & sink */
        ret = avfilter_link(filterSrcCxt, 0, filterSinkCxt, 0);
        if (ret < 0) {
            qDebug() << "avfilter link failed, ret:" << ret;
            avfilter_graph_free(&filterGraph);
            goto out;
        }
    } else {
        /* add filter to graph */
        ret = avfilter_graph_parse_ptr(filterGraph, filter.toLatin1().data(), &in, &out, NULL);
        if (ret < 0) {
            qDebug() << "avfilter graph parse ptr failed, ret:" << ret;
            avfilter_graph_free(&filterGraph);
            goto out;
        }
    }

    /* check validity and configure all the links and formats in the graph */
    if ((ret = avfilter_graph_config(filterGraph, NULL)) < 0) {
        qDebug() << "avfilter graph config failed, ret:" << ret;
        avfilter_graph_free(&filterGraph);
    }

out:
    avfilter_inout_free(&out);
    avfilter_inout_free(&in);

    return ret;
}

void MainDecoder::decoderFile(QString file, QString type)
{
//    qDebug() << "Current state:" << playState;
    qDebug() << "File name:" << file << ", type:" << type;

    //重新初始化参数
    gotStop = false;

    filePath = file;

    if (playState != STOP && playState != FINISH) {
        isStop = true;
        while (playState != STOP && playState != FINISH) {
            SDL_Delay(10);
        }
        SDL_Delay(100);
    }

    clearData();

    SDL_Delay(100);

    currentFile = file;
    currentType = type;


    qDebug()<<"开始解码："<<file;
    this->start();
}

//音频播放结束时调用
void MainDecoder::audioFinished()
{
    isStop = true;
    qDebug()<<"音频播放结束！";
    qDebug()<<"当前类型:"<<currentType;
//    if (currentType == "music") {
//        qDebug()<<"调用了结束接口";
//        SDL_Delay(100);
//        setPlayState(MainDecoder::FINISH);
//        emit sign_playStateChanged(MainDecoder::FINISH);
//    }
}

void MainDecoder::stopVideo()
{
    qDebug()<<"调用了stopAudio";
    if (playState == STOP) {
        setPlayState(MainDecoder::STOP);
        return;
    }

    gotStop = true;
    isStop  = true;
    audioDecoder->stopAudio();

    if (currentType == "video") {
        /* wait for decoding & reading stop */
        while (!isReadFinished || !isDecodeFinished) {
            SDL_Delay(10);
        }
    } else {
        while (!isReadFinished) {
            SDL_Delay(10);
        }
    }
}

void MainDecoder::pauseVideo()
{
    qDebug() << "leftbutton.Decoder::pauseVideo()";
    if (playState == STOP || playState == FINISH) {
        qDebug() << "leftbutton.Decoder::pauseVideo()...stop";
        return;
    }

    isPause = !isPause;
    audioDecoder->pauseAudio(isPause);
    if (isPause) {
        av_read_pause(pFormatCtx);
        setPlayState(PAUSE);
    } else {
        av_read_play(pFormatCtx);
        setPlayState(PLAYING);
    }
}

int MainDecoder::getVolume()
{
    return audioDecoder->getVolume();
}

void MainDecoder::setVolume(int volume)
{
    audioDecoder->setVolume(volume);
}

void MainDecoder::setSpeed(double speed){
    if(audioDecoder){
        audioDecoder->setSpeed(speed);
    }
}

double MainDecoder::getSpeed(){
    if(audioDecoder){
        return audioDecoder->getSpeed();
    }
    return 0.0;
}

void MainDecoder::setSeekTime(qint8 time){
    seekTime = time;
}

qint8 MainDecoder::getSeekTime(){
    return seekTime;
}

void MainDecoder::seekFast(){
    qDebug()<<"快进";
    //简单防止冲突
    if(!isSlow){
        isFast = true;
    }
    qDebug()<<"isFast:"<<isFast;
}

void MainDecoder::seekSlow(){
    //简单防止冲突
    if(!isFast){
        isSlow = true;
    }
}

void MainDecoder::cutOff(){
    isCut = true;
}

int MainDecoder::getAudioDb(){
    return audioDecoder->getPcmDb();
}

qint64 MainDecoder::getTotalTime(){
    //video:以视频流为主
//    if (currentType == "video"){
//        return timeTotal;
//    }else{
//        return audioDecoder->totalTime;
//    }
    return audioDecoder->totalTime;
}

qint64 MainDecoder::getNowTime(){
//    //video:以视频流为主
//    if (currentType == "video"){
//        return nowTime;
//    }else {
//        return audioDecoder->nowTime;
//    }
    return audioDecoder->nowTime;
}

double MainDecoder::getCurrentTime()
{
    if (audioIndex >= 0) {
        return audioDecoder->getAudioClock();
    }

    return 0;
}

MainDecoder::PlayState MainDecoder::getPlayState(){
    return playState;
}

void MainDecoder::seekProgress(double pos)
{
    if (!isSeek) {
        //百分比 * 总时长 * 微秒
//        qDebug()<<"pos:"<<pos;
//        qDebug()<<"totalTime:"<<audioDecoder->totalTime;
        seekPos = pos * audioDecoder->totalTime;
        isSeek = true;
    }
}

double MainDecoder::synchronize(AVFrame *frame, double pts)
{
//    double delay;

    if (pts != 0) {
        videoClk = pts; // Get pts,then set video clock to it
    } else {
        pts = videoClk; // Don't get pts,set it to video clock
    }

    double repeat_pict = frame->repeat_pict;

    double frame_delay = av_q2d(pCodecCtx->time_base);

    double fps = 1/frame_delay;

    double extra_delay = repeat_pict /(2*fps);
    double delay = extra_delay + frame_delay;

    videoClk += delay;
    return pts;
//    delay = av_q2d(pCodecCtx->time_base);
//    delay += frame->repeat_pict * (delay * 0.5);

//    videoClk += delay;

//    return pts;
}

int MainDecoder::videoThread(void *arg)
{

    qDebug()<<"进入解码线程---------";
    int ret;
    double pts;
    AVPacket packet;
    MainDecoder *decoder = (MainDecoder *)arg;
    AVFrame *pFrame  = av_frame_alloc();

    //同步
    double last_delay = 0; //上一次播放的两帧时间差；
    double last_play = 0; //上一帧播放的时间
    double start_time = av_gettime() / 1000000.0; //从第一帧开始的绝对时间

    while (true) {
        if (decoder->isStop) {
            break;
        }

        if (decoder->isPause) {
            SDL_Delay(10);
            continue;
        }

        if (decoder->videoQueue.queueSize() <= 0) {
            /* while video file read finished exit decode thread,
             * otherwise just delay for data input
             */
            if (decoder->isReadFinished) {
                qDebug()<<"------------------------------";
                break;
            }
            SDL_Delay(1);
            continue;
        }

        decoder->videoQueue.dequeue(&packet, true);


        /* flush codec buffer while received flush packet */
        if (packet.data != nullptr && !strcmp((char *)packet.data, "FLUSH")) {
            qDebug() << "Seek video";
            avcodec_flush_buffers(decoder->pCodecCtx);
            av_packet_unref(&packet);
            continue;
        }

        ret = avcodec_send_packet(decoder->pCodecCtx, &packet);
        if ((ret < 0) && (ret != AVERROR(EAGAIN)) && (ret != AVERROR_EOF)) {
            qDebug() << "Video send to decoder failed, error code: " << ret;
            av_packet_unref(&packet);
            continue;
        }

        // raw yuv
        ret = avcodec_receive_frame(decoder->pCodecCtx, pFrame);

        if ((ret < 0) && (ret != AVERROR_EOF)) {
            qDebug() << "Video frame decode failed, error code: " << ret;
            av_packet_unref(&packet);
            continue;
        }

        if( (pFrame->key_frame == 1)
                || (pFrame->pict_type == AV_PICTURE_TYPE_I)){
            decoder->keyNum = decoder->keyNum +1;
//            qDebug()<<"关键帧数量："<<decoder->keyNum;
        }

        //获取pts
        if ((pts = pFrame->pts) == AV_NOPTS_VALUE) {
                pts = 0;
        }
        // 音视频同步:关键
        double play = pts * av_q2d(decoder->videoStream->time_base);
        //纠正时间
        play = decoder->synchronize(pFrame,play);


        if (decoder->audioIndex >= 0) {
              double delay = play - last_play;
              if(delay <=0 || delay >1){
                  delay = last_delay;
              }

              double audioClk = decoder->audioDecoder->getAudioClock() ;
              last_delay = delay;
              last_play = play;

              //音频和视频的时间差
              double diff = decoder->videoClk - audioClk;

              //判断是否在合理范围
              double sync_threshold = (delay>0.01?0.01:delay);
              if(fabs(diff) > 0.08){
                  if(diff <= -sync_threshold){
                      delay = 0; //这里代表视频比音频慢了
                  }else if(diff >= sync_threshold){
                      delay = 2*delay;
                  }
              }
              start_time += delay;
              if(delay == 0 ){  //表示视频比音频慢了，选择性丢帧
                  //qDebug()<<"视频比音频慢了";
                  if( (pFrame->key_frame == 1)
                          || (pFrame->pict_type == AV_PICTURE_TYPE_I)){
                      //qDebug()<<"是关键帧";
                  }else{
                      //qDebug()<<"丢帧了";
                      continue; //丢帧
                  }
              }
              double actual_delay = start_time - av_gettime()/1000000.0;
              if(actual_delay < 0.01){
                  actual_delay = 0.01;
              }

              av_usleep(actual_delay * 1000000.0 + 6000);
        }

        if(decoder->isFilterChanged){
            decoder->isFilterChanged = false;
            decoder->initFilter();
        }

        //过滤

        if (av_buffersrc_add_frame(decoder->filterSrcCxt, pFrame) < 0) {
            qDebug() << "av buffersrc add frame failed.";
            av_packet_unref(&packet);
            continue;
        }

        if (av_buffersink_get_frame(decoder->filterSinkCxt, pFrame) < 0) {
            qDebug() << "av buffersrc get frame failed.";
            av_packet_unref(&packet);
            continue;
        } else {
            //到底如何得到一帧图片并渲染？ qt,
//            qDebug()<<"调用了上下文";
            if(decoder->pCodecCtx == nullptr){
                qDebug()<<"图片渲染时上下文被释放";
                break;
            }
            QImage tmpImage(pFrame->data[0], decoder->pCodecCtx->width, decoder->pCodecCtx->height, QImage::Format_RGB32);
            /* deep copy, otherwise when tmpImage data change, this image cannot display */
            QImage image = tmpImage.copy();
            //判断是否截图
            if(decoder->isCut){
                qDebug()<<"进行截图";
                //提取文件名字
                QString fileName = getNameFromPath(decoder->filePath,decoder->getImgFmt());
                //生成文件名字
                fileName = getNameByTime(fileName,decoder->getImgFmt());
                if (!decoder->cutPath.isEmpty()){
                    //生成保存路径
                    QString savePath = decoder->cutPath + QString("\\") + fileName;
                    qDebug()<<"图片保存路径："<<savePath;
                    image.save(savePath);
                } else {
                    //生成保存路径
                    QString savePath = getPathByName(decoder->filePath) + fileName;
                    qDebug()<<"图片保存路径："<<savePath;
                    image.save(savePath);
                }
                decoder->isCut = false;
            }
            //发送图片信号
            emit decoder->sign_sendOneFrame(image);
        }

        av_frame_unref(pFrame);
        av_packet_unref(&packet);
    }

    av_frame_free(&pFrame);

    if (!decoder->isStop) {
        decoder->isStop = true;
    }

    qDebug() << "Video decoder finished.";

    SDL_Delay(100);

    decoder->isDecodeFinished = true;

    if (decoder->gotStop) {
        qDebug()<<"gotStop:true";
        decoder->setPlayState(MainDecoder::STOP);
    } else {
        qDebug()<<"gotStop里完成！";
        decoder->setPlayState(MainDecoder::FINISH);
    }

    return 0;
}


void MainDecoder::run()
{
    AVCodec *pCodec = NULL;

    AVPacket pkt, *packet = &pkt;        // packet use in decoding

    int seekIndex;
    bool realTime;

    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, currentFile.toLocal8Bit().data(), NULL, NULL) != 0) {
        qDebug() << "Open file failed.";
        return ;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        qDebug() << "Could't find stream infomation.";
        avformat_free_context(pFormatCtx);
        return;
    }

    realTime = isRealtime(pFormatCtx);

//    av_dump_format(pFormatCtx, 0, 0, 0);  // just use in debug output

    /* find video & audio stream index */
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            qDebug() << "Find video stream.";
        }

        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioIndex = i;
            qDebug() << "Find audio stream.";
        }

        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            subtitleIndex = i;
            qDebug() << "Find subtitle stream.";
        }
    }

    if (currentType == "video") {
        if (videoIndex < 0) {
            qDebug() << "Not support this video file, videoIndex: " << videoIndex << ", audioIndex: " << audioIndex;
            avformat_free_context(pFormatCtx);
            return;
        }
    } else {
        if (audioIndex < 0) {
            qDebug() << "Not support this audio file.";
            avformat_free_context(pFormatCtx);
            return;
        }
    }

    if (!realTime) {
        emit gotVideoTime(pFormatCtx->duration);
        timeTotal = pFormatCtx->duration;
    } else {
        emit gotVideoTime(0);
    }
//    qDebug() << timeTotal;

    if (audioIndex >= 0) {
        // 打开音频解码器：入口，注册回调函数
        if (audioDecoder->openAudio(pFormatCtx, audioIndex) < 0) {
            avformat_free_context(pFormatCtx);
            return;
        }
    }

    if (currentType == "video") {
        /* find video decoder */
        pCodecCtx = avcodec_alloc_context3(NULL);
        avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);

        /* find video decoder */
        if((AVCodec*)avcodec_find_decoder(pCodecCtx->codec_id) == NULL){
            qDebug()<<"2空指针";
        }
        if (pCodecCtx == nullptr || (pCodec = (AVCodec*)avcodec_find_decoder(pCodecCtx->codec_id)) == NULL) {
            qDebug() << "Video decoder not found.";
            goto fail;
        }

        // 打开视频解码器
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
            qDebug() << "Could not open video decoder.";
            goto fail;
        }

        videoStream = pFormatCtx->streams[videoIndex];

        if (initFilter() < 0) {
            goto fail;
        }

        SDL_CreateThread(&MainDecoder::videoThread, "video_thread", this);

    }else{
        //提取音频封面
        AVDictionaryEntry* tag = NULL;

        while((tag = av_dict_get(pFormatCtx->metadata,"",tag,AV_DICT_IGNORE_SUFFIX))){
            qDebug()<<tag->key<<":"<<tag->value;
            //保存信息
            if(strcmp("album",tag->key) == 0){
                album = tag->value;
            }else if(strcmp("title",tag->key) == 0){
                title = tag->value;
            }else if(strcmp("artist",tag->key) == 0){
                artist = tag->value;
            }
        }
        //读取专辑封面信息
        if (pFormatCtx->iformat->read_header(pFormatCtx) < 0){
            qDebug()<<"没有header format";
        }

        for(int i = 0;i < pFormatCtx->nb_streams; i++){
            if (pFormatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                AVPacket pkt = pFormatCtx->streams[i]->attached_pic;
                //使用QImage读取图片
                QImage img = QImage::fromData((uchar*)pkt.data,pkt.size);
                //获取专辑封面缓存路径
                QString savePath = cachePath + QString("\\") + getNameFromPath(filePath,imgFormat);
                albumImagePath = savePath;
                qDebug()<<"封面专辑路径："<<savePath;
                img.save(savePath);
                emit sign_sendAlbumImage(albumImagePath);
            }
        }
    }


    setPlayState(MainDecoder::PLAYING);

    while (true) {
        if (isStop) {
            break;
        }

        /* do not read next frame & delay to release cpu utilization */
        if (isPause) {
            SDL_Delay(10);
            continue;
        }

//快进
fast:
        if (isFast) {
            qDebug()<<"真正快进";
            //计算当前应该跳转的位置,并执行后面的seek代码
            if(currentType == "video"){
                seekPos = audioDecoder->nowTime + seekFrames * av_q2d(pCodecCtx->time_base) * AV_TIME_BASE;
            }else {
                seekPos = audioDecoder->nowTime + seekFrames * av_q2d(audioDecoder->getTimeBase()) * AV_TIME_BASE;
            }
            qDebug()<<"快进前时间："<<audioDecoder->nowTime;
            qDebug()<<"快进后时间："<<seekPos;

            if (seekPos > audioDecoder->totalTime){
                seekPos = timeTotal;
            }
            //参数设置
            isFast = false;
            isSeek = true;
            seekType = AVSEEK_FLAG_BACKWARD;
        }

//快退
slow:
        if (isSlow) {
            qDebug()<<"快退";
            //计算当前应该跳转的位置,并执行后面的seek代码
            if(currentType == "video"){
                seekPos = audioDecoder->nowTime - seekFrames * av_q2d(pCodecCtx->time_base) * AV_TIME_BASE;
            }else {
                seekPos = audioDecoder->nowTime - seekFrames * av_q2d(audioDecoder->getTimeBase()) * AV_TIME_BASE;
            }
            if (seekPos < 0){
                seekPos = 0;
            }
            isSlow = false;
            isSeek = true;
            seekType = AVSEEK_FLAG_BACKWARD;
        }

/* this seek just use in playing music, while read finished
 * & have out of loop, then jump back to seek position
 */
seek:
        if (isSeek) {
            if (currentType == "video") {
                seekIndex = videoIndex;
            } else {
                seekIndex = audioIndex;
            }

            nowTime = seekPos;

            AVRational avRational = av_get_time_base_q();
            qDebug()<<"缩放之前的seekPos:"<<seekPos;
            qint64 seekPos_mil = seekPos;
            seekPos = av_rescale_q(seekPos, avRational, pFormatCtx->streams[seekIndex]->time_base);
            if (av_seek_frame(pFormatCtx, seekIndex, seekPos, seekType) < 0) {
                qDebug() << "Seek failed.";
            } else {
                audioDecoder->emptyAudioData();
                audioDecoder->packetEnqueue(&seekPacket);

                if (currentType == "video") {
                    videoQueue.empty();
                    videoQueue.enqueue(&seekPacket);
                    videoClk = 0;
                }

            }

            //isSeek = false;
            seekType = AVSEEK_FLAG_BACKWARD;

            int skip = 0;

            while (true) {
                    qDebug()<<"进入while循环";

                    /* judge haven't reall all frame */

                    if (av_read_frame(pFormatCtx, packet) < 0){
                        if(nowTime + 0.5 * AV_TIME_BASE >= audioDecoder->totalTime){
                            qDebug() << "Read file completed.";
                            isReadFinished = true;
                            emit readFinished();
                            SDL_Delay(10);
                            break;
                        }
                    }

                    if (packet->stream_index == seekIndex){
                        //记录当前帧时间
                        qint64 frameTime = packet->pts * av_q2d(pFormatCtx->streams[seekIndex]->time_base) * AV_TIME_BASE;

                        qDebug()<<"当前帧："<<frameTime;
                        qDebug()<<"解码帧："<<seekPos_mil;
                        //如果当前帧时间小于seekTime，则不解码
                        if(seekPos_mil > frameTime){
                            qDebug()<<"当前帧："<<frameTime;
                            qDebug()<<"解码帧："<<seekPos_mil;
                            av_packet_unref(packet);
//                            skip++;
                            continue;
                        }
                        qDebug()<<"成功啦！";
                        qDebug()<<audioDecoder->packetQueue.queueSize();
                        SDL_Delay(100);
                        isSeek = false;

                        if (packet->stream_index == videoIndex && currentType == "video") {
                            videoQueue.enqueue(packet); // video stream
                        } else if (packet->stream_index == audioIndex) {
                            audioDecoder->packetEnqueue(packet); // audio stream
                        } else if (packet->stream_index == subtitleIndex) {
                            av_packet_unref(packet);    // subtitle stream
                        } else {
                            av_packet_unref(packet);
                        }
                        break;
                    }
                }
            isSeek = false;

        }else{
            if (currentType == "video") {
                if (videoQueue.queueSize() > 512) {
                    SDL_Delay(10);
                    continue;
                }
            }

            /* judge haven't reall all frame */
            if (av_read_frame(pFormatCtx, packet) < 0){
                if(audioDecoder->nowTime + 0.5 * AV_TIME_BASE >= audioDecoder->totalTime){
                    qDebug() << "Read file completed.";
                    isReadFinished = true;
                    emit readFinished();
                    SDL_Delay(10);
                    break;
                }
            }



            if (packet->stream_index == videoIndex && currentType == "video") {
                videoQueue.enqueue(packet); // video stream
            } else if (packet->stream_index == audioIndex) {
                audioDecoder->packetEnqueue(packet); // audio stream
            } else if (packet->stream_index == subtitleIndex) {
                av_packet_unref(packet);    // subtitle stream
            } else {
                av_packet_unref(packet);
            }
        }
    }

    while (!isStop) {
        /* just use at audio playing */
        if (isSeek) {
            goto seek;
        }
        if (isFast){
            goto fast;
        }

        if (isSlow){
            goto slow;
        }

        SDL_Delay(100);
    }

fail:
    /* close audio device */
    if (audioIndex >= 0) {
        qDebug()<<"释放音频上下文";
        audioDecoder->closeAudio();
    }

    if (currentType == "video") {
        qDebug()<<"释放了上下文";
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);
    }

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);

    isReadFinished = true;

    if (currentType == "music") {
        setPlayState(MainDecoder::FINISH);
    }


    qDebug() << "Main decoder finished.";
}

//暂停信息
bool MainDecoder::pauseState(){
    return isPause;
}

void MainDecoder::setFilter(double contrast,double brightness,double saturation){
    isFilterChanged = true;
    if(contrast!=-22)
    this->contrast = contrast;
    if(brightness!=-22)
    this->brightness = brightness;
    if(saturation!=-22)
    this->saturation = saturation;
}
