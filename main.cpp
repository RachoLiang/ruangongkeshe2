#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QIcon>
#include "backend/PlayList.h"
#include "backend/mainDecoder.h"
#include <QQmlContext>
#include "backend/videoShow.h"
#include "backend/thumbnailShow.h"

#include "backend/videoDecoder.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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

    //test
    AVFormatContext *fmt_ctx = NULL;
    const AVDictionaryEntry *tag = NULL;
    int ret;

    QString path = "C:\\Users\\xgy\\Desktop\\mp3_test\\test1.mp4";

    if ((ret = avformat_open_input(&fmt_ctx, path.toLatin1().data(), NULL, NULL)))
        qDebug()<<"1";

    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        qDebug()<<"2";
    }

    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        qDebug()<<tag->key<<tag->value;

    avformat_close_input(&fmt_ctx);

    return app.exec();
}
