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

void mySql::createTableAudios(){
    QSqlQuery sqlQuery(this->qdb);
    QString sql = QString("CREATE TABLE audios (\
                        id INTEGER PRIMARY KEY AUTOINCREMENT,\
                        path TEXT NOT NULL UNIQUE,\
                        mediaType VARCHAR(32),\
                        duration INT,\
                        audioBitRate VARCHAR(16),\
                        numberOfChannels INT,\
                        sample_rate VARCHAR(16),\
                        modicationTime DATETIME,\
                        position DOUBLE CHECK(position>=0 AND position<=1),\
                        isValid TINYINT(1))");
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to create table."<<sqlQuery.lastError();
    }else{
        qDebug()<<"creta table audios successfully.";
    }
}

void mySql::createTableVideos(){
    QSqlQuery sqlQuery(this->qdb);
    QString sql = QString("CREATE TABLE videos (\
                        id INTEGER PRIMARY KEY AUTOINCREMENT,\
                        path TEXT NOT NULL UNIQUE,\
                        mediaType VARCHAR(32),\
                        duration INT,\
                        videoBitRate VARCHAR(16),\
                        videoFrameRate VARCHAR(16),\
                        resolvingPower VARCHAR(16),\
                        audioBitRate VARCHAR(16),\
                        numberOfChannels INT,\
                        sample_rate VARCHAR(16),\
                        modicationTime DATETIME,\
                        position DOUBLE CHECK(position>=0 AND position<=1),\
                        isValid TINYINT(1))");
    sqlQuery.prepare(sql);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to create table."<<sqlQuery.lastError();
    }else{
        qDebug()<<"creta table videos successfully.";
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

bool mySql::insertAudio(Audio* audio){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("INSERT INTO audios(path,mediaType,duration,audioBitRate,numberOfChannels,sample_rate,modicationTime,position,isValid) VALUES(:path,:mediaType,:duration,:audioBitRate,:numberOfChannels,:sample_rate,:modicationTime,:position,:isValid)");
    sqlQuery.bindValue(":path",audio->getFilePath());
    sqlQuery.bindValue(":mediaType",audio->getMediaType());
    sqlQuery.bindValue(":duration",audio->getDuration());
    sqlQuery.bindValue(":audioBitRate",audio->getAudioBitRate());
    sqlQuery.bindValue(":numberOfChannels",audio->getNumberOfChannels());
    sqlQuery.bindValue(":sample_rate",audio->getSampleRate());
    sqlQuery.bindValue(":modicationTime",audio->getModificationTime().toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":position",audio->getPosition());
    sqlQuery.bindValue(":isValid",audio->isValid());
    if(!sqlQuery.exec()){
        qDebug()<<"fail to insert data."<<sqlQuery.lastError();
        return false;
    }else{
        sqlQuery.prepare("select max(id) from audios");
        sqlQuery.exec();
        int max_id=0;
        while(sqlQuery.next())
        {
            max_id=sqlQuery.value(0).toInt();
            qDebug()<<"查询音频表中max(id)="<<max_id;
        }
        audio->setId(max_id);
        return true;
        //qDebug()<<"insert data successfully";
    }
}

bool mySql::insertVideo(Video *video){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("INSERT INTO videos(path,mediaType,duration,videoBitRate,videoFrameRate,resolvingPower,audioBitRate,numberOfChannels,sample_rate,modicationTime,position,isValid) VALUES(:path,:mediaType,:duration,:videoBitRate,:videoFrameRate,:resolvingPower,:audioBitRate,:numberOfChannels,:sample_rate,:modicationTime,:position,:isValid)");
    sqlQuery.bindValue(":path",video->getFilePath());
    sqlQuery.bindValue(":mediaType",video->getMediaType());
    sqlQuery.bindValue(":duration",video->getDuration());
    sqlQuery.bindValue(":videoBitRate",video->getVideoBitRate());
    sqlQuery.bindValue(":videoFrameRate",video->getVideoFrameRate());
    sqlQuery.bindValue(":resolvingPower",video->getResolvingPower());
    sqlQuery.bindValue(":audioBitRate",video->getAudioBitRate());
    sqlQuery.bindValue(":numberOfChannels",video->getNumberOfChannels());
    sqlQuery.bindValue(":sample_rate",video->getSampleRate());
    sqlQuery.bindValue(":modicationTime",video->getModificationTime().toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":position",video->getPosition());
    sqlQuery.bindValue(":isValid",video->isValid());
    if(!sqlQuery.exec()){
        return false;
        qDebug()<<"fail to insert data."<<sqlQuery.lastError();
    }else{
        sqlQuery.prepare("select max(id) from videos");
        sqlQuery.exec();
        int max_id=0;
        while(sqlQuery.next())
        {
            max_id=sqlQuery.value(0).toInt();
            qDebug()<<"查询视频表中max(id)="<<max_id;
        }
        video->setId(max_id);
        return true;
        //qDebug()<<"insert data successfully";
    }
}

Audio* mySql::selectAudio(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from audios where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select audio."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.size()==0){
            qDebug()<<"not found id:"<<id;
            return NULL;
        }else{
            sqlQuery.first();
            Audio* audio = new Audio();
            audio->setId(sqlQuery.value("id").toInt());
            audio->setFilePath(sqlQuery.value("path").toString());
            audio->setMediaType(sqlQuery.value("mediaType").toString());
            audio->setDuration(sqlQuery.value("duration").toInt());
            audio->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            audio->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            audio->setSampleRate(sqlQuery.value("sample_rate").toString());
            audio->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            audio->setPosition(sqlQuery.value("position").toDouble());
            audio->setIsvalid(sqlQuery.value("isValid").toBool());

            return audio;
        }
    }
}

