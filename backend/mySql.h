#ifndef MYSQL_H
#define MYSQL_H

#endif // MYSQL_H

#include <QSqlDatabase>

class mySql{
public:
    mySql();
    mySql(QString,QString,QString);

    void init_db();
    bool open_db();
    void close_db();
    void _insert();  //插入
    void _select();  //查询
    void _update();  //更新
    void _delete();  //删除


private:
    QSqlDatabase qdb;
    QString db_name;
    QString db_user;
    QString db_pw;

};
