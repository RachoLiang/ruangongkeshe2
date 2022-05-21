/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *titleLable;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSlider *videoProgressSlider;
    QLabel *labelTime;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnPreview;
    QPushButton *btnPause;
    QPushButton *btnStop;
    QPushButton *btnNext;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit;
    QPushButton *btnOpenUrl;
    QPushButton *btnOpenLocal;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(799, 551);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        titleLable = new QLabel(centralWidget);
        titleLable->setObjectName(QString::fromUtf8("titleLable"));

        verticalLayout_2->addWidget(titleLable);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        videoProgressSlider = new QSlider(centralWidget);
        videoProgressSlider->setObjectName(QString::fromUtf8("videoProgressSlider"));
        videoProgressSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(videoProgressSlider);

        labelTime = new QLabel(centralWidget);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));

        horizontalLayout_3->addWidget(labelTime);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnPreview = new QPushButton(centralWidget);
        btnPreview->setObjectName(QString::fromUtf8("btnPreview"));

        horizontalLayout_2->addWidget(btnPreview);

        btnPause = new QPushButton(centralWidget);
        btnPause->setObjectName(QString::fromUtf8("btnPause"));

        horizontalLayout_2->addWidget(btnPause);

        btnStop = new QPushButton(centralWidget);
        btnStop->setObjectName(QString::fromUtf8("btnStop"));

        horizontalLayout_2->addWidget(btnStop);

        btnNext = new QPushButton(centralWidget);
        btnNext->setObjectName(QString::fromUtf8("btnNext"));

        horizontalLayout_2->addWidget(btnNext);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_4->addWidget(lineEdit);

        btnOpenUrl = new QPushButton(centralWidget);
        btnOpenUrl->setObjectName(QString::fromUtf8("btnOpenUrl"));

        horizontalLayout_4->addWidget(btnOpenUrl);

        btnOpenLocal = new QPushButton(centralWidget);
        btnOpenLocal->setObjectName(QString::fromUtf8("btnOpenLocal"));

        horizontalLayout_4->addWidget(btnOpenLocal);


        verticalLayout->addLayout(horizontalLayout_4);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        titleLable->setText(QString());
        labelTime->setText(QString());
        btnPreview->setText(QString());
        btnPause->setText(QString());
        btnStop->setText(QString());
        btnNext->setText(QString());
        btnOpenUrl->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\347\275\221\347\273\234\346\226\207\344\273\266", nullptr));
        btnOpenLocal->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\234\254\345\234\260\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
