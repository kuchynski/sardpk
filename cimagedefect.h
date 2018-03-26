#ifndef CIMAGEDEFECT_H
#define CIMAGEDEFECT_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include "CStackFourth.h"
#include "cparam.h"
#define max_size 1000


class CImageDefect : public QWidget
{
    Q_OBJECT
public:
    explicit CImageDefect(QWidget *parent = 0);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);

    void ClearImage();

    unsigned GetWidth()const {return width()/2;}
    unsigned GetHeight()const {return height()/3;}
    QPointF* GetPtrData1(const unsigned &n) {return data1[n];}
    QPointF* GetPtrData2(const unsigned &n) {return data2[n];}
    void SetPorog(const unsigned &n, const unsigned &p) {if(n < 6) porog[n] = p;}
    void SetDefectSize(const unsigned &n, const unsigned &p) {if(n < 6) defect_size[n] = p;}
    void SetTare(const unsigned &n, const unsigned &p) {if(n < 6) tare[n] = p;}
    void SetPbNumber(const unsigned &n, const unsigned &p) {if(n < 6) pb_number[n] = p;}
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);
    void Paint();
private:
    QImage image, image_back;
    QPointF data1[6][max_size];
    QPointF data2[6][max_size];
    unsigned porog[6];
    unsigned defect_size[6];
    unsigned tare[6];
    unsigned pb_number[6];
    QPainter painter;
protected:
signals:
    void IAmReSize();

public slots:

};

#endif
