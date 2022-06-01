#include "backend/utils.h"

//从文件路径中获取文件名字
QString getNameFromPath(QString path){
    if (!path.isEmpty()){
        return path.right(path.size() - path.lastIndexOf("\\") - 1);
    } else {
       return "未知";
    }
}

//获取带有时间戳的文件名字
QString getNameByTime(QString filename){
    QString format = "-";
    QString und = "未知";
    if (!filename.isEmpty()) {
        return filename + format + getNowTimeStr();
    } else{
        return und + format + getNowTimeStr();
    }
}

//获取当前时间的字符串
QString getNowTimeStr(){
    QDateTime date = QDateTime::currentDateTime();
    return date.toString("yyyy-MM-dd-hh-mm-ss");
}

//获取视频媒体类型信息,返回一个Video对象
Video* getVideoInfo(QString path){
    AVFormatContext * m_avFormatCtx = NULL;
    int m_videoStreamIndex = -1;
    int m_audioStreamIndex = -1;
    int res;

    if((res = avformat_open_input(&m_avFormatCtx,path.toStdString().c_str(),0,NULL))<0){
        qDebug()<<"getMediaInfo::open file fail";
        return NULL;
    }

    if(avformat_find_stream_info(m_avFormatCtx,0)<0){
        qDebug()<<"getMediaInfo::find stream info fail";
        return NULL;
    }

    //av_dump_format(m_avFormatCtx,0,path.toLatin1().data(),0); //查看format信息，debug用

    //获取各个流轨道
    for (int i = 0; i < m_avFormatCtx->nb_streams; i++)
    {
        AVStream *in_stream = m_avFormatCtx->streams[i];

        if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_videoStreamIndex = i;

        }
        else if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_audioStreamIndex = i;
        }
    }

    if(m_videoStreamIndex>=0&&m_audioStreamIndex>=0){  //video
        AVStream *videoStream = m_avFormatCtx->streams[m_videoStreamIndex];
        AVStream *audioStream = m_avFormatCtx->streams[m_audioStreamIndex];

        Video *video = new Video(path);

        //获取视频分辨率
        video->setResolvingPower(QString("%1x%2").arg(videoStream->codecpar->width).arg(videoStream->codecpar->height));
        //获取视频码率
        video->setVideoBitRate(QString("%1kbps").arg(videoStream->codecpar->bit_rate/1000));
        //获取视频帧率
        video->setVideoFrameRate(QString("%1fps").arg(videoStream->avg_frame_rate.num/videoStream->avg_frame_rate.den));
        qDebug()<<video->getVideoFrameRate();
        //获取视频容器格式
        video->setMediaType(m_avFormatCtx->iformat->long_name);

        //总时间 单位s
        video->setDuration(m_avFormatCtx->duration/AV_TIME_BASE);
        //音频码率
        video->setAudioBitRate(QString("%1kbps").arg(audioStream->codecpar->bit_rate/1000));
        //声道数
        video->setNumberOfChannels(audioStream->codecpar->channels);
        //采样率
        video->setSampleRate(QString("%1Hz").arg(audioStream->codecpar->sample_rate));
         //获取视频编码类型
//        AVCodecContext *avctx;
//        AVCodec *codec;
//        avctx = avcodec_alloc_context3(NULL);
//        avcodec_parameters_to_context(avctx,m_avFormatCtx->streams[m_videoStreamIndex]->codecpar);
//        // 寻找视频解码器
//        codec = (AVCodec*)avcodec_find_decoder(avctx->codec_id);
//        if(codec == NULL)
//            qDebug()<<"nono";

//        qDebug()<<codec->name; //视频编码器名称

        return video;

    }else if(m_videoStreamIndex>=0){  //无音频流
        Video *video = new Video(path);
        AVStream *videoStream = m_avFormatCtx->streams[m_videoStreamIndex];

        //获取视频分辨率
        video->setResolvingPower(QString("%1x%2").arg(videoStream->codecpar->width).arg(videoStream->codecpar->height));
        //获取视频码率
        video->setVideoBitRate(QString("%1kbps").arg(videoStream->codecpar->bit_rate/1000));
        //获取视频帧率
        video->setVideoFrameRate(QString("%1fps").arg(videoStream->avg_frame_rate.num/videoStream->avg_frame_rate.den));
        qDebug()<<video->getVideoFrameRate();
        //获取视频容器格式
        video->setMediaType(m_avFormatCtx->iformat->long_name);

        //总时间 单位s
        video->setDuration(m_avFormatCtx->duration/AV_TIME_BASE);

        return video;
    }else{
        qDebug()<<"非视频格式";
        return NULL;
    }
}

Audio* getAudioInfo(QString path){
    AVFormatContext * m_avFormatCtx = NULL;
    int m_audioStreamIndex = -1;
    int res;

    if((res = avformat_open_input(&m_avFormatCtx,path.toLatin1().data(),0,NULL))<0){
        qDebug()<<"getMediaInfo::open file fail";
        return NULL;
    }

    if(avformat_find_stream_info(m_avFormatCtx,0)<0){
        qDebug()<<"getMediaInfo::find stream info fail";
        return NULL;
    }

    //av_dump_format(m_avFormatCtx,0,path.toLatin1().data(),0); //查看format信息，debug用

    //获取音频流轨道
    for (int i = 0; i < m_avFormatCtx->nb_streams; i++)
    {
        AVStream *in_stream = m_avFormatCtx->streams[i];
        if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_audioStreamIndex = i;
        }
    }

    if(m_audioStreamIndex>=0){
        Audio* audio = new Audio(path);
        AVStream *audioStream = m_avFormatCtx->streams[m_audioStreamIndex];

        //总时间 单位s
        audio->setDuration(m_avFormatCtx->duration/AV_TIME_BASE);
        //音频码率
        audio->setAudioBitRate(QString("%1kbps").arg(audioStream->codecpar->bit_rate/1000));
        //声道数
        audio->setNumberOfChannels(audioStream->codecpar->channels);
        //采样率
        audio->setSampleRate(QString("%1Hz").arg(audioStream->codecpar->sample_rate));
        audio->setMediaType(m_avFormatCtx->iformat->long_name);
        return audio;
    }else{
        qDebug()<<"getAudioInfo::非音频格式";
        return NULL;
    }
}