Video* mySql::selectVideo(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from videos where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select video."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.size()==0){
            qDebug()<<"not found id:"<<id;
            return NULL;
        }else{
            sqlQuery.first();
            Video* video = new Video();
            video->setId(sqlQuery.value("id").toInt());
            video->setFilePath(sqlQuery.value("path").toString());
            video->setMediaType(sqlQuery.value("mediaType").toString());
            video->setDuration(sqlQuery.value("duration").toInt());
            video->setVideoBitRate(sqlQuery.value("videoBitRate").toString());
            video->setVideoFrameRate(sqlQuery.value("videoFrameRate").toString());
            video->setResolvingPower(sqlQuery.value("resolvingPower").toString());
            video->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            video->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            video->setSampleRate(sqlQuery.value("sample_rate").toString());
            video->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            video->setPosition(sqlQuery.value("position").toDouble());
            video->setIsvalid(sqlQuery.value("isValid").toBool());

            return video;
        }
    }
}

Audio* mySql::selectAudioByPath(QString path){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from audios where path=:path");
    sqlQuery.bindValue(":path",path);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select audio."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.size()==0){
            qDebug()<<"not found path:"<<path;
            return NULL;
        }else{
            sqlQuery.first();
            Audio* audio = new Audio();
            audio->setId(sqlQuery.value("id").toInt());
            audio->setFilePath(sqlQuery.value("path").toString());
            audio->setMediaType(sqlQuery.value("mediaType").toString());
            audio->setDuration(sqlQuery.value("duration").toInt());
            audio->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            audio->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            audio->setSampleRate(sqlQuery.value("sample_rate").toString());
            audio->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            audio->setPosition(sqlQuery.value("position").toDouble());
            audio->setIsvalid(sqlQuery.value("isValid").toBool());

            return audio;
        }
    }
}

Video* mySql::selectVideoByPath(QString path){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from videos where path=:path");
    sqlQuery.bindValue(":path",path);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select video."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.size()==0){
            qDebug()<<"not found path:"<<path;
            return NULL;
        }else{
            sqlQuery.first();
            Video* video = new Video();
            video->setId(sqlQuery.value("id").toInt());
            video->setFilePath(sqlQuery.value("path").toString());
            video->setMediaType(sqlQuery.value("mediaType").toString());
            video->setDuration(sqlQuery.value("duration").toInt());
            video->setVideoBitRate(sqlQuery.value("videoBitRate").toString());
            video->setVideoFrameRate(sqlQuery.value("videoFrameRate").toString());
            video->setResolvingPower(sqlQuery.value("resolvingPower").toString());
            video->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            video->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            video->setSampleRate(sqlQuery.value("sample_rate").toString());
            video->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            video->setPosition(sqlQuery.value("position").toDouble());
            video->setIsvalid(sqlQuery.value("isValid").toBool());

            return video;
        }
    }
}

