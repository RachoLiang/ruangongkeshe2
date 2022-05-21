#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->horizontalSlider_time->installEventFilter(this);

    //关联视频解码器
    connect(&DecodeWorkThread, SIGNAL(SendOneFrame(QImage)), ui->widget_video, SLOT(slotSetOneFrame(QImage)));

    //当前时间
    connect(&DecodeWorkThread, SIGNAL(positionChanged(qint64)), this, SLOT(slotGetCurrentTime(qint64)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotGetCurrentTime(qint64 pts)
{
    qDebug()<<"pts:"<<pts;

    ui->horizontalSlider_time->setValue(pts);

    ui->label_duration->setText(QString("%1/%2").arg(pts).arg(duration));
}


void Widget::on_pushButton_play_clicked()
{
    DecodeWorkThread.SetSate(0);
    DecodeWorkThread.quit();
    DecodeWorkThread.wait();

    DecodeWorkThread.SetSate(2);

    QString filename = QFileDialog::getOpenFileName(this, "选择打开的文件", "C:/", tr("*.*"));
    DecodeWorkThread.LoadVideoFile(filename);
    duration=DecodeWorkThread.GetDuration(); //获取总时间
    ui->horizontalSlider_time->setMaximum(duration);  //设置最大值
    ui->horizontalSlider_time->setMinimum(0); //设置最小值
    ui->horizontalSlider_time->setValue(duration);
    DecodeWorkThread.SetSeekPos(ui->horizontalSlider_time->value());
    DecodeWorkThread.start();
}


void Widget::on_pushButton_pause_clicked()
{
    if (DecodeWorkThread.GetSate() == 2)
    {
        DecodeWorkThread.SetSate(1);
    }
    else if(DecodeWorkThread.GetSate() == 1)
    {
        DecodeWorkThread.SetSate(2);
    }
}


void Widget::on_pushButton_stop_clicked()
{
     DecodeWorkThread.SetSate(0);
}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    //解决QSlider点击不能到鼠标指定位置的问题
    if(obj==ui->horizontalSlider_time)
    {
        if (event->type()==QEvent::MouseButtonPress) //判断类型
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)	//判断左键
            {
               int value = QStyle::sliderValueFromPosition(ui->horizontalSlider_time->minimum(), ui->horizontalSlider_time->maximum(), mouseEvent->pos().x(), ui->horizontalSlider_time->width());
               ui->horizontalSlider_time->setValue(value);

               qDebug()<<"value:"<<value;
               //跳转到指定位置
               DecodeWorkThread.SetSeekPos(ui->horizontalSlider_time->value());
            }
        }
    }
    return QObject::eventFilter(obj,event);
}


/*
工程: FFMPE_ReversePlay
日期: 2021-04-19
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 设置解码时间段
*/
void Widget::on_pushButton_Set_clicked()
{

}

