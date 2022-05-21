#ifndef DRAWPOSLINE_H
#define DRAWPOSLINE_H

#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QColor>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDebug>

class DrawPosLine : public QFrame
{
    Q_OBJECT
public:
    DrawPosLine(QWidget *parent = nullptr);
    ~DrawPosLine();
protected:
    //窗口绘制事件
    void paintEvent(QPaintEvent *e);
};
#endif // DrawPosLine_H