QList<Audio*> mySql::selectAllAudio(){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from audios");
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select data."<<sqlQuery.lastError();
    }else{
        QList<Audio*> qlist;
        while(sqlQuery.next()){
            Audio* audio = new Audio();
            audio->setId(sqlQuery.value("id").toInt());
            audio->setFilePath(sqlQuery.value("path").toString());
            audio->setMediaType(sqlQuery.value("mediaType").toString());
            audio->setDuration(sqlQuery.value("duration").toInt());
            audio->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            audio->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            audio->setSampleRate(sqlQuery.value("sample_rate").toString());
            audio->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            audio->setPosition(sqlQuery.value("position").toDouble());
            audio->setIsvalid(sqlQuery.value("isValid").toBool());

            qlist.push_back(audio);
        }
        return qlist;
    }
}

QList<Video*> mySql::selectAllVideo(){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select * from videos");
    if(!sqlQuery.exec()){
        qDebug()<<"fail to select video."<<sqlQuery.lastError();
    }else{
        QList<Video*> qlist;
        while(sqlQuery.next()){
            Video* video = new Video();
            video->setId(sqlQuery.value("id").toInt());
            video->setFilePath(sqlQuery.value("path").toString());
            video->setMediaType(sqlQuery.value("mediaType").toString());
            video->setDuration(sqlQuery.value("duration").toInt());
            video->setVideoBitRate(sqlQuery.value("videoBitRate").toString());
            video->setVideoFrameRate(sqlQuery.value("videoFrameRate").toString());
            video->setResolvingPower(sqlQuery.value("resolvingPower").toString());
            video->setAudioBitRate(sqlQuery.value("audioBitRate").toString());
            video->setNumberOfChannels(sqlQuery.value("numberOfChannels").toInt());
            video->setSampleRate(sqlQuery.value("sample_rate").toString());
            video->setModificationTime(QDateTime::fromString(sqlQuery.value("modicationTime").toString(),"yyyy-MM-dd hh:mm:ss"));
            video->setPosition(sqlQuery.value("position").toDouble());
            video->setIsvalid(sqlQuery.value("isValid").toBool());

            qlist.push_back(video);
        }
        return qlist;
    }
}




void mySql::selectAllAudioTofileList(std::vector<PlayListNode> &fileList)
{
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select path,duration,id from audios order by id"); //只查询部分列
    if(!sqlQuery.exec())
    {
        qDebug()<<"fail to regenerate fileList."<<sqlQuery.lastError();
    }
    else
    {
        while(sqlQuery.next())  //按顺序加入fileList中
        {
            PlayListNode u;
            u.filePath=sqlQuery.value(0).toString();
            u.duration=sqlQuery.value(1).toInt();
            u.id=sqlQuery.value(2).toInt();
            u.mediaType=1;
            fileList.push_back(u);
        }
    }
}

void mySql::selectAllVideoTofileList(std::vector<PlayListNode> &fileList)
{
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("select path,duration,id from videos order by id"); //只查询部分列
    if(!sqlQuery.exec())
    {
        qDebug()<<"fail to regenerate fileList."<<sqlQuery.lastError();
    }
    else
    {
        while(sqlQuery.next())  //按顺序加入fileList中
        {
            PlayListNode u;
            u.filePath=sqlQuery.value(0).toString();
            u.duration=sqlQuery.value(1).toInt();
            u.id=sqlQuery.value(2).toInt();
            u.mediaType=2;
            fileList.push_back(u);
        }
    }
}

