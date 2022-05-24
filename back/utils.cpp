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

//获取视频媒体类型信息
void getMeidaInfo(QString path,Audio* audio){
    QMediaPlayer* player = new QMediaPlayer;
    player->setSource(QUrl::fromLocalFile(path));
    //是否有效
    if (player->isAvailable()){

        //获取媒体信息
        QMediaMetaData metaData = player->metaData();

        //判断媒体类型
        if(player->hasVideo()){  //视频
            audio = new Video();
            ((Video*)audio)->setVideoBitRate(metaData.value(QMediaMetaData::VideoBitRate).toInt());  //视频码率
            ((Video*)audio)->setVideoFrameRate(metaData.value(QMediaMetaData::VideoFrameRate).toInt());    //视频帧率
            ((Video*)audio)->setVideoType(metaData.value(QMediaMetaData::MediaType).toString());     //视频类型
            ((Video*)audio)->setResolvingPower(metaData.value(QMediaMetaData::Resolution).toInt());    //分辨率
            ((Video*)audio)->setAlbumImage(qvariant_cast<QImage>(metaData.value(QMediaMetaData::ThumbnailImage)));   //视频封面
        }else{  //音频
            audio = new Audio();
            audio->setAlbumInfo(metaData.value(QMediaMetaData::AlbumTitle).toString());  //专辑标题
            audio->setSinger(metaData.value(QMediaMetaData::AlbumArtist).toString());   //歌手
//            audio->setLyrics(metaData.value(QMediaMetaData::Lyrics).toString());  //歌词
            audio->setAlbumImage(qvariant_cast<QImage>(metaData.value(QMediaMetaData::CoverArtImage)));  //封面
        }

        //设置文件路径
        audio->setFilePath(path);
        audio->setFileName(getNameFromPath(path));
        audio->setIsvalid(true);
        //修改时间
        audio->setModificationTime(QDateTime::currentDateTime());
        audio->setPosition(0);

        //设置音频等信息
        audio->setDuration(player->duration());
        audio->setAudioBitRate(metaData.value(QMediaMetaData::AudioBitRate).toInt());  //音频码率
        audio->setAudioType(metaData.value(QMediaMetaData::MediaType).toString());   //音频类型
//        audio->setNumberOfChannels(metaData.value(QMediaMetaData::ChannelCount).toInt()); //声道数
    }else{
        audio = new Video();
        audio->setIsvalid(false);
    }
}
