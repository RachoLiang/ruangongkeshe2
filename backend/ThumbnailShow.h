#ifndef THUMBNAILSHOW_H
#define THUMBNAILSHOW_H

#include <QQuickPaintedItem>
#include <QImage>
#include "backend/videoDecoder.h"

class ThumbnailShow:public QQuickPaintedItem{

    Q_OBJECT
public:
    Q_PROPERTY(int nWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int nHeight  READ getHeight WRITE setHeight NOTIFY heightChanged)

    explicit ThumbnailShow();
    ~ThumbnailShow();

    //设置框的大小
    void setWidth(int);
    void setHeight(int);

    //获取框的大小
    int getWidth();
    int getHeight();

    //设置video路径并开启线程
    Q_INVOKABLE void setPathAndStart(QString path);
    Q_INVOKABLE void getFrame(double percent);

protected:
    //绘制图片
   virtual void paint(QPainter *painter);

private:
    QImage image;   //当前播放的图片
    VideoDecoder *videoDecoder;//缩略图解析器
    int nHeight; //屏幕高
    int nWidth; //屏幕宽

public slots:
    //获取图片的信号槽
    void slot_getOneFrame(QImage image);
};

#endif // THUMBNAILSHOW_H
