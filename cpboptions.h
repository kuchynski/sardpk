#ifndef CPBOPTIONS_H
#define CPBOPTIONS_H

#include <QWidget>
#include "cchecks.h"
#include "cspinbox.h"
#include "cparam.h"

class CPBOptions : public QWidget
{
    Q_OBJECT
private:
    QPainter painter;
    QImage image_my;
    bool enabled_ns;

    QString name;
    QLabel *label_sb[3];
    CSpinBox *spin_box_koof[2];
    CChecks *button_ns;
    int max_height;
    int sm_height, sm_height_end, sm_height_step;
    int timer_id;
    bool visiable;

    void SetGeometry();
public:
    explicit CPBOptions(bool ens, int h, QString n, QWidget *parent = 0);

    void SetKoof(unsigned a, unsigned b, int c);
    void SetNS(bool c);
    void SetVisible(bool v);
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
    void timerEvent(QTimerEvent *event);
signals:
    void ChangeKoof(unsigned n, int k);
    void CheckedNSPB(bool);

public slots:
    void OnKoofAChanged(int value);
    void OnKoofBChanged(int value);
    void OnCheckedNSPB(unsigned,bool);
};

#endif // CPBOPTIONS_H
