//#ifndef AUDIO_H
//#define AUDIO_H

//#endif // AUDIO_H
//#include <QMediaPlayer>
//#include "caption.h"
//#include <QString>
//#include <QList>
//#include <QAudioOutput>

////音频类
//class Audio  //暂时不做字幕功能，后期可能会继承/包含caption类
//{
//public:
//    Audio() {}
//    void setAudioOutput(QAudioOutput ouput);
//    Audio(QString audioPath);
//    void play();                //播放
//    void pause();               //暂停
//    void adjustVolume(int);     //调节音量
//    void adjustProgress(int);   //调节播放进度
//    //albumCoverExtraction      //提取专辑封面
//    void setIsPlayingNow(bool);
//    void setAudioBitRate(int);
//    /* 每个属性都有对应的set操作
//    * setXXX()
//    */

//private:
//    QMediaPlayer* player;
//    bool invalid;
//    QString filePath;  //文件路径
//    QString fileName;  //文件名
//    bool isPlayingNow; //是否当前正在播放
//    QDateTime modificationTime;  //上次修改时间
//    int duration;   //音频时间长度
//    int audioBitRate;   //音频码率
//    QString audioType;  //音频类型
//    int numberOfChannels;  //声道数
//    QString albumInfo; //专辑信息
//    QString singer;  //演唱者

//};
