#ifndef CCHECKS_H
#define CCHECKS_H

#include <QWidget>
#include <QtGui>

class CChecks : public QWidget
{
    Q_OBJECT
public:
    struct SButtonsInit
    {
        enum eBackground {ebColor, ebPicture};
        enum eDirection {edVertical, edHorizontal};
        enum eSize {esFixedSize, esCompressSize};

        unsigned hieght_button;
        unsigned width_button;
        QFont font;
        int align_flags;
        eDirection direction;
        eSize size_horizontal;
        eSize size_vertical;
        int r;
        QColor color_backgraund;
        QColor color_backgraund_checked;
        QColor color_backgraund2;
        QColor color_backgraund_checked2;
        QColor color_line;
        QColor color_text;
        QColor color_text_checked;
        eBackground style_background;

        SButtonsInit() :
            hieght_button(-1), width_button(-1), direction(edVertical), font("Courier New", 11, 20),
            style_background(ebColor),
            color_backgraund(Qt::blue), color_backgraund_checked(Qt::white),
            color_backgraund2(Qt::red), color_backgraund_checked2(Qt::yellow),
            color_text_checked(Qt::yellow),
            size_horizontal(esCompressSize), size_vertical(esCompressSize), align_flags(Qt::AlignCenter),
            color_line(Qt::white), color_text(Qt::white) {}
    };
    enum eState {esFree, esFocus, esChecked, esCheckedFocus};
    struct SButton
    {
        QString name;
        QImage image;
        QImage image_check;
        eState state;
        eState state2;
        int x1, y1, x2, y2;
        SButton(QString na): name(na) {}
        bool IsMy(const int x, const int y) {return (x>=x1)&&(y>=y1)&&(x<x2)&&(y<y2);}
    };
    explicit CChecks(QWidget *parent = 0);

    void AddButton(SButton sb, const int n = -1);
    void AddButton(SButton sb, QImage &i, QImage &ic, const int n = -1);
    void SubButton(const int n = -1);
    unsigned GetSize() const {return buttons.size();}
    void RenameButton(const QString str, const unsigned &n);
    void SetInitStruct(const SButtonsInit &set_struct);
    void SetChecked(const unsigned &n, const bool &state);
    void SetChecked2(const unsigned &n, const bool &state);
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
    void Paint();
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);// {image_back = ib; style_back = esImage; ReSize();}
private:
    enum eStyleBack {esColor, esImage};
    eStyleBack style_back;

    SButtonsInit init_struct;
    QImage image, image_back;
    QPainter painter;
    bool mouse_press;

    void ReSize();
protected:
    QList<SButton> buttons;
    bool focus;

    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
signals:
    void PressButton(unsigned n, bool state);
    void IAmReSize();

public slots:

};

#endif // CCHECKS_H
