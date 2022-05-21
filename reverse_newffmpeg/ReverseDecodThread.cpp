//#include "..\stdafx.h"
#include "ReverseDecodThread.h"

ReverseDecodThread::ReverseDecodThread()
{
    //注册解码器
    //av_register_all();

    //清空队列
    video_pack.clear();

    qDebug() << "FFMPEG版本信息:" << av_version_info();
}

ReverseDecodThread::~ReverseDecodThread()
{
    FreeRAM();
    if (codec_ctx)
    {
        avcodec_free_context(&codec_ctx);
    }

    if (format_ctx)
    {
        avformat_close_input(&format_ctx);//释放解封装器的空间，以防空间被快速消耗完
        avformat_free_context(format_ctx);
    }
}

void ReverseDecodThread::FreeRAM()
{
    if (SRC_VIDEO_pFrame) av_frame_free(&SRC_VIDEO_pFrame);
    if (RGB24_pFrame) av_frame_free(&RGB24_pFrame);
    if (img_convert_ctx)sws_freeContext(img_convert_ctx);
    if (out_buffer_rgb)av_free(out_buffer_rgb);

    SRC_VIDEO_pFrame = nullptr;
    RGB24_pFrame = nullptr;
    img_convert_ctx = nullptr;
    out_buffer_rgb = nullptr;
}


/*
工程: FFMPE_ReversePlay
日期: 2021-04-06
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 加载媒体文件
*/
int ReverseDecodThread::LoadVideoFile(QString media)
{
    //释放空间
    FreeRAM();
    strncpy(m_MediaFile, media.toUtf8().data(), sizeof(m_MediaFile));

    QFileInfo info(media);

    //得到后缀
    file_suffix=info.suffix();
    qDebug()<<"file_suffix:"<<file_suffix;


    //分配上下文
    format_ctx = avformat_alloc_context();
    if (avformat_open_input(&format_ctx, m_MediaFile, nullptr, nullptr) != 0)
    {
        LogSend(tr("无法打开视频文件: %1").arg(m_MediaFile));
        return -1;
    }

    //读取媒体文件的数据包以获取流信息
    if (avformat_find_stream_info(format_ctx, nullptr) < 0)
    {
        LogSend(tr("无法获取流信息.\n"));
        return -1;
    }

    LogSend(tr("视频中流的数量: %1\n").arg(format_ctx->nb_streams));
    for (int i = 0; i < format_ctx->nb_streams; ++i)
    {
        stream = format_ctx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            //查找解码器
            if (!(codec_ctx = avcodec_alloc_context3(NULL)))
            {
                LogSend(tr("无法获取codec context.\n"));
                return -1;
            }

            if (avcodec_parameters_to_context(codec_ctx, stream->codecpar) < 0)
            {
                LogSend(tr("avcodec_parameters_to_context失败.\n"));
                return -1;
            }

            if (!(codec = (AVCodec*)avcodec_find_decoder(codec_ctx->codec_id)))
            {
                LogSend(tr("codec not found.\n"));
                return -1;
            }

            //打开解码器
            if (avcodec_open2(codec_ctx, codec, nullptr) != 0)
            {
                LogSend(tr("解码器打开失败.\n"));
                return -1;
            }

            video_stream_index = i;
            //得到视频帧的宽高
            video_width = stream->codecpar->width;
            video_height = stream->codecpar->height;

            LogSend(tr("视频帧的尺寸(以像素为单位): (宽X高)%1x%2 像素格式: %3\n").arg(
                stream->codecpar->width).arg(stream->codecpar->height).arg(stream->codecpar->format));
            break;
        }
    }

    if (video_stream_index == -1)
    {
        LogSend("没有检测到视频流.\n");
        return -1;
    }

    AVRational frameRate = format_ctx->streams[video_stream_index]->avg_frame_rate;

    /*设置视频转码器*/
    SRC_VIDEO_pFrame = av_frame_alloc();
    RGB24_pFrame = av_frame_alloc();// 存放解码后YUV数据的缓冲区

    //将解码后的YUV数据转换成RGB24
    img_convert_ctx = sws_getContext(video_width, video_height,
        codec_ctx->pix_fmt, video_width, video_height,
        AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr);

    //计算RGB图像所占字节大小
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, video_width, video_height, 1);

    //申请空间存放RGB图像数据
    out_buffer_rgb = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    // avpicture_fill函数将ptr指向的数据填充到picture内,但并没有拷贝,只是将picture结构内的data指针指向了ptr的数据
    av_image_fill_arrays(RGB24_pFrame->data, RGB24_pFrame->linesize, out_buffer_rgb, AV_PIX_FMT_RGB24, video_width, video_height, 1);

    over_pack.clear();
    video_pack.clear();

    //每当加载新媒体的时候这间隔时间就清0
    m_DifferTime = DEFAULT_DIFFER_TIME;

    //初始值给视频的最大值
    m_oldPosMs = format_ctx->duration;

    return 0;
}


