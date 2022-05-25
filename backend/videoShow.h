#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H
#include <QQuickPaintedItem>
#include <QImage>
#include "backend/mainDecoder.h"
//怎么把视频解析的decoder和video类结合起来


class VideoShow:public QQuickPaintedItem{
    Q_OBJECT
public:
    Q_INVOKABLE void setSize(int width,int height);
    Q_PROPERTY(int nWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int nHeight  READ getHeight WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString sourPath READ getSourPath WRITE setSourPath NOTIFY sourPathChanged)
    
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

    //音量信息
    Q_INVOKABLE int getVolume();
    Q_INVOKABLE void setVolume(int);
    Q_INVOKABLE bool isSilence();
    Q_INVOKABLE void silence();

    //暂停信息
    Q_INVOKABLE bool isPaused();
    Q_INVOKABLE void pause();

    //设置倍速播放
    Q_INVOKABLE void setSpeed(double speed);
    Q_INVOKABLE double getSpeed();

    //获取进度条信息
    Q_INVOKABLE qint64 getNowProcess();
    Q_INVOKABLE qint64 getTotalProcess();
    Q_INVOKABLE void setProcess(qint64 process);

    //调整进度
    Q_INVOKABLE void seekFast();
    Q_INVOKABLE void seekSlow();

    //播放（暂时封装）
    Q_INVOKABLE void show(QString path,QString type);

    //调整播放参数，对比度，亮度，饱和度
    Q_INVOKABLE void setArgs(double contrast_per,double brightness_per,double saturation_per);
    
protected:
    //绘制图片
   virtual void paint(QPainter *painter);
    
private:
    QImage image;   //当前播放的图片
    MainDecoder* maindecoder;   //音视频解析器
    int nHeight; //屏幕高
    int nWidth; //屏幕宽
    QString sourPath;   //文件路径
    int lastVolume; //记录静音前的音量
    
public slots:
    //获取图片的信号槽
    void slot_getOneFrame(QImage image);
signals:
    void widthChanged(int);
    void heightChanged(int);
    void sourPathChanged(QString);
};

#endif // VIDEOSHOW_H
