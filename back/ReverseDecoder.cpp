#include "backend/ReverseDecoder.h"

using namespace std;

/*
功能：构造函数，初始化
参数：视频文件名
*/
ReverseDecoder::ReverseDecoder(QString file)
{
    setFilename(file);
    state = STOP;
    formatCtx = nullptr;
    videoStream = nullptr;
    codecCtx = nullptr;
    codec = nullptr;
    convertCtx = nullptr;
    video_stream_index = -1;
    video_width = -1;
    video_height = -1;
    srcFrame = nullptr;
    RGBFrame = nullptr;
    RGB_buffer = nullptr;
    seekStart = seekEnd = currTime = duration = -1;
    seekEvent = afterSeek = false;
    interval = 1 / 30;
    decodeFinished = false;
    runFinished = playThreadFinished = true;
    frameListMutex = SDL_CreateMutex();
    playListMutex = SDL_CreateMutex();
    init();
}


/*
功能：析构函数，释放内存
*/
ReverseDecoder::~ReverseDecoder()
{
    freeRAM();
    SDL_DestroyMutex(frameListMutex);
    SDL_DestroyMutex(playListMutex);
    this->deleteLater();
    this->wait();
}

/*
功能：设置视频文件名
参数：QString
*/
void ReverseDecoder::setFilename(QString file)
{
    if (file.size() > sizeof(filename))
    {
        qDebug() << "文件名过长";
        return;
    }
    strncpy(filename, file.toUtf8().data(), sizeof(filename));
}


/*
功能：获取视频文件名
返回：QString
*/
QString ReverseDecoder::getFilename()
{
    return QString(filename);
}

/*
功能：获取播放状态
返回：枚举（int）
*/
int ReverseDecoder::getState()
{
    return state;
}

/*
功能：设置为pause状态
*/
void ReverseDecoder::pause()
{
    if (state == STOP)
        return;
    state = (state == PAUSE) ? PLAYING : PAUSE;
}

void ReverseDecoder::stop()
{
    state = STOP;
    msleep(100);
}

/*
功能：获取当前帧的时间，单位：微秒
返回：qint64
*/
qint64 ReverseDecoder::getCurrTime()
{
    return currTime*1000000;
}

/*
功能：设置当前帧的时间，单位：秒
返回：double
*/
void ReverseDecoder::setCurrTime(double time)
{
    currTime = time;
}

/*
功能：获取总时间，单位：微秒
返回：qint64
*/
qint64 ReverseDecoder::getTotalTime()
{
    return duration*1000000;
}

/*
功能：点击进度条完成跳转
参数：进度条百分比位置
*/
void ReverseDecoder::seekBySlider(double pos)
{
    seekEnd = pos * duration;   //重新设置seekEnd，完成跳转
    seekEvent = true;
    pause();                    //设置为PAUSE，避免继续解析和播放
    SDL_LockMutex(frameListMutex);
    frameList.clear();
    SDL_UnlockMutex(frameListMutex);
    SDL_LockMutex(playListMutex);
    playList.clear();
    SDL_UnlockMutex(playListMutex);
    pause();                    //设置为PLAYING
}

bool ReverseDecoder::isRunFinished()
{
    return runFinished;
}

bool ReverseDecoder::isPlayThreadFinished()
{
    return playThreadFinished;
}

