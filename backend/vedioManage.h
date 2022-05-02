#ifndef VEDIOMANAGE_H
#define VEDIOMANAGE_H

#endif // VEDIOMANAGE_H

#include "audio.h"
#include "vedio.h"
#include <QString>
#include <QThread>
#include <vector>
#include <deque>

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
    int dequePos=0;
    int dequeSize=0;
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