void ReverseDecodThread::SetSate(int run)
{
    m_run = run;
}

int ReverseDecodThread::GetSate()
{
    return m_run;
}


//跳转视频帧  传入的pos是毫秒单位
void ReverseDecodThread::SetSeekPos(qint64 pos)
{
    //初始值给视频的最大值
    m_oldPosMs = format_ctx->duration;
    is_CurrentSeekPos = 1;
    m_seek=m_n64CurrentSeekPos = pos / 1000.0;  //变成秒单位
    over_pack.clear();
    qDebug() << "**************倒放解码:SetSeekPos**************:" << pos << "," << m_seek;
}


void ReverseDecodThread::PausePlay()
{
    qDebug() << "**************倒放解码:PausePlay**************";
    emit ss_ReversePlayFinish();
    m_run = 2;
}

void ReverseDecodThread::StopPlay()
{
    qDebug() << "**************倒放解码:StopPlay**************";
    emit ss_ReversePlayFinish();
    m_run = 0;
}

void ReverseDecodThread::LogSend(QString text)
{
    qDebug() << text;
}

qint64 ReverseDecodThread::GetDuration()
{
    //format_ctx->duration *1.0 / AV_TIME_BASE  //得到秒
    return format_ctx->duration / 1000;  //得到毫秒
}

//线程执行起点
void ReverseDecodThread::run()
{
    LogSend("开始播放视频.\n");
    StartPlay();
}

//解码数据包
int ReverseDecodThread::DecodDataPack()
{
    //判断上次是否还有没有播放完的包
    if (over_pack.size() > 0)
    {
        for (int i = 0; i < over_pack.size(); i++)
        {
            video_pack.append(over_pack.at(i));
        }
    }

    if (video_pack.size() >= 2)
    {
        //如果等于默认时间
        if (m_DifferTime == DEFAULT_DIFFER_TIME)
        {
            //计算间隔时间
            qint32 m_tmp = (video_pack.at(1).video_clock - video_pack.at(0).video_clock) * 1000;
            if(m_tmp>0)
            {
                //重新更新间隔时间
                m_DifferTime =m_tmp;
                qDebug() << "两帧相差的时间:" << m_DifferTime;
            }
        }
    }

    if (video_pack.size() > 0)
    {
        int i = 0;
        int cnt_i=1;
        if(file_suffix=="mpg")cnt_i=8;
        for (i = video_pack.size() - 1; i > cnt_i; i--)
        {
            //时间信号-秒转为毫秒
            qint64 pos_ms = video_pack.at(i).video_clock * 1000;

            //因为是倒放.下一帧的时间肯定要比上一帧小,如果大于说明帧有问题不能显示
            if (pos_ms<m_oldPosMs)
            {
                //通知界面更新
                SendOneFrame(video_pack.at(i).image.copy());

                //更新时间
                m_oldPosMs = pos_ms;
                positionChanged(pos_ms);
                //同步画面,看起来差不多
                //QThread::msleep(40);
                QThread::msleep(m_DifferTime);

                //int delay_time_cnt = 0;
                //while (true)
                //{
                //	if (m_run != 1)break;//如果状态不是运行状态
                //	delay_time_cnt++;
                //	QThread::msleep(1);
                //	if (delay_time_cnt >= m_DifferTime/2)break; //时间到达
                //}
            }
            //qDebug() << "m_oldPosMs:" << m_oldPosMs<<"_pos_ms:"<< pos_ms;

            //如果状态不是运行状态
            if (m_run != 1)
            {
                break;
            }
        }
        if (i != 0)
        {
            over_pack.clear();
            //保存没有播放完的包
            for (int j = 0; j <= i; j++)
            {
                over_pack.append(video_pack.at(j));
            }
        }
        video_pack.clear();

        //判断是否是因为需要跳转才退出循环的
        if (is_CurrentSeekPos)
        {
            video_pack.clear();
            over_pack.clear();
        }
    }
    return 0;
}

