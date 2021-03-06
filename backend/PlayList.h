#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "vedio.h"
#include <QString>
#include <QThread>
#include <vector>
#include <deque>
#include <QDebug>
#include <QObject>
#include <ctime>
#include <random>
#include "mySql.h"
#include "PlayListNode.h"
#include "utils.h"
#include <string>
#include <fstream>
#include <iostream>
#include <QFileInfo>
enum PlayBackMode
{
    SinglePlay=0,    //只播放当前
    SingleLoop=1,    //单曲循环
    SequentialPlay=2,//顺序播放
    Repeat=3,      //列表循环
    Shuffle=4     //随机播放
};

//视频列表管理类，全局只有一个对象，程序启动时构造
class PlayList : public QObject
{
    Q_OBJECT
private:
    int playListType;         //为1表示音频列表，2表示视频列表
    /* 基于vector的fileList
     * 保存整个音视频列表（歌单）
     * 在程序启动时，从数据库中读取数据恢复此vector
     * 其余时候，仅在用户导入，删除，置顶一个媒体时，这个vector才会修改
    */
    std::vector<PlayListNode>fileList;
    /* 基于deque的historyList，其中的元素是随机播放时选取的fileList中的下标
     * 仅用于随机播放模式下，“上一首/下一首”就是在此deque中左右移动的过程
     * 变更播放模式，将清空此deque
    */
    std::deque<int>historyList;
    int dequePos;
    int dequeSize;
    int playMode;     //播放模式（程序启动时默认为只播放当前）
    int nowIndex;     //当前播放的视频序号
    mySql* sql;
public:
    explicit PlayList(QObject *parent=nullptr);
    ~PlayList();
    //void setPlayMode(int);
    //int findNextNodeID();
    //int findLastNodeID();

    Q_INVOKABLE bool setNowIndex(int);  //用户点击某个媒体时，更新当前播放序号，并播放该媒体
    Q_INVOKABLE bool playNextMedia(int callTimes=0);    //点击下一首，根据播放模式获取下一首的文件路径，并播放
    Q_INVOKABLE bool playLastMedia(int callTimes=0);    //点击上一首
    Q_INVOKABLE void autoPlayNextMedia();    //在当前曲目播放完成后调用，自动播放下一首，根据播放模式获取下一首的文件路径，并播放
    Q_INVOKABLE void changePlayMode();   //点击播放模式按钮

    //QString nowPlayingFilePath();
    Q_INVOKABLE void addFile(QString);   //用户导入一个媒体文件
    Q_INVOKABLE void removeFile(int);    //用户删除一个媒体文件
    Q_INVOKABLE void toppingFile(int);   //用户置顶一个媒体文件
    Q_INVOKABLE void saveToDataBase();  //保存当前现场，包括当前播放的媒体和时刻位置
    Q_INVOKABLE void init(int);   //需要在qml的ListView组件准备好之后，才能init，不然发射了信号也没人接收
    Q_INVOKABLE void showFileList();
    Q_INVOKABLE QVariantMap getMediaInfo(int index,QString type); //获取文件详细信息
    Q_INVOKABLE int getNowIndex();
    Q_INVOKABLE void saveFlags(bool,int,double,QString,QString);
    Q_INVOKABLE void selectFlags(bool&,int&,double&);
    Q_INVOKABLE bool selectISAudio();
    Q_INVOKABLE int selectNowIndex();
    Q_INVOKABLE double selectControlValue();
    Q_INVOKABLE QString selectCutPath();
    Q_INVOKABLE QString selectCutType();
    Q_INVOKABLE bool setNowIndexWhenInit(int); //程序刚启动时，恢复上一次播放的条目，但保持暂停状态
    Q_INVOKABLE QString getFilename(int);   //通过index获取filename，用于倒放模块

signals: //信号只需要定义，不需要实现
    void addAudioFileInGUI(QString audioPath,QString audioDuration);
    void addVideoFileInGUI(QString videoPath,QString videoDuration);
    void changeCurrentPlayingIndex(int index);
    void changePlayModeButtonIcon(QString iconName);
    void showVideo(QString videoPath);
    void showAudio(QString audioPath);
    void showMessage(QString messageInfo);
};
#endif // PLAYLIST_H
