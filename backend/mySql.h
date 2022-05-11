#ifndef MYSQL_H
#define MYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <vector>
#include "PlayListNode.h"
typedef struct
{
    int id; //key
    QString path; //路径
    QString title; //标题
    int time; //时长，单位s
    QString type; //媒体类型，比如mp3,mp4
    QDateTime playTime; //上次播放日期 yyyy-MM-dd hh:mm:ss
    double progress; //播放进度
}Record;

class mySql{
public:
    mySql();

    //打开数据库
    bool openDb(void);
    //创建表
    void createTable(void);
    //判断表是否存在
    bool isTableExist(QString tableName);
    //插入数据
    void insertData(Record &data);
    //修改数据
    void modifyData(int,QString,QString,int,QString,QDateTime,double);
    //查询数据
    Record selectData(int id);
    //查询所有数据
    QList<Record> selectall();
    void selectAllTofileList(std::vector<PlayListNode>&fileList);
    //删除数据
    void deleteData(int);
    //删除所有数据
    void deleteAll();
    //删除表
    void deleteTable(QString tableName);
    //关闭数据库
    void closeDb(void);



private:
    QSqlDatabase qdb;
};
#endif // MYSQL_H
