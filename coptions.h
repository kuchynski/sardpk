#ifndef COPTIONS_H
#define COPTIONS_H

#include <QWidget>
#include <QtGui>
#include "cparam.h"
#include "cchecks.h"
#include "cspinbox.h"
#include "cpboptions.h"
#include "clabelchange.h"

class COptions : public QWidget
{
    Q_OBJECT
private:
    QPainter painter;
    QImage image_my;

    CChecks *buttons_enabled_pb[12];
    CSpinBox *spin_box_rast[3];
    CPBOptions *pb_options[12];
    CLabelChange label_about;

    void SetKoof(unsigned pb, unsigned n, int value);

public:
    explicit COptions(QWidget *parent = 0);

    void NewKoof();
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
signals:

public slots:
    void OnCheckedNSPB0(bool);
    void OnCheckedNSPB1(bool);
    void OnCheckedNSPB2(bool);
    void OnCheckedNSPB3(bool);
    void OnCheckedNSPB4(bool);
    void OnCheckedNSPB5(bool);
    void OnCheckedNSPB6(bool);
    void OnCheckedNSPB7(bool);
    void OnCheckedNSPB8(bool);
    void OnCheckedNSPB9(bool);
    void OnCheckedNSPB10(bool);
    void OnCheckedNSPB11(bool);
    void OnCheckedEnabledPb0(unsigned n, bool s);
    void OnCheckedEnabledPb1(unsigned n, bool s);
    void OnCheckedEnabledPb2(unsigned n, bool s);
    void OnCheckedEnabledPb3(unsigned n, bool s);
    void OnCheckedEnabledPb4(unsigned n, bool s);
    void OnCheckedEnabledPb5(unsigned n, bool s);
    void OnCheckedEnabledPb6(unsigned n, bool s);
    void OnCheckedEnabledPb7(unsigned n, bool s);
    void OnCheckedEnabledPb8(unsigned n, bool s);
    void OnCheckedEnabledPb9(unsigned n, bool s);
    void OnCheckedEnabledPb10(unsigned n, bool s);
    void OnCheckedEnabledPb11(unsigned n, bool s);
    void OnRastA0ValueChanged(int value);
    void OnRastA1ValueChanged(int value);
    void OnRastA2ValueChanged(int value);
    void OnChangedKoofPB0(unsigned n, int value);
    void OnChangedKoofPB1(unsigned n, int value);
    void OnChangedKoofPB2(unsigned n, int value);
    void OnChangedKoofPB3(unsigned n, int value);
    void OnChangedKoofPB4(unsigned n, int value);
    void OnChangedKoofPB5(unsigned n, int value);
    void OnChangedKoofPB6(unsigned n, int value);
    void OnChangedKoofPB7(unsigned n, int value);
    void OnChangedKoofPB8(unsigned n, int value);
    void OnChangedKoofPB9(unsigned n, int value);
    void OnChangedKoofPB10(unsigned n, int value);
    void OnChangedKoofPB11(unsigned n, int value);
};

#endif // COPTIONS_H
