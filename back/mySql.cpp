#include "backend/mySql.h"

mySql::mySql(){
    if (QSqlDatabase::contains("my_qt_sql_conn"))
    {
        qdb = QSqlDatabase::database("my_qt_sql_conn");
    }else{
        qDebug()<<"new conn";
        qdb = QSqlDatabase::addDatabase("QSQLITE","my_qt_sql_conn");
        qdb.setDatabaseName("records.db"); //数据库文件名
    }
}

bool mySql::openDb(){
    if(!qdb.isOpen()){
        if(!qdb.open()){
            return false;
        }else{
            return true;
        }
    }else{
        return true;
    }
}

void mySql::createTable(){
    QSqlQuery sqlQuery(this->qdb);
    QString sql = QString("CREATE TABLE records (\
                        id INTEGER PRIMARY KEY AUTOINCREMENT,\
                        path TEXT NOT NULL UNIQUE,\
                        title TEXT,\
                        time INT,\
                        type VARCHAR(16),\
                        playTime DATETIME,\
                        progress DOUBLE CHECK(progress>=0 AND progress<=1))");
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to create table."<<sqlQuery.lastError();
    }else{
        qDebug()<<"creta table records successfully.";
    }
}

bool mySql::isTableExist(QString tableName){
    QSqlDatabase db = QSqlDatabase::database("my_qt_sql_conn");
    if(db.tables().contains(tableName)){
        qDebug()<<"the table "<<tableName<<" is exist.it's not need to create a new one.";
        return true;
    }else{
        return false;
    }
}

void mySql::insertData(Record &data){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("INSERT INTO records(path,title,time,type,playTime,progress) VALUES(:path,:title,:time,:type,:playTime,:progress)");
    sqlQuery.bindValue(":path",data.path);
    sqlQuery.bindValue(":title",data.title);
    sqlQuery.bindValue(":time",data.time);
    sqlQuery.bindValue(":type",data.type);
    sqlQuery.bindValue(":playTime",data.playTime.toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":progress",data.progress);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to insert data."<<sqlQuery.lastError();
    }else{
        qDebug()<<"insert data successfully";
    }
}

Record mySql::selectData(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from records where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select data."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.size()==0){
            qDebug()<<"not found id:"<<id;
            Record rd;
            return rd;
        }else{
            sqlQuery.first();
            Record rd;
            rd.id = sqlQuery.value("id").toInt();
            rd.path = sqlQuery.value("path").toString();
            rd.title = sqlQuery.value("title").toString();
            rd.time = sqlQuery.value("time").toInt();
            rd.type = sqlQuery.value("type").toString();
            rd.playTime = QDateTime::fromString(sqlQuery.value("playTime").toString(),"yyyy-MM-dd hh:mm:ss");
            rd.progress = sqlQuery.value("progress").toDouble();

            return rd;
        }
    }
}

QList<Record> mySql::selectall(){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from records");
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select data."<<sqlQuery.lastError();
    }else{
        QList<Record> qlist;
        while(sqlQuery.next()){
            Record rd;
            rd.id = sqlQuery.value("id").toInt();
            rd.path = sqlQuery.value("path").toString();
            rd.title = sqlQuery.value("title").toString();
            rd.time = sqlQuery.value("time").toInt();
            rd.type = sqlQuery.value("type").toString();
            rd.playTime = QDateTime::fromString(sqlQuery.value("playTime").toString(),"yyyy-MM-dd hh:mm:ss");
            rd.progress = sqlQuery.value("progress").toDouble();

            qlist.push_back(rd);
        }
        return qlist;
    }
}

void mySql::modifyData(int id,QString path,QString title,int time,QString type,QDateTime playTime,double progress){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("update records set path=:path,title=:title,time=:time,type=:type,playTime=:playTime,progress=:progress where id=:id");
    sqlQuery.bindValue(":path",path);
    sqlQuery.bindValue(":title",title);
    sqlQuery.bindValue(":time",time);
    sqlQuery.bindValue(":type",type);
    sqlQuery.bindValue(":playTime",playTime.toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":progress",progress);
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to update data."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"update successful";
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteData(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("delete from records where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete successful";
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteAll(){
    QSqlQuery sqlQuery(this->qdb);
    if(!sqlQuery.exec("delete from records")){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete all successful.rows:"<<sqlQuery.numRowsAffected();
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteTable(QString tableName){
    QSqlQuery sqlQuery(this->qdb);
    if(!sqlQuery.exec("drop table records")){
        qDebug()<<"drop table error"<<sqlQuery.lastError();
    }else{
        qDebug()<<"drop table successful";
    }
}

void mySql::closeDb(){
    qdb.close();
}