/*
功能：初始化ReverseDecoder，获取AVFormatContext, AVCodec等数据结构
返回：-1:失败   0:成功
*/
int ReverseDecoder::init()
{
    freeRAM();
    formatCtx = avformat_alloc_context();	//分配AVFormatContext

    //打开文件
    if (avformat_open_input(&formatCtx, filename, nullptr, nullptr) != 0)
    {
        qDebug() << "无法打开视频文件：" << filename;
        return -1;
    }

    //获取流信息
    if (avformat_find_stream_info(formatCtx, nullptr) < 0)
    {
        qDebug() << "无法获取流信息";
        return -1;
    }

    //找视频流，只倒放视频流
    for (int i = 0; i < formatCtx->nb_streams; i++)
    {
        videoStream = formatCtx->streams[i];
        if (videoStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            //获取codec
            if (!(codecCtx = avcodec_alloc_context3(NULL)))
            {
                qDebug() << "AVCodecContext内存分配失败";
                return -1;
            }

            if (avcodec_parameters_to_context(codecCtx, videoStream->codecpar) < 0)
            {
                qDebug() << "无法获取AVCodecContext";
                return -1;
            }

            if (!(codec = (AVCodec*)avcodec_find_decoder(codecCtx->codec_id)))
            {
                qDebug() << "解码器不存在";
                return -1;
            }

            //打开codec
            if (avcodec_open2(codecCtx, codec, nullptr) != 0)
            {
                qDebug() << "解码器打开失败";
                return -1;
            }

            video_stream_index = i;
            video_width = codecCtx->width;
            video_height = codecCtx->height;
            break;
        }

        if (video_stream_index == -1)
        {
            qDebug() << "视频流不存在";
            return -1;
        }
    }

    //以下是帧转码相关代码
    srcFrame = av_frame_alloc();
    RGBFrame = av_frame_alloc();

    convertCtx = sws_getContext(video_width, video_height, codecCtx->pix_fmt, video_width, video_height, AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr);

    //计算一帧RGB图像需要的字节数
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, video_width, video_height, 1);

    //申请存放RGB frame数据的buffer
    RGB_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

    //将RGBFrame的data指针指向buffer
    av_image_fill_arrays(RGBFrame->data, RGBFrame->linesize, RGB_buffer, AV_PIX_FMT_RGB24, video_width, video_height, 1);

    seekEnd = duration = (double)formatCtx->duration / AV_TIME_BASE;
    seekEvent = true;

    //帧率
    frameRate = formatCtx->streams[video_stream_index]->r_frame_rate.num / formatCtx->streams[video_stream_index]->r_frame_rate.den;

    state = PLAYING;

    start();	//启动解码线程
}

void ReverseDecoder::freeRAM()
{
    SDL_LockMutex(frameListMutex);
    frameList.clear();
    SDL_UnlockMutex(frameListMutex);
    SDL_LockMutex(playListMutex);
    playList.clear();
    SDL_UnlockMutex(playListMutex);
    if (srcFrame) { av_frame_free(&srcFrame); srcFrame = nullptr; }
    if (RGBFrame) { av_frame_free(&RGBFrame); RGBFrame = nullptr; }
    if (convertCtx) { sws_freeContext(convertCtx); convertCtx = nullptr; }
    if (RGB_buffer) { av_free(RGB_buffer); RGB_buffer = nullptr; }
    if (codecCtx) { avcodec_free_context(&codecCtx); codecCtx = nullptr; }
    if (formatCtx) { avformat_close_input(&formatCtx); formatCtx = nullptr; }
}

