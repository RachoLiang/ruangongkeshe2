#include "backend/PlayList.h"
PlayList::PlayList(QObject *parent):QObject(parent)
{
    qDebug()<<"构造playlist对象";
    init();
}
void PlayList::addFile(QString filePath)
{
    PlayListNode u;
    u.filePath=filePath;
    u.mediaType=1;
    fileList.push_back(u);
    qDebug()<<"playlist对象调用addFile："<<filePath;
    /*
     * 发射信号给qml，在界面列表中加入该媒体
     */
    emit addFileInGUI(filePath);
}
void PlayList::init()
{
    qDebug()<<"调用playlist对象的init";
    fileList.clear();
    historyList.clear();
    dequePos=0;
    dequeSize=0;
    playMode=SinglePlay;
    /*
     * 从数据库中读取数据，恢复出上次的fileList并同步nowIndex
     * 如果数据库中还存了上次的播放模式等信息，也可以考虑恢复
     */
}

