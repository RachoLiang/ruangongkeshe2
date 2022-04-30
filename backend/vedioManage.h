#ifndef VEDIOMANAGE_H
#define VEDIOMANAGE_H

#endif // VEDIOMANAGE_H

#include "audio.h"
#include "vedio.h"
#include <QString>
#include <QList>
#include <QThread>
#include <vector>
#include <stack>

struct PlayListNode
{
    int mediaType;
    Audio *audioPtr;
    Video *videoPtr;
    PlayListNode(int MediaType=1,Audio *AudioPtr=nullptr,Video *VideoPtr=nullptr)
    {
        mediaType=MediaType;
        audioPtr=AudioPtr;
        videoPtr=VideoPtr;
    }
};
//视频列表管理类，全局只有一个对象，程序启动时构造
class PlayList:public Video
{
private:
    vector<PlayListNode>fileList;      //音视频列表
    //stack<PlayListNode>historyList;   //播放历史记录（需要自行实现stack类）
    int playMode;     //播放模式
    int nowIndex;     //当前播放的视频序号
public:
    void setPlayMode(int);
    int findNextNodeID();
    int findLastNodeID();
    QString nowPlayingFilePath();
    void addFile(QString);
    void deleteFile(QString);
    void saveToDataBase();
    void init();
};
