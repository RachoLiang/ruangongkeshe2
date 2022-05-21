#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ReverseDecodThread.h"
#include <QFileDialog>
#include <QStyle>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_play_clicked();
    void slotGetCurrentTime(qint64);
    void on_pushButton_pause_clicked();
    void on_pushButton_stop_clicked();

    void on_pushButton_Set_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::Widget *ui;

    ReverseDecodThread DecodeWorkThread;

    qint64 duration;


};
#endif // WIDGET_H