//播放视频
int ReverseDecodThread::StartPlay()
{
    ////默认从视频结尾开始播放
    //m_n64CurrentSeekPos=format_ctx->duration *1.0 / AV_TIME_BASE;

    //m_endSeekPos=m_n64CurrentSeekPos;
    ////向后偏移2秒
    //m_n64CurrentSeekPos-=2;
    ////偏移到指定位置再开始解码    AVSEEK_FLAG_BACKWARD 向后找最近的关键帧
    //av_seek_frame(format_ctx, -1, m_n64CurrentSeekPos* AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);
    //seek_state=1;

    //表示视频加载成功
    while (m_run)
    {
        if (m_run == 2)
        {
            // qDebug()<<"暂停状态.";
            msleep(100); //暂停播放
            continue;
        }

        if (is_CurrentSeekPos)
        {
            is_CurrentSeekPos = 0;


            //向后偏移1秒
            m_endSeekPos = m_seek;

            int64_t seek_val = m_seek * AV_TIME_BASE;

            //偏移到指定位置再开始解码    AVSEEK_FLAG_BACKWARD 向后找最近的关键帧
            av_seek_frame(format_ctx, -1, seek_val, AVSEEK_FLAG_BACKWARD);

            seek_state = 1;
            //清空队列
            video_pack.clear();
            over_pack.clear();
            qDebug() << "外部触发seek:" << seek_val<<",m_seek:"<< m_seek;
        }

        double video_clock;
        AVPacket pkt;
        int ret;

        //读取一帧数据
        if (av_read_frame(format_ctx, &pkt) < 0)
        {
            //m_run = 2; //设置为暂停状态
            //解码数据包
            DecodDataPack();

            m_endSeekPos = m_n64CurrentSeekPos;

            //向后偏移1秒
            m_n64CurrentSeekPos -= 1;
            //偏移到指定位置再开始解码    AVSEEK_FLAG_BACKWARD 向后找最近的关键帧

            qint64 seek_val = m_n64CurrentSeekPos * AV_TIME_BASE;
            av_seek_frame(format_ctx, -1, seek_val, AVSEEK_FLAG_BACKWARD);
            seek_state = 1;

            //qDebug() << "完成一段解码.";
          //  QThread::sleep(5);
            qDebug() << "数据读取完毕.";
            continue;
        }

        if (pkt.stream_index == video_stream_index)
        {
            //当前时间
            video_clock = av_q2d(format_ctx->streams[video_stream_index]->time_base) * pkt.pts;

            //说明之前偏移过
            if (seek_state)
            {
                m_n64CurrentSeekPos = video_clock;
                seek_state = 0;
            }

            //解码视频 frame
            //发送视频帧
            if ((ret = avcodec_send_packet(codec_ctx, &pkt)) != 0)
            {
                qDebug() << ret;
                av_packet_unref(&pkt);//不成功就释放这个pkt
                continue;
            }
            //接受后对视频帧进行解码
            if ((ret = avcodec_receive_frame(codec_ctx, SRC_VIDEO_pFrame)) != 0)
            {
                qDebug() << ret;
                av_packet_unref(&pkt);//不成功就释放这个pkt
                continue;
            }


            //转格式
            sws_scale(img_convert_ctx,
                (uint8_t const **)SRC_VIDEO_pFrame->data,
                SRC_VIDEO_pFrame->linesize, 0, video_height, RGB24_pFrame->data,
                RGB24_pFrame->linesize);

            //释放包
            av_packet_unref(&pkt);

            //加载图片数据
            QImage image(out_buffer_rgb, video_width, video_height, QImage::Format_RGB888);

            struct IMAGE_FRAME image_frame;
            image_frame.image = image.copy();
            image_frame.video_clock = video_clock;

            //添加到队列
            video_pack.append(image_frame);

            //解码到结尾
            if (video_clock >= m_endSeekPos+1)
            {
                //读取到最开头的一帧数据
                if (video_pack.size() == 1)
                {
                    if (video_pack.at(0).video_clock <= 1.5)
                    {
                        qDebug() << "读取到最开头的一帧数据:" << video_pack.at(0).video_clock;
                        //解码数据包
                        DecodDataPack();
                        m_run = 2; //设置为暂停状态
                        continue; //回到循环头继续
                    }
                }

                //解码数据包
                DecodDataPack();

                m_endSeekPos = m_n64CurrentSeekPos;

                //向后偏移10秒
                m_n64CurrentSeekPos -= 5;
                //偏移到指定位置再开始解码    AVSEEK_FLAG_BACKWARD 向后找最近的关键帧

                qint64 seek_val = m_n64CurrentSeekPos * AV_TIME_BASE;
                av_seek_frame(format_ctx, -1, seek_val, AVSEEK_FLAG_BACKWARD);
                seek_state = 1;

                qDebug() << "完成一段解码.";
              //  QThread::sleep(5);
            }
        }
    }

    LogSend("视频音频解码播放器的线程退出成功.\n");
    return 0;
}

