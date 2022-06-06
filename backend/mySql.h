#ifndef MYSQL_H
#define MYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <vector>
#include <QDebug>
#include "PlayListNode.h"
#include "audio.h"
#include "vedio.h"

class mySql{
public:
    mySql();

    //打开数据库
    bool openDb(void);
    //创建表 两个表 一个videos一个audios
    void createTableAudios(void);
    void createTableVideos(void);
    void createTableFlags(void);

    //判断表是否存在
    bool isTableExist(QString tableName);

    //插入,插入时不需要设置对象的id
    bool insertAudio(Audio* data);
    bool insertVideo(Video* video);
    void saveFlags(bool isAudio,int index,double percent,QString cutPath,QString cutType);

    //修改
    void modifyAudio(int id,Audio* audio);
    void modifyVideo(int id,Video* video);
    //查询
    Audio* selectAudio(int id);
    Video* selectVideo(int id);
    Audio* selectAudioByPath(QString path);
    Video* selectVideoByPath(QString path);
    void selectFlags1(bool& isAudio,int& index,double& percent);
    void selectFlags2(QString& cutPath,QString& cutType);
    //查询所有数据
    QList<Audio*> selectAllAudio();
    QList<Video*> selectAllVideo();
    //删除数据
    void deleteAudio(int id);
    void deleteVideo(int id);
    //删除所有数据
    void deleteAllAudio();
    void deleteAllVideo();
    //删除表
    void deleteTable(QString tableName);
    //关闭数据库
    void closeDb(void);
    //恢复播放列表用
    void selectAllAudioTofileList(std::vector<PlayListNode>&fileList);
    void selectAllVideoTofileList(std::vector<PlayListNode>&fileList);


private:
    QSqlDatabase qdb;
};
#endif // MYSQL_H
