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
    emit addFileInGUI(extractFileName(filePath));
}
void PlayList::saveToDataBase()
{
    mySql sql;
    if(sql.openDb())
    {
        if(!sql.isTableExist("records"))
        {
            sql.createTable();
        }
        sql.deleteAll(); //暂时先清空整个表，然后再重新写入，待修改:优化时间复杂度
        int len=fileList.size();
        for(int idx=0;idx<len;idx++)
        {
            Record record;record.path=fileList[idx].filePath;
            sql.insertData(record);
        }
        sql.closeDb(); //操作完成，关闭数据库，整个程序也关闭
    }
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
    mySql sql;
    if(sql.openDb())
    {
        if(sql.isTableExist("records")) //如果有之前存的数据
        {
            qDebug()<<"恢复上次的播放列表";
            sql.selectAllTofileList(fileList); //读取数据，恢复出fileList
            //在后台恢复其他数据
        }
        sql.closeDb();
    }
    int len=fileList.size();
    if(len!=0)
    {
        qDebug()<<"在界面恢复播放列表";
        for(int idx=0;idx<len;idx++)
        {
            QString p=extractFileName(fileList[idx].filePath);
            qDebug()<<"界面恢复："<<p;
            emit addFileInGUI(p);
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
