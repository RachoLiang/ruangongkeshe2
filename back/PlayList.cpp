#include "backend/PlayList.h"
PlayList::PlayList(QObject *parent):QObject(parent)
{
    qDebug()<<"构造playlist对象";
}
QString extractFileName(QString filePath)
{
    int len=filePath.length();
    int pos=len-1;
    while(pos>0&&filePath[pos-1]!='/')pos--;
    return filePath.mid(pos);
}
QString changePathFormat(QString filePath) //     把"file:///C:/a.mp4" 转换成 "C:\\a.mp4"
{
    int len=filePath.length();
    QString ret="";
    if(len>8&&filePath[7]=='/')
    {
        for(int pos=8;pos<len;pos++)
        {
            if(filePath[pos]=='/')
            {
                ret+='\\';
                //ret+='\\';

            }
            else ret+=filePath[pos];
        }
    }
    return ret;
}
/*
 *                         nameFilters: [ "视频文件 (*.ts *.mp4 *.avi *.flv *.mkv *.3gp)",
                            "音频文件 (*.mp3 *.ogg *.wav *.wma *.ape *.ra)"]
*/
QString AllVideoType[]={"ts","mp4","avi","flv","mkv","3gp"};
QString AllAudioType[]={"mp3","ogg","wav","wma","ape","ra"};
bool isVideoFile(QString filePath)
{
    int len=filePath.length();
    int pos=len-1;
    while(pos>0&&filePath[pos-1]!='.')pos--;
    QString fileType=filePath.mid(pos);
    for(int idx=0;idx<6;idx++)
    {
        if(fileType==AllVideoType[idx])
        {
            return true;
        }
    }
    return false;
}
void PlayList::addFile(QString filePath)
{
    bool isVideo=isVideoFile(filePath);
    if(isVideo==true&&playListType==1||isVideo==false&&playListType==2)
    {
        return; //该媒体不应该放到这个列表中，而是应该放到另一个列表
    }
    QString changedPath=changePathFormat(filePath);
    PlayListNode u;
    u.filePath=changedPath;
    u.mediaType=1;
    fileList.push_back(u);
    qDebug()<<"playlist对象调用addFile："<<changedPath;
//    mySql sql;
//    if(sql.openDb())  //每次用户导入一个新的媒体，就在数据库中同步插入这个媒体信息
//    {
//        if(!sql.isTableExist("records"))
//        {
//            sql.createTable();
//        }
//        Record record;record.path=filePath;
//        sql.insertData(record);
//        sql.closeDb();
//    }
    /*
     * 发射信号给qml，在界面列表中加入该媒体
     */
    if(isVideo==false)
    {
        emit addAudioFileInGUI(extractFileName(filePath));
    }
    else
    {
        emit addVideoFileInGUI(extractFileName(filePath));
    }
}
void PlayList::saveToDataBase()  //已经不需要把表清空再写入，程序关闭时只需要保存播放的媒体和位置即可
{
//    mySql sql;
//    if(sql.openDb())
//    {
//        if(!sql.isTableExist("records"))
//        {
//            sql.createTable();
//        }
//        sql.deleteAll(); //暂时先清空整个表，然后再重新写入，待修改:优化时间复杂度
//        int len=fileList.size();
//        for(int idx=0;idx<len;idx++)
//        {
//            Record record;record.path=fileList[idx].filePath;
//            sql.insertData(record);
//        }
//        sql.closeDb(); //操作完成，关闭数据库，整个程序也关闭
//    }
}
void PlayList::init(int PlayListType)
{
    qDebug()<<"调用playlist对象的init";
    playListType=PlayListType;
    fileList.clear();
    historyList.clear();
    dequePos=0;
    dequeSize=0;
    playMode=SinglePlay;
    nowIndex=0;
    /*
     * 从数据库中读取数据，恢复出上次的fileList并同步nowIndex
     * 如果数据库中还存了上次的播放模式等信息，也可以考虑恢复
     */
//    mySql sql;
//    if(sql.openDb())
//    {
//        if(sql.isTableExist("records")) //如果有之前存的数据
//        {
//            qDebug()<<"恢复上次的播放列表";
//            sql.selectAllTofileList(fileList); //读取数据，恢复出fileList
//            //在后台恢复其他数据
//        }
//        sql.closeDb();
//    }
//    int len=fileList.size();
//    if(len!=0)
//    {
//        qDebug()<<"在界面恢复播放列表";
//        for(int idx=0;idx<len;idx++)
//        {
//            QString p=extractFileName(fileList[idx].filePath);
//            //qDebug()<<"界面恢复："<<p;
//            //emit addFileInGUI(p);
//        }
//    }
}
void PlayList::showFileList()
{
    qDebug()<<"展示整个playlist的vector：(实际filePath没有引号)";
    int len=fileList.size();
    for(int i=0;i<len;++i)
    {
        PlayListNode u=fileList[i];
        qDebug()<<i+1<<", "<<u.filePath ;
    }
}

