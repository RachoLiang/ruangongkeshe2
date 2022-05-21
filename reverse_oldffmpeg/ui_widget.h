/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "drawposline.h"
#include "videoframedisplay.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_duration;
    VideoFrameDisplay *widget_video;
    QSlider *horizontalSlider_time;
    DrawPosLine *frame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_play;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_stop;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit_startTime;
    QLabel *label_2;
    QLineEdit *lineEdit_EndTime;
    QPushButton *pushButton_Set;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(715, 454);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_duration = new QLabel(Widget);
        label_duration->setObjectName(QString::fromUtf8("label_duration"));
        label_duration->setMaximumSize(QSize(16777215, 40));

        horizontalLayout->addWidget(label_duration);


        verticalLayout->addLayout(horizontalLayout);

        widget_video = new VideoFrameDisplay(Widget);
        widget_video->setObjectName(QString::fromUtf8("widget_video"));

        verticalLayout->addWidget(widget_video);

        horizontalSlider_time = new QSlider(Widget);
        horizontalSlider_time->setObjectName(QString::fromUtf8("horizontalSlider_time"));
        horizontalSlider_time->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider_time);

        frame = new DrawPosLine(Widget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(0, 1));
        frame->setMaximumSize(QSize(16777215, 1));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_play = new QPushButton(Widget);
        pushButton_play->setObjectName(QString::fromUtf8("pushButton_play"));

        horizontalLayout_2->addWidget(pushButton_play);

        pushButton_pause = new QPushButton(Widget);
        pushButton_pause->setObjectName(QString::fromUtf8("pushButton_pause"));

        horizontalLayout_2->addWidget(pushButton_pause);

        pushButton_stop = new QPushButton(Widget);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));

        horizontalLayout_2->addWidget(pushButton_stop);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit_startTime = new QLineEdit(Widget);
        lineEdit_startTime->setObjectName(QString::fromUtf8("lineEdit_startTime"));

        horizontalLayout_3->addWidget(lineEdit_startTime);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_EndTime = new QLineEdit(Widget);
        lineEdit_EndTime->setObjectName(QString::fromUtf8("lineEdit_EndTime"));

        horizontalLayout_3->addWidget(lineEdit_EndTime);

        pushButton_Set = new QPushButton(Widget);
        pushButton_Set->setObjectName(QString::fromUtf8("pushButton_Set"));

        horizontalLayout_3->addWidget(pushButton_Set);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_duration->setText(QCoreApplication::translate("Widget", "00/00", nullptr));
        pushButton_play->setText(QCoreApplication::translate("Widget", "\350\275\275\345\205\245\350\247\206\351\242\221\346\226\207\344\273\266", nullptr));
        pushButton_pause->setText(QCoreApplication::translate("Widget", "\346\232\202\345\201\234\344\270\216\347\273\247\347\273\255", nullptr));
        pushButton_stop->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\350\265\267\345\247\213\346\227\266\351\227\264:", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\347\273\223\346\235\237\346\227\266\351\227\264:", nullptr));
        pushButton_Set->setText(QCoreApplication::translate("Widget", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
