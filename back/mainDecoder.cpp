﻿#include "backend/mainDecoder.h"

MainDecoder::MainDecoder():audioDecoder(new AudioDecoder){
    this->isStop = false;
    this->isPause = false;
    this->isSeek = false;
    this->isReadFinished = false;
    this->filterGraph = NULL;

    //音频播放器信号槽
    connect(audioDecoder,SIGNAL(sign_playFinished()),this,SLOT(audioFinished()));
    connect(this,SIGNAL(sign_readFinished()),audioDecoder,SLOT(slot_readFileFinished()));
    //确实播放状态改变槽

}

MainDecoder::~MainDecoder(){

}

void MainDecoder::setCurrentFile(QString path){
    this->currentFile = path;
}

void MainDecoder::run(){
    AVCodec *pCodec;
    AVPacket pkt, *packet=&pkt;

    pFormatCtx = avformat_alloc_context(); //用于打开多媒体流
    if(avformat_open_input(&pFormatCtx,currentFile.toLocal8Bit().data(),NULL,NULL) != 0){  //返回0表示成功，否则失败
        qDebug()<<"open file failed:"<<currentFile;
        avformat_free_context(pFormatCtx); //释放流对象
        return;
    }

    if(avformat_find_stream_info(pFormatCtx,NULL)<0){ //读取媒体文件的数据包，以获取流信息，>=0表示ok
        qDebug()<<"find stream failed";
        avformat_free_context(pFormatCtx); //释放流对象
        return;
    }

    //av_dump_format(pFormatCtx, 0, currentFile.toLocal8Bit().data(), 0);  这个打印到哪里了？

    for(unsigned int i=0;i<pFormatCtx->nb_streams;i++){
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){  //视频流
            videoIndex = i;
            qDebug()<<"find video stream index:"<<i;
        }
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){ //音频流
            audioIndex = i;
            qDebug()<<"find audio stream index:"<<i;
        }
//        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
//            subtitleIndex = i;
//            qDebug()<<"find subtitle stream index:"<<i;
//        }
    }

    //相当于检查音视频的格式，格式错误直接返回
    if(currentType == "video"){
        if(videoIndex<0){
            qDebug()<<"Not support this video file";
            avformat_free_context(pFormatCtx);
            return;
        }
    }else if(currentType == "audio"){
        if(audioIndex<0){
            qDebug()<<"Not support this audio file";
            avformat_free_context(pFormatCtx);
            return;
        }
    }

    //可能有些不能获取到时长
    timeTotal = pFormatCtx->duration;

    //打开音频处理，初始化音频回调函数
    if(audioIndex >= 0){
        //打开失败
        if(audioDecoder->open(pFormatCtx,audioIndex) < 0){
            avformat_free_context(pFormatCtx);
            return ;
        }
    }

    if(currentType == "video"){  //对于视频流，做额外处理（
        pCodecCtx = avcodec_alloc_context3(NULL);
        avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);

        //寻找解码器
        if((pCodec = (AVCodec*)avcodec_find_decoder(pCodecCtx->codec_id)) == NULL){
            qDebug()<<"Video decoder not found";
            goto fail;
        }

        //打开视频解码器,decoder用于后面videoThread对packet进行decode
        if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
            qDebug()<<"Could not open video decoder.";
            goto fail;
        }

        //获取视频流
        videoStream = pFormatCtx->streams[videoIndex];

        //初始化过滤器,用于videoThread中的frame过滤
        if(initFilter()<0){
            goto fail;
        }

        SDL_CreateThread(&MainDecoder::vedioThread,"vedioThread",this); //开启另一个线程去解析vedio并更新到前端
    }

    //下面的操作是read frame到packet,然后将packet加入videoQueue中，队列将作为vedioThread的处理对象
    while(true){
        if(isStop){   //停止，直接break
            break;
        }

        if(isPause){  //
            SDL_Delay(10);
            continue;
        }

seek:
        if(isSeek){
            //todo  定位播放
        }

        if(currentType=="video"){
            if(videoQueue.queueSize() > 512){
                SDL_Delay(10);
                continue;
            }
        }

        if(av_read_frame(pFormatCtx, packet) < 0){  //读取到packet,直到结束
            qDebug()<<"read file completed.";
            isReadFinished = true;
            break;
            //todo
            emit sign_readFinished();

            SDL_Delay(10);
            break;
        }

        if(packet->stream_index == videoIndex && currentType == "video"){
            videoQueue.enqueue(packet);  //video stream，进队列
        }else if(packet->stream_index == audioIndex){
            //todo
            audioDecoder->avpacketEnqueue(packet);
            //audio stream
        }else if(packet->stream_index == subtitleIndex){
            //todo
            //subtitle stream
            av_packet_unref(packet);
        }else{
            av_packet_unref(packet);
        }
    }
    while(!isStop){
        if(isSeek){
            goto seek;
        }

        SDL_Delay(100);
    }

fail:
    /* close audio device */
    if (audioIndex >= 0) {
        audioDecoder->close();
        //todo
    }

    if (currentType == "video") {
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);
    }

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);

    isReadFinished = true;

//    if (currentType == "music") {
//        setPlayState(MainDecoder::STOP);
//    }

    qDebug() << "Main decoder finished.";
}