void mySql::modifyAudio(int id,Audio* audio){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("update audios set path=:path,mediaType=:mediaType,duration=:duration,audioBitRate=:audioBitRate,numberOfChannels=:numberOfChannels,sample_rate=:sample_rate,modicationTime=:modicationTime,position=:position,isValid=:isValid where id=:id");
    sqlQuery.bindValue(":path",audio->getFilePath());
    sqlQuery.bindValue(":mediaType",audio->getMediaType());
    sqlQuery.bindValue(":duration",audio->getDuration());
    sqlQuery.bindValue(":audioBitRate",audio->getAudioBitRate());
    sqlQuery.bindValue(":numberOfChannels",audio->getNumberOfChannels());
    sqlQuery.bindValue(":sample_rate",audio->getSampleRate());
    sqlQuery.bindValue(":modicationTime",audio->getModificationTime().toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":position",audio->getPosition());
    sqlQuery.bindValue(":isValid",audio->isValid());

    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to update audios."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"update audios successful";
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::modifyVideo(int id,Video* video){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("update videos set path=:path,mediaType=:mediaType,duration=:duration,videoBitRate=:videoBitRate,videoFrameRate=:videoFrameRate,resolvingPower=:resolvingPower,audioBitRate=:audioBitRate,numberOfChannels=:numberOfChannels,sample_rate=:sample_rate,modicationTime=:modicationTime,position=:position,isValid=:isValid where id=:id");
    sqlQuery.bindValue(":path",video->getFilePath());
    sqlQuery.bindValue(":mediaType",video->getMediaType());
    sqlQuery.bindValue(":duration",video->getDuration());
    sqlQuery.bindValue(":videoBitRate",video->getVideoBitRate());
    sqlQuery.bindValue(":videoFrameRate",video->getVideoFrameRate());
    sqlQuery.bindValue(":resolvingPower",video->getResolvingPower());
    sqlQuery.bindValue(":audioBitRate",video->getAudioBitRate());
    sqlQuery.bindValue(":numberOfChannels",video->getNumberOfChannels());
    sqlQuery.bindValue(":sample_rate",video->getSampleRate());
    sqlQuery.bindValue(":modicationTime",video->getModificationTime().toString("yyyy-MM-dd hh:mm:ss"));
    sqlQuery.bindValue(":position",video->getPosition());
    sqlQuery.bindValue(":isValid",video->isValid());

    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"fail to update videos."<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"update videos successful";
        }else{
            qDebug()<<sqlQuery.lastQuery();
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteAudio(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("delete from audios where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete audio successful";
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteVideo(int id){
    QSqlQuery sqlQuery(this->qdb);
    sqlQuery.prepare("delete from videos where id=:id");
    sqlQuery.bindValue(":id",id);
    if(!sqlQuery.exec()){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete video successful";
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteAllAudio(){
    QSqlQuery sqlQuery(this->qdb);
    if(!sqlQuery.exec("delete from audios")){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete all audios successful.rows:"<<sqlQuery.numRowsAffected();
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteAllVideo(){
    QSqlQuery sqlQuery(this->qdb);
    if(!sqlQuery.exec("delete from videos")){
        qDebug()<<"delete error"<<sqlQuery.lastError();
    }else{
        if(sqlQuery.numRowsAffected()){
            qDebug()<<"delete all videos successful.rows:"<<sqlQuery.numRowsAffected();
        }else{
            qDebug()<<"affect rows: 0";
        }
    }
}

void mySql::deleteTable(QString tableName){
    QSqlQuery sqlQuery(this->qdb);
    if(!sqlQuery.exec("drop table "+tableName)){
        qDebug()<<"drop table "+tableName+" error"<<sqlQuery.lastError();
    }else{
        qDebug()<<"drop table "+tableName+" successful";
    }
}

void mySql::closeDb(){
    qdb.close();
}

//>>>>>>>>>>>>>>>>>>>>>>TEST>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//    mySql sql;
//    if(sql.openDb()){
//        if(!sql.isTableExist("audios")){
//            sql.createTableAudios();
//        }else{
//            qDebug()<<"audios表已存在";
//            //sql.deleteTable("audios");
//        }
//        if(!sql.isTableExist("videos")){
//            sql.createTableVideos();
//        }else{
//            qDebug()<<"videos表已存在";
//            //sql.deleteTable("videos");
//        }
            //insert test
//        Video *video1 = getVideoInfo("C:\\Users\\xgy\\Desktop\\mp3_test\\test1.mp4");
//        Video *video2 = getVideoInfo("C:\\Users\\xgy\\Desktop\\mp3_test\\test2.mp4");
//        sql.insertVideo(video1);  //如果路径重复了，插入失败
//        sql.insertVideo(video2);
//        Audio *audio = getAudioInfo("C:\\Users\\xgy\\Desktop\\mp3_test\\1.mp3");
//        sql.insertAudio(audio);
            //delete test
//        sql.deleteVideo(1);
//        sql.deleteAudio(1);
//        sql.deleteAllAudio();
//        sql.deleteAllVideo();
            //update test
//        Video *video = getVideoInfo("C:\\Users\\xgy\\Desktop\\mp3_test\\test1.mp4");
//        Audio *audio = getAudioInfo("C:\\Users\\xgy\\Desktop\\mp3_test\\1.mp3");
//        sql.insertAudio(audio);
//        sql.insertVideo(video);

//        video->setDuration(12345);
//        sql.modifyVideo(4,video);

//        audio->setDuration(12345);
//        sql.modifyAudio(3,audio);
            //select test
        //Video* video = sql.selectVideo(4);
        //Audio* audio = sql.selectAudio(3);
//        QList<Video*> list = sql.selectAllVideo();
//        QList<Audio*> list2 = sql.selectAllAudio();
//      }



