TEMPLATE = app
TARGET = musicplayer
QT += quick quickcontrols2
QT += core gui multimedia network
QT += multimediawidgets
QT += sql

SOURCES += \
    back/PlayList.cpp \
    back/audio.cpp \
    back/captions.cpp \
    back/mySql.cpp \
    back/playerThread.cpp \
    back/records.cpp \
    back/vedio.cpp \
    back/myStack.cpp \
    musicplayer.cpp

RESOURCES += \
    icons/icons.qrc \
    images/album-cover.jpg \
    imagine-assets/imagine-assets.qrc \
    qtquickcontrols2.conf \
    musicplayer.qml
    Headers/

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/imagine/musicplayer
INSTALLS += target

HEADERS += \
    backend/PlayList.h \
    backend/audio.h \
    backend/caption.h \
    backend/mySql.h \
    backend/myStack.h \
    backend/playerThread.h \
    backend/records.h \
    backend/vedio.h \

DISTFILES += \
    CMakeLists.txt