/*
功能：解码线程
*/
void ReverseDecoder::run()
{
    runFinished = false;
    double timestamp;	//单位：秒
    AVPacket pkt;
    while (state != STOP)
    {
        if (state == PAUSE)
        {
            msleep(100);
            continue;
        }

        if (playList.size() > frameRate*2)    //2看经验设置，过小会使倒放卡顿，过大会使内存占用过高，取一个平衡
        {
            msleep(100);
            continue;
        }

        if (seekEvent)
        {
            //需要seek
            seekEvent = false;
            int64_t seek_val = seekEnd / av_q2d(formatCtx->streams[video_stream_index]->time_base) - 1;	//-1为了防止一直seek到同一个关键帧上
            if (seek_val < 0)	//解码完成
            {
                decodeFinished = true;
                break;
            }
            av_seek_frame(formatCtx, video_stream_index, seek_val, AVSEEK_FLAG_BACKWARD);	//找到seekEnd前的第一个关键帧
            avcodec_flush_buffers(codecCtx);
            afterSeek = true;
        }

        //读取一帧
        if (av_read_frame(formatCtx, &pkt) < 0)
        {
            //完成了最后一段视频的解码
            frameList2playList();
            seekEnd = seekStart;
            seekEvent = true;
            SDL_CreateThread(ReverseDecoder::playThread, "play thread", this);	//完成了第一次解码，启动渲染线程
            continue;
        }

        if (pkt.stream_index == video_stream_index)
        {
            timestamp = pkt.pts * av_q2d(formatCtx->streams[video_stream_index]->time_base);	//该帧的时间戳，单位：秒

            //刚调用过av_seek_frame
            if (afterSeek)
            {
                afterSeek = false;
                seekStart = timestamp;	//记录下该段解码的起始时间戳
            }

            //解码
            if (avcodec_send_packet(codecCtx, &pkt) != 0)
            {
                qDebug() << "解码失败";
                av_packet_unref(&pkt);
                continue;
            }
            if (avcodec_receive_frame(codecCtx, srcFrame) != 0)
            {
                qDebug() << "解码失败";
                av_packet_unref(&pkt);
                continue;
            }

            //转为RGB帧
            sws_scale(convertCtx, srcFrame->data, srcFrame->linesize, 0, video_height, RGBFrame->data, RGBFrame->linesize);

            //释放packet
            av_packet_unref(&pkt);

            QImage img(RGB_buffer, video_width, video_height, QImage::Format_RGB888);

            SDL_LockMutex(frameListMutex);
            frameList.append(Frame{ img.copy(),timestamp });
            SDL_UnlockMutex(frameListMutex);

            //完成一段视频的解码，或者是完成了最后一段视频的阶码
            if (timestamp >= seekEnd)
            {
                frameList2playList();
                seekEnd = seekStart;
                seekEvent = true;
            }
        }
    }
    runFinished = true; //解码线程已结束
}

/*
功能：渲染线程
*/
int ReverseDecoder::playThread(void* arg)
{
    ReverseDecoder* decoder = (ReverseDecoder*)arg;
    decoder->playThreadFinished = false;
    while (decoder->state != STOP)
    {
        //暂停状态
        if (decoder->state == PAUSE)
        {
            msleep(100);
            continue;
        }

        SDL_LockMutex(decoder->playListMutex);
        //获取两帧之间的时间间隔
        if (decoder->playList.size() >= 2)
        {
            if (decoder->playList[0].timestamp - decoder->playList[1].timestamp >= 0)
                decoder->interval = decoder->playList[0].timestamp - decoder->playList[1].timestamp;
            else    //如果两帧的时间差<=0，说明该帧有问题，直接跳过
            {
                decoder->playList.removeAt(1);						    //丢弃该帧
                SDL_UnlockMutex(decoder->playListMutex);
                continue;
            }
        }
        SDL_UnlockMutex(decoder->playListMutex);

        //播放一帧
        SDL_LockMutex(decoder->playListMutex);
        if (decoder->playList.size() > 0)
        {
            decoder->sendOneFrame(decoder->playList[0].frame);	    //发送一帧完成渲染
            decoder->setCurrTime(decoder->playList[0].timestamp);   //设置当前帧时间，控制进度条
            decoder->playList.pop_front();						    //pop该帧
            SDL_UnlockMutex(decoder->playListMutex);
            msleep(decoder->interval * 1000);						//sleep两帧间隔的时长
        }
        else if (decoder->decodeFinished)	//解码和播放都完成了，退出线程
        {
            SDL_UnlockMutex(decoder->playListMutex);
            break;
        }
        else
        {
            SDL_UnlockMutex(decoder->playListMutex);
        }
    }
    decoder->playThreadFinished = true; //渲染线程已结束
    return 0;
}

void ReverseDecoder::frameList2playList()
{
    SDL_LockMutex(frameListMutex);
    SDL_LockMutex(playListMutex);
    for (int i = frameList.size() - 1; i >= 0; i--)
    {
        playList.append(frameList[i]);
    }
    frameList.clear();
    SDL_UnlockMutex(playListMutex);
    SDL_UnlockMutex(frameListMutex);
}