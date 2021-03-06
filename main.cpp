#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QIcon>
#include "backend/PlayList.h"
#include "backend/mainDecoder.h"
#include <QQmlContext>
#include "backend/videoShow.h"
#include "backend/thumbnailShow.h"

#include "backend/mySql.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<PlayList>("playlistclass",1,0,"PlayList");
    qmlRegisterType<VideoShow>("VideoShow",1,0,"VideoShow");
    qmlRegisterType<ThumbnailShow>("ThumnailShow",1,0,"ThumbnailShow"); 

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/musicplayer/qml/imports");
    const QUrl url(u"qrc:/musicplayer/qml/content/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