void PlayList::setNowIndex(int index)
{
    nowIndex=index;
    if(playMode==Shuffle)
    {
        historyList.clear();
        historyList.push_back(nowIndex);
        dequeSize=1;
        dequePos=0;
    }
    QString mediaPath=fileList[nowIndex].filePath;
    /*
     * 播放mediaPath
     */
    qDebug()<<"选择播放："<<mediaPath;
    qDebug()<<"现在nowIndex=="<<nowIndex;
    if(playListType==2)
    {
        emit showVideo(mediaPath);
    }
}
//enum PlayBackMode
//{
//    SinglePlay=0,    //只播放当前
//    SingleLoop=1,    //单曲循环
//    SequentialPlay=2,//顺序播放
//    Repeat=3,      //列表循环
//    Shuffle=4     //随机播放
//};
long long bigRandomInteger(long long mod,long long nowIndex)
{
    long long ret=rand()%mod;
    if(ret==nowIndex)ret=rand()%mod;  //手动减小返回的随机数与nowIndex相同的概率
    return ret;  //本来试图产生大随机数，但试了很多种方法，结果都不够随机
}
void PlayList::playNextMedia()  //用户点击下一首按钮
{
    int listLength=fileList.size();
    if(listLength==0)return ; //列表为空，则什么都不做
    if(playMode==Shuffle)     //随机播放模式
    {
        if(dequeSize>dequePos+1)  //存在下一首的历史记录
        {
            dequePos++;
            nowIndex=historyList[dequePos];
        }
        else  //没有下一个的历史记录，则随机产生下一个，并增加到历史记录中
        {
            nowIndex=bigRandomInteger(listLength,nowIndex);
            historyList.push_back(nowIndex);
            dequeSize++;
            dequePos=dequeSize-1;
        }
    }
    else   //非随机播放模式，点击下一首，必定是列表中直接选取下一首（对表长取模）
    {
        nowIndex=(nowIndex+1)%(listLength);
    }
    QString mediaPath=fileList[nowIndex].filePath;
    qDebug()<<"点击了下一首，播放："<<mediaPath;
    emit changeCurrentPlayingIndex(nowIndex);

    /*
     * 调用播放mediaPath操作
     */
}
void PlayList::playLastMedia()
{
    int listLength=fileList.size();
    if(listLength==0)return ; //列表为空，则什么都不做
    if(playMode==Shuffle)
    {
        if(dequePos>0)  //存在上一首的历史记录
        {
            dequePos--;
            nowIndex=historyList[dequePos];
        }
        else   //没有上一首的历史记录，则随机产生上一首，并增加到历史记录中
        {
            nowIndex=bigRandomInteger(listLength,nowIndex);
            historyList.push_front(nowIndex);
            dequeSize++;
        }
    }
    else
    {
        nowIndex=(nowIndex-1+listLength)%(listLength);  //实际上是列表循环的处理方式，后续要改成如果到列表尾部，则播放结束
    }
    QString mediaPath=fileList[nowIndex].filePath;
    qDebug()<<"点击了上一首，播放："<<mediaPath;
    emit changeCurrentPlayingIndex(nowIndex);

    /*
     * 播放mediaPath
     */
}
void PlayList::changePlayMode()
{
    playMode=(playMode+1)%5;
    QString iconName="";
    historyList.clear();   //改变播放模式，会导致随机播放记录的清空
    dequePos=0;
    dequeSize=0;
    if(playMode==SinglePlay)iconName="singlePlay";
    else if(playMode==SingleLoop)iconName="singleLoop";
    else if(playMode==SequentialPlay)iconName="sequentialPlay";
    else if(playMode==Repeat)iconName="repeat";
    else
    {
        iconName="shuffle";
        historyList.push_back(nowIndex); //当前播放的一首，加入播放列表中
        dequeSize++;
    }
    qDebug()<<"当前播放模式："<<iconName;
    emit changePlayModeButtonIcon(iconName);

}
