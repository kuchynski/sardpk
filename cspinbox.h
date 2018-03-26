#ifndef CSPINBOX_H
#define CSPINBOX_H

#include <QWidget>
#include <QtGui>

class CSpinBox : public QWidget
{
    Q_OBJECT
public:
    struct SSpinBoxInit
    {
        QFont font;
        int align_flags;
        int height_buttons;
        QColor color_backgraund;
        QColor color_backgraund_buttons;
        QColor color_backgraund_checked;
        QColor color_line;
        QColor color_text;

        SSpinBoxInit() : font("Courier New", 11, 20), height_buttons(20),
            align_flags(Qt::AlignCenter), color_line(Qt::white),
            color_backgraund(Qt::gray), color_backgraund_buttons(Qt::black),
            color_text(Qt::white), color_backgraund_checked(Qt::gray)
        {}
    };
    enum eState {esFree, esFocus, esChecked};
    struct SButton
    {
        eState state;
        int x1, y1, x2, y2;
        SButton():state(esFree), x1(0), x2(0), y1(0), y2(0) {}
        bool IsMy(const int x, const int y) {return (x>=x1)&&(y>=y1)&&(x<x2)&&(y<y2);}
    };
    explicit CSpinBox(QWidget *parent = 0);

    void SetValue(const int &v);
    int GetValue() {return value;}
    void SetMin(const int &mi) {min = mi; OnNewValue();}
    void SetMax(const int &ms) {max = ms; OnNewValue();}
    void SetMinMax(const int &mi, const int &ms) {min = mi; SetMax(ms);}
    void SetStepSmall(const int &s) {step_small = s;}
    void SetStepLage(const int &s) {step_lage = s;}
    bool OnNewValue();
    void SetInitStruct(const SSpinBoxInit &set_struct) {init_struct = set_struct;}
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
    void Paint();
private:
    SSpinBoxInit init_struct;
    QPainter painter;
    QImage image;
    int value, new_value;
    int min, max;
    int step_small, step_lage;
    QList<SButton> buttons;
    int timer_id;
    bool is_my_focus;

    bool NewStateOnButtons(const unsigned &n);
    void timerEvent(QTimerEvent *event);
protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

signals:
    void NewValue(int v);
public slots:
};

#endif // CSPINBOX_H
