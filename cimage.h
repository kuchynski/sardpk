#ifndef CIMAGE_H
#define CIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include "CStackFourth.h"
#include "cparam.h"
#define max_size 1000


class CImage : public QWidget
{
    Q_OBJECT
public:
    explicit CImage(QWidget *parent = 0);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);

    void ClearImage();

    int GetWidth()const {return width();}
    int GetHeight()const {return height();}
    QPointF* GetPtrData(const unsigned &n) {return data[n];}
    bool* GetBoolMyVan() {return my_van;}
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);
    void Paint();
private:
    QPointF data[6][max_size];
    bool my_van[max_size];
    QImage image, image_back;
    QPainter painter;
    int begin_mouse_x;
    int begin_mouse_compress_x, begin_mouse_compress_y;

protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseDoubleClickEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
signals:
    void Drag(int s);
    void WheelX(int s);
    void WheelY(int s);
    void IAmReSize();
    void DoubleClick();

public slots:

};

#endif
