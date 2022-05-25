#include "backend/videoDecoder.h"

VideoDecoder::VideoDecoder(){
    this->isGetFrame = false;
    this->filterGraph = NULL;
}

VideoDecoder::~VideoDecoder(){

}


void VideoDecoder::start_thread(){
    this->start(); //开启线程
}

void VideoDecoder::getFrame(double p){
    this->pos = this->total_time * p;  //算出秒数
    this->isGetFrame = true;
}

int VideoDecoder::initFilter(){
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

void VideoDecoder::run(){
    AVCodec *pCodec;

    AVPacket pkt, *packet = &pkt;        // packet use in decoding
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, currentFile.toLocal8Bit().data(), NULL, NULL) != 0) {
        qDebug() << "thumbnail::Open file failed.";
        return ;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        qDebug() << "thumbnail::Could't find stream infomation.";
        avformat_free_context(pFormatCtx);
        return;
    }

    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            qDebug() << "thumbnail::Find video stream."<<videoIndex;
        }
    }

    if (videoIndex < 0) {
        qDebug() << "thumbnail::Not support this video file, videoIndex: " << videoIndex;
        avformat_free_context(pFormatCtx);
        return;
    }

    total_time = pFormatCtx->duration / AV_TIME_BASE;
    qDebug()<<"total_time"<<total_time;

    pCodecCtx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);

    /* find video decoder */
    if ((pCodec = (AVCodec*)avcodec_find_decoder(pCodecCtx->codec_id)) == NULL) {
        qDebug() << "thumbnail::Video decoder not found.";
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);

        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
    }

    // 打开视频解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        qDebug() << "thumbnail::Could not open video decoder.";
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);

        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
    }

    videoStream = pFormatCtx->streams[videoIndex];

    if (initFilter() < 0) {
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);

        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
    }

    //循环判断是否读帧
    while(true){
        if(!isGetFrame){
            //qDebug()<<"no";
            continue;
        }
        isGetFrame = false;

        //解析指定位置的帧
        AVRational avRational = av_get_time_base_q();
        qDebug()<<pos;
        int pos_mili = pos * AV_TIME_BASE;
        qint64 pos_int64 = av_rescale_q(pos_mili,avRational,pFormatCtx->streams[videoIndex]->time_base);
        if(av_seek_frame(pFormatCtx,videoIndex,pos_int64,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME)<0){
            qDebug()<<"change postion fail!";
            continue;
        }

        while(true){
            //
            if(av_read_frame(pFormatCtx,packet)<0){
                qDebug()<<"read frame fail";
                break;
            }

            if(packet->stream_index == videoIndex){
                //开始解码
                AVFrame *pFrame = av_frame_alloc();
                int ret;
                AVPacket _packet = *packet;

                /* flush codec buffer while received flush packet */
                if (!strcmp((char *)_packet.data, "FLUSH")) {
                    qDebug() << "Seek video";
                    avcodec_flush_buffers(pCodecCtx);
                    av_packet_unref(&_packet);
                    continue;
                }

                ret = avcodec_send_packet(pCodecCtx, &_packet);
                if ((ret < 0) && (ret != AVERROR(EAGAIN)) && (ret != AVERROR_EOF)) {
                    qDebug() << "Video send to decoder failed, error code: " << ret;
                    av_packet_unref(&_packet);
                    continue;
                }
                double packet_time = _packet.pts * av_q2d(pFormatCtx->streams[videoIndex]->time_base);
                //qDebug()<<"packet time"<<packet_time;
                if(fabs(packet_time - pos)>1){
                    qDebug()<<"与原位置相差过大 "<<packet_time<<pos;
                    continue;
                }

                // raw yuv
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                qDebug()<<"ret:"<<ret;

                if ((ret < 0) && (ret != AVERROR_EOF)) {
                    qDebug() << "Video frame decode failed, error code: " << ret;
                    av_packet_unref(&_packet);
                    continue;
                }

                   //使用frame_time判断
//                double frame_time = pFrame->best_effort_timestamp * av_q2d(pFormatCtx->streams[videoIndex]->time_base);
//                qDebug()<<"frame_time"<<frame_time;
//                if(fabs(frame_time - pos)>1){
//                    qDebug()<<"与原位置相差过大 "<<frame_time<<pos;
//                    continue;
//                }

                if (av_buffersrc_add_frame(filterSrcCxt, pFrame) < 0) {
                    qDebug() << "av buffersrc add frame failed.";
                    av_packet_unref(&_packet);
                    continue;
                }

                if (av_buffersink_get_frame(filterSinkCxt, pFrame) < 0) {
                    qDebug() << "av buffersrc get frame failed.";
                    av_packet_unref(&_packet);
                    continue;
                } else {
                    QImage tmpImage(pFrame->data[0], pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
                    QImage image = tmpImage.copy();
                    qDebug()<<"获取到图片";
                    //image.save(QString("C:\\Users\\xgy\\Desktop\\mp3_test\\frame\\frame%1.png").arg(pos));
                    emit sign_sendOneFrame(image);
                }
                //free
                av_frame_unref(pFrame);
                av_packet_unref(&_packet);
                av_frame_free(&pFrame);
                break;
            }else{
                qDebug()<<"非视频流packet";
                av_packet_unref(packet);
            }
        }
    }
}
