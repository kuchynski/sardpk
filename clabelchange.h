#ifndef CLABELCHANGE_H
#define CLABELCHANGE_H

#include <QWidget>
#include <QtGui>

class CLabelChange : public QWidget
{
    Q_OBJECT
public:
    struct SLabelInit
    {
        QFont font;
        QColor color_backgraund;
        QColor color_line;
        QColor color_text;
        SLabelInit() : color_line(Qt::white), color_backgraund(Qt::black), color_text(Qt::white) {}
    };
    explicit CLabelChange(QWidget *parent = 0);

    void SetInitStruct(const SLabelInit &set_struct) {init_struct = set_struct;}
    void AddString(QString str) {strings << str;}
    void SetImageBack(QImage &ib, QRect r1, QRect r2);
    void Paint();
    void Start();
    void Stop();

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
private:
    enum eStyleBack {esColor, esImage};
    eStyleBack style_back;
    SLabelInit init_struct;
    QList<QString> strings;
    unsigned ptr_strings;
    int ptr_timer, state_timer;
    QImage image, image_back;
    QPainter painter;
    int timer_id;

    void timerEvent(QTimerEvent *event);
signals:
    void IAmReSize();

public slots:

};

#endif // CLABELCHANGE_H
