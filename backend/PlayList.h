#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "audio.h"
#include "vedio.h"
#include <QString>
#include <QThread>
#include <vector>
#include <deque>
#include<QDebug>
#include<QObject>
#include "mySql.h"
#include "PlayListNode.h"
enum PlayBackMode
{
    SinglePlay=0,    //只播放当前
    SingleLoop=1,    //单曲循环
    SequentialPlay=2,//顺序播放
    ListLoop=3,      //列表循环
    RandomPlay=4     //随机播放
};

//视频列表管理类，全局只有一个对象，程序启动时构造
class PlayList : public QObject
{
    Q_OBJECT
private:
    /* 基于vector的fileList
     * 保存整个音视频列表（歌单）
     * 在程序启动时，从数据库中读取数据恢复此vector
     * 其余时候，仅在用户导入/剔除一个媒体时，这个vector才会修改
    */
    std::vector<PlayListNode>fileList;

    /* 基于deque的historyList
     * 仅用于随机播放模式下，“上一首/下一首”就是在此deque中左右移动的过程
     * 变更播放模式，将清空此deque
    */
    std::deque<PlayListNode>historyList;
    int dequePos;
    int dequeSize;
    int playMode;     //播放模式（程序启动时默认为只播放当前）
    int nowIndex;     //当前播放的视频序号
public:
    explicit PlayList(QObject *parent=nullptr);
    //void setPlayMode(int);
    //int findNextNodeID();
    //int findLastNodeID();
    //QString nowPlayingFilePath();
    Q_INVOKABLE void addFile(QString);
    //void deleteFile(QString);
    Q_INVOKABLE void saveToDataBase();  //保存当前现场，包括整个fileList和当前播放的媒体和时刻位置
    Q_INVOKABLE void init();   //需要在qml的ListView组件准备好之后，才能init，不然发射了信号也没人接收
    Q_INVOKABLE void showFileList();

signals:
    void addFileInGUI(QString filePath); //信号只需要定义，不需要实现
};
#endif // PLAYLIST_H
