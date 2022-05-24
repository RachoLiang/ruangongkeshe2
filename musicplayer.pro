TEMPLATE = app
TARGET = musicplayer
QT += quick quickcontrols2
QT += core gui multimedia network
QT += multimediawidgets
QT += sql

SOURCES += \
    back/PlayList.cpp \
    back/audio.cpp \
    back/audioDecoder.cpp \
    back/avPacketQueue.cpp \
    back/captions.cpp \
    back/mainDecoder.cpp \
    back/mySql.cpp \
    back/playerThread.cpp \
    back/records.cpp \
    back/vedio.cpp \
    back/myStack.cpp \
    back/videoShow.cpp \
    main.cpp

HEADERS += \
    backend/PlayList.h \
    backend/PlayListNode.h \
    backend/audio.h \
    backend/audioDecoder.h \
    backend/avPacketQueue.h \
    backend/caption.h \
    backend/mainDecoder.h \
    backend/mySql.h \
    backend/myStack.h \
    backend/playerThread.h \
    backend/records.h \
    backend/vedio.h \
    backend/videoShow.h

INCLUDEPATH += $$PWD/ffmpeg/include \
                $$PWD/sdl/include

LIBS    += $$PWD/ffmpeg/lib/avcodec.lib \
            $$PWD/ffmpeg/lib/avdevice.lib \
            $$PWD/ffmpeg/lib/avfilter.lib \
            $$PWD/ffmpeg/lib/avformat.lib \
            $$PWD/ffmpeg/lib/avutil.lib \
            $$PWD/ffmpeg/lib/postproc.lib \
            $$PWD/ffmpeg/lib/swresample.lib \
            $$PWD/ffmpeg/lib/swscale.lib \
            $$PWD/sdl/lib/libSDL2.a


resources.files = qml/
resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = imports/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

