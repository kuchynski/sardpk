#ifndef CManyWindows_H
#define CManyWindows_H

#include <QWidget>
#include <QtGui>
#include "cbuttonscheck.h"
#include "CSostav.h"


class CManyWindows : public QWidget
{
    Q_OBJECT
public:
    struct SAllWindowsInit
    {
        enum eDirection {edVertical, edHorizontal};

        eDirection direction_frame;
        eDirection direction_widget;
        unsigned rewindow_time_all;
        unsigned rewindow_kol_step;
        unsigned frame_height;
        unsigned frame_width;
        SAllWindowsInit(): rewindow_time_all(700), rewindow_kol_step(10), frame_height(30), frame_width(30),
                           direction_frame(edHorizontal), direction_widget(edHorizontal) {}
    };

    explicit CManyWindows(QWidget *parent = 0);

    void SetInitStruct(const SAllWindowsInit &set_struct);
    void AddWidget(QWidget *w, QString name);
    void AddWidget(QWidget *w, QString name, QPixmap &pix);
    void RePlaceWidget(unsigned n, QWidget *w, QString name, QPixmap &pix);
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
private:

    QBoxLayout *box_layout;
    QList<QWidget*> list_windows;
    QWidget *main_widget;
    SAllWindowsInit init_struct;
    unsigned tek, new_tek;
    int mouse_press_x, mouse_press_y;
    bool mouse_press;
    CButtonsCheck buttons;

    int timer_id, timer_st;

    void SetWidget(const unsigned &n);
    void timerEvent(QTimerEvent *event);

    void ChoiceStart();
    void ChoiceEnd();

protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);

signals:
    void NewWindow(unsigned n);

private slots:
    void OnButtonsClick(unsigned n, bool s = true);

public slots:
    virtual void OnNewSostav(){}
    virtual void OnPressButton(unsigned n){}
    virtual void OnBeginTarirovka(CSostav &s){}
    virtual void OnEndTarirovka(CSostav &s, bool res){}
    virtual void OnNewKoof(SElementFourth<unsigned> a, SElementFourth<unsigned> b, SElementFourth<int> c){}
};

#endif // CManyWindows_H