int MainDecoder::vedioThread(void *arg){  //完成vedio的解析，结果为一帧一帧的图片
    int ret;
    double pts;  //这个时间戳告诉播放器应该在什么时候播放这个帧
    AVPacket packet;
    MainDecoder *decoder = (MainDecoder *)arg;
    AVFrame *pFrame = av_frame_alloc();

    int temp = 0;

    int64_t start_time = av_gettime();
    while(true){
        if(decoder->isStop){   //停止播放
            break;
        }
        if(decoder->isPause){  //
            SDL_Delay(100);
            continue;
        }

        if(decoder->videoQueue.queueSize()<=0){ //队列为空，如果是读完了，直接break,如果还没有读完，delay
            if(decoder->isReadFinished){
                break;
            }
            SDL_Delay(1);
            continue;
        }

        decoder->videoQueue.dequeue(&packet,true);

        if(!strcmp((char*)packet.data,"FLUSH")){
            qDebug()<<"Seek video";
            avcodec_flush_buffers(decoder->pCodecCtx);
            av_packet_unref(&packet);
            continue;
        }

        ret = avcodec_send_packet(decoder->pCodecCtx,&packet);
        if((ret<0) && (ret!=AVERROR(EAGAIN)) && (ret!=AVERROR_EOF)){
            qDebug()<<"Video send to decoder failed";
            av_packet_unref(&packet);
            continue;
        }

        //raw yuv
        ret = avcodec_receive_frame(decoder->pCodecCtx, pFrame);
        if((ret<0)&&(ret!=AVERROR_EOF)){
            qDebug()<<"video frame decode failed";
            av_packet_unref(&packet);
            continue;
        }

        //获取此帧的pts
        if((pts=pFrame->pts)==AV_NOPTS_VALUE){
            pts = 0;
        }

        int64_t realTime = av_gettime()-start_time;
        while(pts > realTime){
            msleep(10);
            realTime = av_gettime()-start_time;
        }

        //音视频同步
        pts *= av_q2d(decoder->videoStream->time_base);
        pts = decoder->sync(pFrame,pts);

        if(decoder->audioIndex >= 0){
            while(1){
                //控制信息
                if(decoder->isStop){
                    break;
                }

                double audioClock = decoder->audioDecoder->getAudioClock();
                pts = decoder->videoClk;

                if(pts <= audioClock){
                    break;
                }
                //延迟时间
                int delayTime = (pts - audioClock) * 1000;

                delayTime = delayTime > 5 ? 5 : delayTime;

                SDL_Delay(delayTime);
            }
        }


        //过滤
        if(av_buffersrc_add_frame(decoder->filterSrcCxt,pFrame)<0){
            qDebug()<<"av buffersrc add frame failed.";
            av_packet_unref(&packet);
            continue;
        }

        if(av_buffersink_get_frame(decoder->filterSinkCxt,pFrame)<0){
            qDebug()<<"av buffersrc get frame failed.";
            av_packet_unref(&packet);
            continue;
        }else{
            QImage tmpImage(pFrame->data[0], decoder->pCodecCtx->width, decoder->pCodecCtx->height, QImage::Format_RGB32);
            /* deep copy, otherwise when tmpImage data change, this image cannot display */
            QImage image = tmpImage.copy();

//            QString path = QString("D:\\mediaPicture\\%1.png").arg(temp);
//            image.save(path);
            temp++;
//            sleep(10);
            qDebug()<<"发送一张图片----------------";
            //SDL_Delay(1000);
            emit decoder->sign_sendOneFrame(image);
//            if(temp >= 2){
//            break;
//            }
            //decoder->displayVideo(image);
        }
    }

    return 0;
}

int MainDecoder::initFilter(){  //过滤器初始化
    int ret;

    AVFilterInOut *out = avfilter_inout_alloc();
    AVFilterInOut *in = avfilter_inout_alloc();
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
    QString filter("pp=hb/vb/dr/al");

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


void MainDecoder::decodeFile(QString file,QString type){
    qDebug()<<"begin file decode:"<<file<<" type:"<<type;

    clearData();

    currentFile = file;
    currentType = type;

    this->start(); //线程开始
}

void MainDecoder::displayVideo(QImage image){
    //todo
}

void MainDecoder::clearData(){
    qDebug()<<"clearData";
    videoIndex = -1;
    audioIndex = -1;
    subtitleIndex = -1;

    timeTotal = 0;

    isStop = false;
    isPause = false;
    isSeek = false;
    isReadFinished = false;

    videoQueue.empty();

    //清空audio数据
    audioDecoder->clearDate();

    videoClk = 0; //时钟清零
}

double MainDecoder::sync(AVFrame *frame, double pts){
    double delay;

    if(pts != 0){
        videoClk = pts;
    }else{
        pts = videoClk;
    }

    delay = av_q2d(pCodecCtx->time_base);
    delay += frame->repeat_pict * (delay * 0.5);

    videoClk += delay;
    return pts;
}

//设置播放状态
void MainDecoder::setPlayState(MainDecoder::PlayState state){
    emit sign_playStateChanged(state);
    qDebug()<<"播放状态改变";
}

//播放状态改变
void MainDecoder::pauseVideo(){
    qDebug()<<"暂停播放:pauseVideo";
    if(playState == STOP){
        qDebug()<<"视频处于Stop状态:pauseVideo";
        return;
    }

    isPause = !isPause;
    //控制音频
    audioDecoder->pause(isPause);
    if(isPause){
        av_read_pause(pFormatCtx);
        setPlayState(PAUSE);
    }else{
        av_read_play(pFormatCtx);
        setPlayState(PLAYING);
    }
}

void MainDecoder::audioFinished(){
    //完成读取
    isStop = true;
    //toConfirm
    if(currentType == "audio"){
        SDL_Delay(100);
        emit sign_playStateChanged(MainDecoder::FINISH);
    }
}

void MainDecoder::stopVideo(){
    if(playState == STOP){
        setPlayState(MainDecoder::STOP);
        return;
    }

    //gotStop
    //toConfirm
    isStop = true;
    audioDecoder->stop();

    if(currentType == "video"){
        //todo
        while(!isReadFinished){
            SDL_Delay(10);
        }
    } else{
        while(!isReadFinished){
            SDL_Delay(10);
        }
    }
}


