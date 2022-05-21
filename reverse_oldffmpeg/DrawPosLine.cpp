#include "DrawPosLine.h"

DrawPosLine::DrawPosLine(QWidget *parent)
    : QFrame(parent)
{

}

DrawPosLine::~DrawPosLine()
{

}


/*
工程: DrawPosLine
日期: 2020-12-23
环境: win10 QT5.12.6 MinGW32
功能: 窗口绘制事件
*/
void DrawPosLine::paintEvent(QPaintEvent *e)
{
    qDebug()<<"窗口绘制事件";

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QBrush posBrush(QColor("#00BFFF"));

    //设置矩形框的坐标尺寸
    QRectF posRect;

    posRect.setRect(6,0,100,this->height());
    painter.setBrush(posBrush); //设置画刷
    painter.setPen(Qt::NoPen); //不设置画笔,不绘制边界线
    painter.drawRect(posRect); //绘制矩形
}

