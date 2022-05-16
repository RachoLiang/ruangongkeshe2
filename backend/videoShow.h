#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H
#include <QQuickPaintedItem>
#include <QImage>
#include "backend/mainDecoder.h".h"
//怎么把视频解析的decoder和video类结合起来


class VideoShow:public QQuickPaintedItem{
    Q_OBJECT
public:
    Q_INVOKABLE void setSize(int width,int height);
    Q_PROPERTY(int nWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int nHeight  READ getHeight WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString sourPath READ setSourPath WRITE setSourPath NOTIFY sourPathChanged)
    
    VideoShow();
    explicit VideoShow(QString path);
    ~VideoShow();
    
    //资源路径
    QString getSourPath();
    void setSourPath(QString);
    
    //设置框的大小
    void setWidth(int);
    void setHeight(int);
    
    //获取框的大小
    int getWidth();
    int getHeight();
    
protected:
    //绘制图片
   virtual void paint(QPainter *painter);
    
private:
    QImage image;   //当前播放的图片
    MainDecoder* maindecoder;   //音视频解析器
    int nHeight; //屏幕高
    int nWidth; //屏幕宽
    QString sourPath;   //文件路径
    
public slots:
    //获取图片的信号槽
    void slot_getOneFrame(QImage* image);
signals:
    void widthChanged(int);
    void heightChanged(int);
    void sourPathChanged(QString);
};

#endif // VIDEOSHOW_H
