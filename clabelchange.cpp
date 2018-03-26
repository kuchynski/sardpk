#include "clabelchange.h"

CLabelChange::CLabelChange(QWidget *parent) : QWidget(parent), ptr_strings(0), style_back(esColor),
    ptr_timer(0), state_timer(0), image(width(), height(), QImage::Format_RGB32)
{
}
void CLabelChange::Paint()
{
    painter.begin(&image);
    QRect rect_all = image.rect();
    painter.setRenderHint(QPainter::Antialiasing, true);
    if(style_back == esColor)
    {
        painter.setBrush(init_struct.color_backgraund);
        painter.setPen(init_struct.color_line);
        painter.drawRect(rect_all);
    }
    else
        painter.drawImage(rect_all, image_back, image_back.rect());

    painter.setFont(init_struct.font);
    painter.setPen(QPen(init_struct.color_text));
    QString s = strings[ptr_strings];
    int center_x = 0;
    if(!state_timer)
        center_x += (9 - ptr_timer) * rect_all.width() / 9;
    else if(state_timer == 2)
        center_x += -ptr_timer * rect_all.width() / 9;
    rect_all.setX(center_x);
    painter.drawText(rect_all, Qt::AlignCenter, s);
    painter.end();
    update();
}
void CLabelChange::Start()
{
    ptr_timer = 0;
    state_timer = 0;
    timer_id = QObject::startTimer(50);
}
void CLabelChange::Stop()
{
    killTimer(timer_id);
}
void CLabelChange::timerEvent(QTimerEvent *event)
{
    if(state_timer == 1)
    {
        killTimer(timer_id);
        timer_id = QObject::startTimer(50);
        state_timer = 2;
        return;
    }
    if(ptr_timer == 10)
    {
        ptr_timer = 0;
        state_timer ++;
        if(state_timer == 1)
        {
            killTimer(timer_id);
            timer_id = QObject::startTimer(2000);
        }
        else if(state_timer == 3)
        {
            state_timer = 0;
            ptr_strings ++;
            if(ptr_strings >= strings.size())
                ptr_strings = 0;
        }
    }
    Paint();
    ptr_timer ++;
}
void CLabelChange::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(rect(), image, image.rect());
    painter.end();
}
void CLabelChange::resizeEvent(QResizeEvent *r)
{
    image = QImage(width(), height(), QImage::Format_RGB32);
    emit IAmReSize();
    Paint();
}
void CLabelChange::SetImageBack(QImage &ib, QRect r1, QRect r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    style_back = esImage;
    Paint();
}
