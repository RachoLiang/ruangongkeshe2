#include "backend/PlayList.h"
PlayList::PlayList(QObject *parent):QObject(parent)
{
    sql = new mySql();
    if(sql->openDb()){   //打开数据库
        if(!sql->isTableExist("audios")){
            sql->createTableAudios();
        }else{
            qDebug()<<"audios表已存在";
        }
        if(!sql->isTableExist("videos")){
            sql->createTableVideos();
        }else{
            qDebug()<<"videos表已存在";
        }
        if(!sql->isTableExist("flags")){
            sql->createTableFlags();
        }else{
            qDebug()<<"Flags表已存在";
        }
    }else{
        qDebug()<<"数据库打开失败";
    }
    //qDebug()<<"构造playlist对象";
}
PlayList::~PlayList(){
    //关闭数据库
    sql->closeDb();
    delete sql;
    sql = NULL;
}

QString extractFileName(QString filePath)
{
    int len=filePath.length();
    int pos=len-1;
    while(pos>0&&filePath[pos-1]!='/'&&filePath[pos-1]!='\\')pos--;
    return filePath.mid(pos);
}
QString changeTimeFormat(int duration)
{
    int hours=duration/3600;
    duration%=3600;
    int minutes=duration/60;
    int seconds=duration%60;
    QString ret="";
    if(hours<10)ret+="0";
    ret+=std::to_string(hours).c_str();ret+=":";
    if(minutes<10)ret+="0";
    ret+=std::to_string(minutes).c_str();ret+=":";
    if(seconds<10)ret+="0";
    ret+=std::to_string(seconds).c_str();
    return ret;
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
                            "音频文件 (*.mp3 *.ogg *.wav *.wma *.ape *.ra *.flac)"]
