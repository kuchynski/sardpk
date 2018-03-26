#ifndef CMONITOR_H
#define CMONITOR_H

#include <QString>
#include "CStackFourth.h"
#include <QWidget>
#include <QPainter>
#include "ccomport.h"
#include "cparam.h"
//#include <QImage>
#include "SStateBloc.h"

#define hieght_image 300
#define width_image 600

class CMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit CMonitor(QWidget *parent = 0);


    void ClearImage();
    void ClearData();
    void SetComPort(CComPort *cp);
    void NewParam();
    void SetVisible(bool v);

private:
    QPainter painter;
    //QImage image;
    QPixmap image;
    CComPort *com_port;
    int timer_id;
    QPointF data[width_image];
    bool paint_all, paint_state, visiable;
    int begin_y[2];
    int end_y[2];
    int size_y[2];
    int center_y[2];
    int begin_x;
    int end_x;
    int size_x;
    int center_x;
//int tmp_ves;

    SElement12<int> data_pb[width_image];
    SElementFourth<int> data_dm[width_image];
    SStateBloc state_bloc;

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
    void timerEvent(QTimerEvent *event);
    void Paint();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
signals:
    void ClickGraph();
    void WheelX(int s);
    void WheelY(int s);
};

#endif // CMONITOR_H
