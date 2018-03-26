#ifndef CBDIMAGE_H
#define CBDIMAGE_H

//#include<QIODevice>
#include <QString>
#include <QStringList>
#include "cimage.h"
#include "cimagedefect.h"
#include "CSostav.h"
#include <QWidget>
#include "cbuttons.h"
#include "cchecks.h"
#include "cbuttonscheck.h"

class CBDImage: public QWidget
{
    Q_OBJECT
private:
    unsigned side;
    CChecks check_pb;
    CImage image;
    CButtonsCheck check_axel;
    CImageDefect image_defect;
    QLabel label_type_of_van;

    CSostav sostav;

    int dial_arch;
    QImage image_back;
    QPainter painter;
    unsigned activ_axel;

    void Clear();
    void PaintGridSostav();
public:
    explicit CBDImage(QWidget *parent = 0);

    void RePicGraph();
    void SetSostav(CSostav &s);
    void CloseSostav();
    void SetActivVan(unsigned row = 1000);
    void SetSide(unsigned s);


    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);
    void resizeEvent(QResizeEvent *r);
public slots:
    void OnCheckedPB(unsigned n, bool s);
    void OnCheckedAxel(unsigned n, bool s);
    void DragImage(int s);
    void CompressXImage(int s);
    void CompressYImage(int s);
    void DoubleClickImage();
    void OnReSizeImage();
    void OnReSizeCheck();
    void OnReSizeCheckAxel();
    void OnReSizeImageDefect();
signals:
    void IAmReSize();
    void DefectForButtonSide(bool s1, bool s2);
};

#endif
