﻿#include "VideoFrameDisplay.h"

#include <QPainter>

VideoFrameDisplay::VideoFrameDisplay(QWidget *parent) :
    QWidget(parent)
{
    m_nRotateDegree=0;
    this->setMouseTracking(true);
}

VideoFrameDisplay::~VideoFrameDisplay()
{

}

void VideoFrameDisplay::Set_Rotate(int Rotate)
{
    m_nRotateDegree=Rotate;
}

void VideoFrameDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	
	painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
	
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    //将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); //这里效率比较低下  还不知道如何优化

    //画面旋转
    if(m_nRotateDegree > 0)
    {
        QTransform matrix;
        matrix.rotate(m_nRotateDegree);
        img = img.transformed(matrix, Qt::SmoothTransformation);
    }

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像
}


void VideoFrameDisplay::slotSetOneFrame(QImage img)
{
    src_mImage =mImage = img;
    update(); //调用update将执行 paintEvent函数
}

/*
工程: QtAV_VideoPlayer
日期: 2021-03-24
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 获取原图数据
*/
QImage VideoFrameDisplay::GetImage()
{
    return src_mImage.copy();
}

/*
工程: QtAV_VideoPlayer
日期: 2021-03-25
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 鼠标双击事件
*/
void VideoFrameDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit s_VideoWidgetEvent(1);
}
