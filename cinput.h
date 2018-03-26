#ifndef CINPUT_H
#define CINPUT_H

#include <QWidget>
#include<QDial>
#include <QtGui>
#include "cmonitor.h"
#include "ccomport.h"
#include "cbuttonscheck.h"

class CInput : public QWidget
{
    Q_OBJECT
private:
    CButtonsCheck check_dat1, check_dat2;
    CButtonsCheck check_port;
    CComPort com_port;
    CMonitor monitor;
    QPainter painter;
    QImage image_my;
    QLabel *label_com_state;
    QProcess *process_reload;
    bool visiable;

    bool now_exit;
    QString dir_path;
    int timer_new_second;
    void timerEvent(QTimerEvent *event) {com_port.SetNewSecond();}
    void OnReSizeCheckDat1();
    void OnReSizeCheckDat2();
    void OnReSizeCheckPort();
public:
    explicit CInput(QWidget *parent = 0);
    ~CInput();

    void GetSostav(CSostav &s) {com_port.GetSostav(s);}
    void SetVisiable(bool v);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
signals:

public slots:
    void OnNewComPortState(unsigned s);
    void OnNewSostav();
    void OnEndReload(int res);
    void on_checked_dat1Changed(unsigned n, bool tmp);
    void on_checked_dat2Changed(unsigned n, bool tmp);
    void on_checked_portChanged(unsigned n, bool tmp);
    void on_center_monitor_clicked();
    void on_dial_x_monitor_valueChanged(int value);
    void on_dial_y_monitor_valueChanged(int value);
signals:
    void NewSostav();
};

#endif // CINPUT_H
