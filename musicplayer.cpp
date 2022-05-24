﻿/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QIcon>
//#include "backend/mySql.h"
#include "backend/PlayList.h"
#include "backend/mainDecoder.h"
#include <QQmlContext>
#include "backend/videoShow.h"

void initFFmpeg()
{
//    av_log_set_level(AV_LOG_INFO);

    //avfilter_register_all();

    /* ffmpeg init */
    //av_register_all();

    /* ffmpeg network init for rtsp */
    if (avformat_network_init()) {
        qDebug() << "avformat network init failed";
    }

    /* init sdl audio */
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        qDebug() << "SDL init failed";
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("Music Player");
    QGuiApplication::setOrganizationName("QtProject");

    QGuiApplication app(argc, argv);
    qmlRegisterType<PlayList>("playlistclass",1,0,"PlayList");//注册播放列表类，注意，第四个参数必须是大写开头！
    QIcon::setThemeName("musicplayer");

    QQmlApplicationEngine engine;

    //注册videoShow类
    qmlRegisterType<VideoShow>("VideoShow",1,0,"VideoShow");

    engine.load(QUrl("qrc:/QML/content/App.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;



//    mySql sql;
//    if(sql.openDb()){
//        if(sql.isTableExist("records")){
//            //nothing
//        }else{
//            sql.createTable();
//        }
//        //插入
//        Record record = {1,"/100.mp3","test",100,"mp3",QDateTime::currentDateTime(),0};
//        sql.insertData(record);
//        //查询
//        Record rd = sql.selectData(1);
//        qDebug()<<rd.id<<rd.path<<rd.playTime<<rd.progress<<rd.time<<rd.title<<rd.type;
//        //查询所有数据
//        QList<Record> qlist = sql.selectall();
//        for(int i=0;i<qlist.size();i++){
//            Record rd = qlist[i];
//            qDebug()<<rd.id<<rd.path<<rd.playTime<<rd.progress<<rd.time<<rd.title<<rd.type;
//        }
//        //sql.deleteAll();
//        sql.closeDb();
//    }
//    initFFmpeg();
//    MainDecoder mainDecoder;
//    QString path = "C:/Users/xgy/Desktop/mp3_test/test.mp4";
//    QString type = "video";
//    mainDecoder.decodeFile(path,type);

    /**
     * 测试视频播放
    */
//    QObject *pRoot = engine.rootObjects().first();
//    Video* player = new Video("D:/chat/wechat/WeChat Files/wxid_mnkxh12c0w5n22/FileStorage/Video/2022-04/0b6d50c476ea6be54ec82e2790ad3f90.mp4");
//    QAudioOutput* audioOutput = new QAudioOutput;
//    QVideoWidget* videoOutput = new QVideoWidget;
//    player->setAudioOutput(audioOutput);
//    player->setVideoOutput(videoOutput);
//    QObject::connect(player->getPlayer(), SIGNAL(positionChanged(qint64)), pRoot, SLOT(positionChanged(qint64)));
//    player->onPlay();
    return app.exec();
}