*/
QString AllVideoType[]={"ts","mp4","avi","flv","mkv","3gp"};
QString AllAudioType[]={"mp3","ogg","wav","wma","ape","ra","flac"};
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
    qDebug()<<"isVideo="<<isVideo<<",and playListType="<<playListType;
    if(isVideo==true&&playListType==1||isVideo==false&&playListType==2)
    {
        return; //该媒体不应该放到这个列表中，而是应该放到另一个列表
    }
    QString changedPath=changePathFormat(filePath);
    PlayListNode u;
    u.filePath=changedPath;
    /*
     * 发射信号给qml，在界面列表中加入该媒体
     */
    if(isVideo==false) //音频
    {
        //或许详细信息，插入数据库
        Audio* audio = getAudioInfo(changedPath);
        bool flag = sql->insertAudio(audio);  //如果插入成功，会顺便查询分配给它的id
        u.duration=audio->getDuration();
        u.id=audio->getId();
        delete audio;
        audio = NULL;
        if(flag){
            u.mediaType=1;
            fileList.push_back(u);
            qDebug()<<"音频列表成功添加："<<changedPath<<",数据表中id="<<u.id;
            emit addAudioFileInGUI(extractFileName(filePath),changeTimeFormat(u.duration));
        }else{
            qDebug()<<"插入失败，重复的插入";
        }
    }
    else  //视频
    {
        //获取详细信息，插入数据库
        Video *video = getVideoInfo(changedPath);
        bool flag = sql->insertVideo(video);
        u.duration=video->getDuration();
        u.id=video->getId();
        delete video;
        video = NULL;
        if(flag){
            u.mediaType=2;
            fileList.push_back(u);
            qDebug()<<"视频列表成功添加："<<changedPath;
            emit addVideoFileInGUI(extractFileName(filePath),changeTimeFormat(u.duration));
        }else{
            qDebug()<<"插入失败，重复的插入";
        }
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
    if(PlayListType == 1){
        sql->selectAllAudioTofileList(fileList); //读取数据，恢复出fileList
        int len=fileList.size();
        if(len!=0)
        {
            qDebug()<<"在界面恢复音频列表";
            for(int idx=0;idx<len;idx++)
            {
                emit addAudioFileInGUI(extractFileName(fileList[idx].filePath),changeTimeFormat(fileList[idx].duration));
            }
        }

    }else if(PlayListType == 2){
        sql->selectAllVideoTofileList(fileList);
        int len=fileList.size();
        if(len!=0)
        {
            qDebug()<<"在界面恢复视频列表";
            for(int idx=0;idx<len;idx++)
            {
                emit addVideoFileInGUI(extractFileName(fileList[idx].filePath),changeTimeFormat(fileList[idx].duration));
            }
        }
    }
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
//bool fileExist(QString filePath)
//{
//    std::string str=filePath.toStdString();
//    if(FILE *file=fopen(str.c_str(),"r"))
//    {
//        fclose(file);
//        return true;
//    }

//    return false;
//}
bool isFileExist(QString& filePath)
{
    QFileInfo fileInfo(filePath);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}
bool PlayList::setNowIndex(int index)
{
    QString mediaPath=fileList[index].filePath;
    nowIndex=index;
    if(playMode==Shuffle)
    {
        historyList.clear();
        historyList.push_back(nowIndex);
        dequeSize=1;
        dequePos=0;
    }
    if(isFileExist(mediaPath)==false) //文件不存在
    {
        qDebug()<<"点播的文件不存在:"<<mediaPath;
        showMessage("播放失败，已自动切换下一首");
        return false;
    }
    /*
     * 播放mediaPath
     */
    qDebug()<<"选择播放："<<mediaPath;
    qDebug()<<"现在nowIndex=="<<nowIndex;
    if(playListType==2)
    {
        emit showVideo(mediaPath);
    }
    else
    {
        emit showAudio(mediaPath);
    }
    return true; //点播成功
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
bool PlayList::playNextMedia(int callTimes)  //用户点击下一首按钮
{
    int listLength=fileList.size();
    if(listLength==0)return true; //列表为空，则什么都不做
    if(callTimes>listLength)return false;  //递归次数达到上限
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

    if(isFileExist(mediaPath)==false)  //文件不存在
    {
        showMessage("播放失败，已自动切换下一首");
        return playNextMedia(callTimes+1);
    }
    /*
     * 调用播放mediaPath操作
     */
    if(playListType==1)
    {
        emit showAudio(mediaPath);
    }
    else
    {
        emit showVideo(mediaPath);
    }
    return true;
}
bool PlayList::playLastMedia(int callTimes)
{
    int listLength=fileList.size();
    if(listLength==0)return true; //列表为空，则什么都不做
    if(callTimes>listLength)return false;  //递归次数达到上限
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

    if(isFileExist(mediaPath)==false)  //文件不存在
    {
        showMessage("播放失败，已自动切换上一首");
        return playLastMedia(callTimes+1);
    }
    /*
     * 播放mediaPath
     */
    if(playListType==1)
    {
        emit showAudio(mediaPath);
    }
    else
    {
        emit showVideo(mediaPath);
    }
    return true;
}
void PlayList::changePlayMode()
{
    playMode=(playMode+1)%5;
    historyList.clear();   //改变播放模式，会导致随机播放记录的清空
    dequePos=0;
    dequeSize=0;
    QString iconName="";
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
    if(playListType==2)return;  //只需要一个列表对象对界面上的图片进行替换
    qDebug()<<"当前播放模式："<<iconName;
    iconName="images/"+iconName+".png";
    emit changePlayModeButtonIcon(iconName);

}
QVariantMap PlayList::getMediaInfo(int index,QString mediaType){
    QString mediaPath = fileList[index].filePath;
    if(mediaType == "video"){
        Video* video = NULL;
        if(sql->selectVideoByPath(mediaPath) != NULL){  //判断数据库中是否存在
            video = sql->selectVideoByPath(mediaPath);
        }else{
            video = getVideoInfo(mediaPath);  //不存在就调用getVideoInfo解析文件获取信息
        }
        QVariantMap map;
        map.insert("fileName",extractFileName(mediaPath));
        map.insert("fileType",video->getMediaType());
        map.insert("path",video->getFilePath());
        map.insert("totalTime",QString("%1").arg(video->getDuration()));
        map.insert("videoBitRate",video->getVideoBitRate());
        map.insert("videoFrameRate",video->getVideoFrameRate());
        map.insert("resolvingPower",video->getResolvingPower());
        map.insert("audioBitRate",video->getAudioBitRate());
        map.insert("numberOfChannels",QString("%1").arg(video->getNumberOfChannels()));
        map.insert("sample_rate",video->getSampleRate());
        return map;
    }else if(mediaType == "music"){
        Audio* audio = NULL;
        if(sql->selectAudioByPath(mediaPath) != NULL){  //判断数据库中是否存在
            audio = sql->selectAudioByPath(mediaPath);
        }else{
            audio = getAudioInfo(mediaPath);  //不存在就调用getAudioInfo解析文件获取信息
        }
        QVariantMap map;
        map.insert("fileName",extractFileName(mediaPath));
        map.insert("fileType",audio->getMediaType());
        map.insert("path",audio->getFilePath());
        map.insert("totalTime",QString("%1").arg(audio->getDuration()));
        map.insert("audioBitRate",audio->getAudioBitRate());
        map.insert("numberOfChannels",QString("%1").arg(audio->getNumberOfChannels()));
        map.insert("sample_rate",audio->getSampleRate());
        return map;
    }else{
        QVariantMap map;
        return map;
    }
}

int PlayList::getNowIndex(){
    return this->nowIndex;
}

Q_INVOKABLE QString PlayList::getFilename(int index)
{
    return fileList[index].filePath;
}

/*
 * enum PlayBackMode
{
    SinglePlay=0,    //只播放当前
    SingleLoop=1,    //单曲循环
    SequentialPlay=2,//顺序播放
    Repeat=3,      //列表循环
    Shuffle=4     //随机播放
};*/
void PlayList::autoPlayNextMedia()
{
    qDebug()<<"自动下一首";
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
    else if(playMode==SinglePlay)  //只播放当前，则播放完成后就不再播放下一首
    {
        return;
    }
    else if(playMode==SingleLoop)
    {
        //单曲循环模式，则播放完成后重新播放即可
    }
    else if(playMode==SequentialPlay) //顺序播放模式，如果正好播完了列表最后一首，那么停止，否则顺序下一首
    {
        if(nowIndex==listLength-1)return;
        nowIndex++;
    }
    else if(playMode==Repeat) //列表循环模式，对表长取模
    {
        nowIndex=(nowIndex+1)%(listLength);
    }
    QString mediaPath=fileList[nowIndex].filePath;
    qDebug()<<"点击了下一首，播放："<<mediaPath;
    emit changeCurrentPlayingIndex(nowIndex);
    if(isFileExist(mediaPath)==false)
    {
        qDebug()<<"auto next";
        playNextMedia(0);
        return;
    }
    /*
     * 调用播放mediaPath操作
     */
    if(playListType==1)
    {
        emit showAudio(mediaPath);
    }
    else
    {
        emit showVideo(mediaPath);
    }
}

void PlayList::removeFile(int index)
{
    //在数据库中删除
    int id=fileList[index].id;
    qDebug()<<"在数据库中删除，id="<<id;
    if(playListType==1)
    {
        sql->deleteAudio(id);
    }
    else
    {
        sql->deleteVideo(id);
    }
    //vector中也删除这个文件
    fileList.erase(fileList.begin()+index);
    if(nowIndex>=index)nowIndex--;
    //随机播放模式的历史记录也要维护
    int pos=0;
    while(pos<dequeSize)
    {
        if(historyList[pos]<index)  //下标在index之前的，不需要变动
        {
            pos++;
        }
        else if(historyList[pos]==index) //历史记录中，这一个正好是被删除的文件
        {
            historyList.erase(historyList.begin()+pos);
            if(dequePos>=pos)dequePos--;
            dequeSize--;
        }
        else if(historyList[pos]>index)  //下标在index之后的，需要往前移一位
        {
            historyList[pos]--;
            pos++;
        }
    }
}
void PlayList::toppingFile(int index)
{
    //vector中把index位置的元素移动到vector的首部
    PlayListNode u=fileList[index];
    fileList.erase(fileList.begin()+index);
    fileList.insert(fileList.begin(),u);
    //维护nowIndex
    if(nowIndex==index)nowIndex=0;
    else if(nowIndex<index)nowIndex++;
    //维护historyList
    int pos=0;
    while(pos<dequeSize)
    {
        if(historyList[pos]==index)historyList[pos]=0;
        else if(historyList[pos]<index)historyList[pos]++;
        pos++;
    }
}
