TEMPLATE = app
TARGET = musicplayer
QT += quick quickcontrols2
QT += core gui multimedia network
QT += multimediawidgets

SOURCES += \
    back/audio.cpp \
    back/captions.cpp \
    back/mySql.cpp \
    back/playerThread.cpp \
    back/records.cpp \
    back/vedio.cpp \
    back/vedioManage.cpp \
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
    backend/audio.h \
    backend/captions.h \
    backend/mySql.h \
    backend/playerThread.h \
    backend/records.h \
    backend/vedio.h \
    backend/vedioManage.h \

