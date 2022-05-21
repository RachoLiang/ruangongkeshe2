QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TRANSLATIONS += tr_Chinese.ts tr_English.ts tr_Japanese.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DrawPosLine.cpp \
    ReverseDecodThread.cpp \
    VideoFrameDisplay.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    DrawPosLine.h \
    ReverseDecodThread.h \
    VideoFrameDisplay.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += $$PWD/ffmpeg/lib/avutil.lib \
        $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avformat.lib \
        $$PWD/ffmpeg/lib/swscale.lib \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/avfilter.lib

#可执行文件名称
TARGET=FFMPEG_ReversePlayer

#版本信息
VERSION = 1.3.4

#图标
RC_ICONS =logo.ico

#公司名称
QMAKE_TARGET_COMPANY = "Jiangxi Shangdao Jingcheng"

#产品名称
QMAKE_TARGET_PRODUCT = "FFMPEG_ReversePlayer"

#文件说明
QMAKE_TARGET_DESCRIPTION = "Qt Creator based on Qt 5.12.6 (MinGW 32 bit)"

#版权信息
QMAKE_TARGET_COPYRIGHT = "Copyright 2021-2026 The Jiangxi Shangdao Jingcheng Company Ltd. All rights reserved."

#中文（简体）
RC_LANG = 0x0004

