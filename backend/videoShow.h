#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H
#include <QQuickPaintedItem>
#include <QImage>
#include "backend/mainDecoder.h"
#include "backend/utils.h"
//怎么把视频解析的decoder和video类结合起来


class VideoShow:public QQuickPaintedItem{
    Q_OBJECT
public:
    Q_INVOKABLE void setSize(int width,int height);
    Q_PROPERTY(int nWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int nHeight  READ getHeight WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QString sourPath READ getSourPath WRITE setSourPath NOTIFY sourPathChanged)
    Q_PROPERTY(double process MEMBER m_process READ getmProcess WRITE setmProcess NOTIFY processChanged)  //当前进度条
    
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
    Q_INVOKABLE qint64 getNowProcess(); //获取当前播放的微秒数
    Q_INVOKABLE qint64 getTotalProcess();   //获取当前总时长（微秒数）
    Q_INVOKABLE void setProcess(double process);    //设置进度，传入进度百分比
    static int updateProcess(void* arg);  //获取当前进度（百分比）
    Q_INVOKABLE double getmProcess()const;
    Q_INVOKABLE void setmProcess(double);

    //调整进度
    Q_INVOKABLE void seekFast();
    Q_INVOKABLE void seekSlow();

    //播放（暂时封装）
    Q_INVOKABLE void show(QString path,QString type);

    //截图功能
    Q_INVOKABLE void cutOff();
    Q_INVOKABLE QString getCutPath();
    Q_INVOKABLE void setCutPath(QString path);

    //获取媒体信息
    //Q_INVOKABLE Audio* getMediaObject(QString path);

    //播放状态
    MainDecoder::PlayState getPlayState();

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
    double m_process;
    
public slots:
    //获取图片的信号槽
    void slot_getOneFrame(QImage image);
    //获取当前播放状态
//    void slot_playStateChanged(MainDecoder::PlayState playstate);

signals:
    void widthChanged(int);
    void heightChanged(int);
    void sourPathChanged(QString);
    void processChanged(const double& newprocess);
};

#endif // VIDEOSHOW_H
