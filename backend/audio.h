#ifndef AUDIO_H
#define AUDIO_H

#endif // AUDIO_H
#include <QMediaPlayer>
#include "caption.h"
#include <QString>
#include <QList>
#include <QAudioOutput>

//音频类
class Audio  //暂时不做字幕功能，后期可能会继承/包含caption类
{
public:
    Audio() {}
    Audio(QString audioPath);   //通过文件路径导入资源
    Audio(QString audioPath,QString fileName,QString duration,int position,QDateTime modificationTime,bool isvalid);  //通过数据库信息导入资源
    void setAudioOutput(QAudioOutput* ouput);
    bool onPlay();               //切换视频的播放状态:播放-》暂停|暂停-》播放
    void adjustVolume(int);     //调节音量
    void adjustProgress(int);   //调节播放进度
    void adjustRate(int);   //调节播放速率
    bool isPlaying();   //当前是否正在播放
    bool isPausing();   //当前是否暂停
    bool isValid();     //当前资源是否有效：资源文件获取不到|被用户手动删除；在关闭播放器后会对应修改数据库中资源有效性
    //albumCoverExtraction      //提取专辑封面
    void setIsPlayingNow(bool);
    void setAudioBitRate(int);
    void setPosition(int value);    //设置播放进度
    void setVaild(bool value);  //设置资源有效性：删除时value = false
    QMediaPlayer* getPlayer();  //获取播放player对象
    /* 每个属性都有对应的set操作
    * setXXX()
    */

private:
    QMediaPlayer* player;
    bool isvalid;   //是否有效
    //资源信息
    QString filePath;  //文件路径
    QString fileName;  //文件名
    //媒体信息
    int duration;   //音频|音视频时间长度
    int audioBitRate;   //音频码率
    QString audioType;  //音频类型
    int numberOfChannels;  //声道数
    QString albumInfo; //专辑信息
    QString singer;  //演唱者
    //缓存信息
    int position;   //上次播放的进度|当前播放的进度
    QDateTime modificationTime;  //上次修改时间

};
