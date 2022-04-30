#include "backend/mySql.h"

mySql::mySql(QString db_name,QString db_user,QString db_pw){
    this->db_name = db_name;
    this->db_user = db_user;
    this->db_pw = db_pw;
    init_db();
}

mySql::mySql(){
    this->db_name = "music";
    this->db_user = "zhaoyang";
    this->db_pw = "zhaoyang";
    init_db();
}

void mySql::init_db(){
    if (QSqlDatabase::contains("my_qt_sql_conn"))
    {
        qdb = QSqlDatabase::database("my_qt_sql_conn");
    }else{
        qdb = QSqlDatabase::addDatabase("QSQLITE","my_qt_sql_conn");
        qdb.setDatabaseName(db_name);
        qdb.setUserName(db_user);
        qdb.setPassword(db_pw);
    }
}

bool mySql::open_db(){
    if(!qdb.open()){
        return false;
    }else{
        return true;
    }
}

void mySql::close_db(){
    qdb.close();
}






