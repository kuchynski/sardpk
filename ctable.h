#ifndef CTABLE_H
#define CTABLE_H

#include <QWidget>
#include <QtGui>

class CTable : public QWidget
{
    Q_OBJECT
public:
    struct STableInit
    {
        unsigned hieght_line;
        QList<unsigned> width_line;
        QFont font;
        int align_flags;
        int r;
        QColor color_backgraund;
        QColor color_backgraund_checked;
        int r2;
        QColor color_backgraund_title;
        QColor color_backgraund_button;
        QColor color_line;
        QColor color_line_checked;
        QColor color_text;
        QColor color_text_checked;
        QColor color_text_title;

        STableInit() : hieght_line(30), font("Courier New", 11, 20),
            color_backgraund_title(Qt::blue), color_backgraund_button(Qt::blue),
            align_flags(Qt::AlignVCenter), color_line(Qt::white), color_backgraund(Qt::black),
            color_text(Qt::white), color_text_title(Qt::white),
            color_line_checked(Qt::white), color_backgraund_checked(Qt::gray), color_text_checked(Qt::white)
        {}
    };
    enum eState {esNo, esFree, esFocus, esChecked};
    struct SButton
    {
        QString name;
        eState state;
        int x1, y1, x2, y2;
        SButton() {}
        SButton(QString na): name(na) {}
        bool IsMy(const int x, const int y) {return (x>=x1)&&(y>=y1)&&(x<x2)&&(y<y2);}
    };
    struct SLine
    {
        QList<QString> field;
        eState state;
        int x1, y1, x2, y2;
        bool repic;
        SLine() {}
        bool IsMy(const int x, const int y) {return (x>=x1)&&(y>=y1)&&(x<x2)&&(y<y2);}
    };
    explicit CTable(QWidget *parent = 0);

    void SetTitle(const QList<QString> &sl, const bool paint = true);
    void SetLine(const QList<QString> &sl, const unsigned &n, const bool paint = false);
    void AddLine(const QList<QString> &sl, const bool paint = false);
    void SubLine(const unsigned &n, const bool paint = false);
    void Clear(const bool paint = false);
    void SetChecked(const unsigned &n);
    unsigned GetSize() {return lines.size();}
    void SetInitStruct(const STableInit &set_struct);
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);// {image_back = ib; style_back = esImage; ReSize();}
    void Paint();
private:
    enum eStyleBack {esColor, esImage};
    eStyleBack style_back;
    STableInit init_struct;
    QImage image, image_back;
    QPainter painter;
    SLine title;
    QList<SLine> lines;
    QList<SButton> buttons;
    bool visiable_title, visiable_buttons;
    int first_line, end_line, kol_line_screen, number_checked_line;
    int timer_id;

    int PaintLine(SLine &line, QPainter &painter, const bool tit = false);
    void MouseMove(const int x, const int y, const bool repic = true);
    void RePaint(const bool all);
    void ReSize();

    bool PlusMinus(int n);

    bool NewStateOnButtons(const unsigned &n);
    void timerEvent(QTimerEvent *event);
protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseDoubleClickEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void keyPressEvent(QKeyEvent *pe);
    virtual void focusInEvent(QFocusEvent *pe);
    virtual void focusOutEvent(QFocusEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

signals:
    void ClickLine(unsigned n);
    void IAmReSize();

public slots:

};

#endif // CTABLE_H
