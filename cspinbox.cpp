#include "cspinbox.h"

CSpinBox::CSpinBox(QWidget *parent) : QWidget(parent), min(0), max(20),
    step_small(1), step_lage(10), is_my_focus(false)
{
    for(int i = 0; i < 6; i ++)
        buttons << SButton();
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
    setCursor(QCursor(Qt::PointingHandCursor));
    Paint();
}
void CSpinBox::SetValue(const int &v)
{
    new_value = v;
    if(OnNewValue())
        Paint();
}
bool CSpinBox::OnNewValue()
{
    if(new_value < min)
        new_value = min;
    if(new_value > max)
        new_value = max;
    if(value == new_value)
        return false;
    value = new_value;
    emit NewValue(value);
    return true;
}
void CSpinBox::Paint()
{
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int stap_gr00[4] = {150, 200, 230, 255};
    int stap_gr01[4] = {120, 170, 200, 225};
    int *stap_gr0 = (is_my_focus)? stap_gr01 : stap_gr00;
    int stap_gr[5] = {255, 100, 200, 255, 180};
    QRect rect_all(0, 0, width(), height());
    QPointF cen_gr_piint(width()/4, height()/3);
    QRadialGradient gradient(cen_gr_piint, width()/2, cen_gr_piint);
    QColor cg = init_struct.color_backgraund;
    for(int i = 0; i < 4; i ++)
    {
        cg.setAlpha(stap_gr0[i]);
        gradient.setColorAt((double)i / 4.0, cg);
    }
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(Qt::white);
    painter.drawRect(rect_all);
    painter.setBrush(gradient);
    painter.drawRect(rect_all);

    QColor cl = init_struct.color_text;
    if(!is_my_focus)
        cl.setAlpha(100);
    painter.setPen(QPen(cl));
    painter.setFont(init_struct.font);
    painter.drawText(rect_all, init_struct.align_flags, QString::number(value));

    if(is_my_focus)
    for(int i = 0; i < 6; i ++)//buttons
    {
        QRect rect_but_all(buttons[i].x1, buttons[i].y1, buttons[i].x2 - buttons[i].x1, buttons[i].y2 - buttons[i].y1);
        int sglaz = (buttons[i].y2 - buttons[i].y1) / 4;
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(init_struct.color_backgraund_buttons);
        painter.drawRect(rect_but_all);

        if(buttons[i].state != esFree)
        {
            QLinearGradient gradient((buttons[i].x1+buttons[i].x2)/2, buttons[i].y1, (buttons[i].x1+buttons[i].x2)/2, buttons[i].y2);
            QColor cg = (buttons[i].state == esChecked)? init_struct.color_backgraund_checked : init_struct.color_backgraund_buttons;
            for(int i = 0; i < 5; i ++)
            {
                cg.setAlpha(stap_gr[i]);
                gradient.setColorAt((double)i / 4.0, cg);
            }
            painter.setBrush(Qt::white);
            painter.drawRoundedRect(rect_but_all, sglaz, sglaz);
            painter.setPen(init_struct.color_line);
            painter.setBrush(gradient);
            painter.drawRoundedRect(rect_but_all, sglaz, sglaz);
        }
//        painter.setPen(QPen(init_struct.color_text));
  //      painter.drawText(rect_but_all, Qt::AlignCenter, buttons[i].name);
        QColor cll = init_struct.color_line;
        if(!is_my_focus)
            cll.setAlpha(100);
        painter.setPen(QPen(cll));
        int center_x = (buttons[i].x1+buttons[i].x2)/2;
        int center_y = (buttons[i].y1+buttons[i].y2)/2;
        switch(i)
        {
            case(0):
                painter.drawLine(QPointF(center_x - 6, center_y +4), QPointF(center_x, center_y-4));
                painter.drawLine(QPointF(center_x + 6, center_y +4), QPointF(center_x, center_y-4));
                break;
            case(1):
                painter.drawLine(QPointF(center_x - 6, center_y -4), QPointF(center_x, center_y+4));
                painter.drawLine(QPointF(center_x + 6, center_y -4), QPointF(center_x, center_y+4));
                break;
            case(2):
                painter.drawLine(QPointF(center_x - 6, center_y +1), QPointF(center_x, center_y-6));
                painter.drawLine(QPointF(center_x + 6, center_y +1), QPointF(center_x, center_y-6));
                painter.drawLine(QPointF(center_x - 6, center_y +6), QPointF(center_x, center_y-1));
                painter.drawLine(QPointF(center_x + 6, center_y +6), QPointF(center_x, center_y-1));
                break;
            case(3):
                painter.drawLine(QPointF(center_x - 6, center_y -6), QPointF(center_x, center_y+1));
                painter.drawLine(QPointF(center_x + 6, center_y -6), QPointF(center_x, center_y+1));
                painter.drawLine(QPointF(center_x - 6, center_y -1), QPointF(center_x, center_y+6));
                painter.drawLine(QPointF(center_x + 6, center_y -1), QPointF(center_x, center_y+6));
                break;
            case(4):
                painter.drawLine(QPointF(center_x - 7, center_y -6), QPointF(center_x + 7, center_y-6));
                painter.drawLine(QPointF(center_x, center_y -6), QPointF(center_x, center_y+6));
                painter.drawLine(QPointF(center_x - 3, center_y), QPointF(center_x, center_y-6));
                painter.drawLine(QPointF(center_x + 3, center_y), QPointF(center_x, center_y-6));
                break;
            case(5):
                painter.drawLine(QPointF(center_x - 7, center_y +6), QPointF(center_x + 7, center_y+6));
                painter.drawLine(QPointF(center_x, center_y -6), QPointF(center_x, center_y+6));
                painter.drawLine(QPointF(center_x - 3, center_y), QPointF(center_x, center_y+6));
                painter.drawLine(QPointF(center_x + 3, center_y), QPointF(center_x, center_y+6));
                break;
        }
    }
    painter.end();
    update();
}
void CSpinBox::timerEvent(QTimerEvent *event)
{
    killTimer(timer_id);
    int who = -1;
    for(int i = 0; i < 4; i ++)
        if(buttons[i].state == esChecked)
            who = i;
    if(who < 0)
        return;
    if(NewStateOnButtons(who))
    {
        timer_id = QObject::startTimer(100);
        Paint();
    }
}
bool CSpinBox::NewStateOnButtons(const unsigned &n)
{
    if(n >= 6)
        return false;
    int step[6] = {value+step_small, value-step_small, value+step_lage, value-step_lage, max, min};
    new_value = step[n];
    return OnNewValue();
}
void CSpinBox::mousePressEvent(QMouseEvent *pe)
{
    if(pe->button() != 1)
        return;
    bool change_state = false;
    for(int i = 0; i < 6; i ++)
    {
        eState s = buttons[i].state;
        if(buttons[i].IsMy(pe->x(), pe->y()))
        {
            buttons[i].state = esChecked;
            if(NewStateOnButtons(i))
            {
                change_state = true;
                if(i < 4)
                    timer_id = QObject::startTimer(500);
            }
        }
        else
            buttons[i].state = esFree;
        if(s != buttons[i].state)
            change_state = true;
    }
    if(change_state)
        Paint();
}
void CSpinBox::mouseMoveEvent(QMouseEvent *pe)
{
    bool change_state = false;
    for(int i = 0; i < 6; i ++)
    {
        eState s = buttons[i].state;
        if(buttons[i].IsMy(pe->x(), pe->y()))
        {
            if(s != esChecked)
                buttons[i].state = esFocus;
        }
        else
            buttons[i].state = esFree;

        //buttons[i].state = (buttons[i].IsMy(pe->x(), pe->y()))? esFocus : esFree;
        if(s != buttons[i].state)
            change_state = true;
    }
    if(change_state)
        Paint();
}
void CSpinBox::mouseReleaseEvent(QMouseEvent *pe)
{
    mouseMoveEvent(pe);
    for(int i = 0; i < 6; i ++)
        if(buttons[i].state == esChecked)
            buttons[i].state = esFocus;
}
void CSpinBox::wheelEvent(QWheelEvent *pe)
{
    SetValue(value + pe->delta() / 8 / 15);
}
void CSpinBox::enterEvent(QEvent *pe)
{
    setMouseTracking(true);
    is_my_focus = true;
    setFocus();
    Paint();
}
void CSpinBox::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
    is_my_focus = false;
    for(int i = 0; i < 6; i ++)
        buttons[i].state = esFree;
    setMouseTracking(false);
    Paint();
}
void CSpinBox::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CSpinBox::resizeEvent(QResizeEvent *r)
{
    image = QImage(width(), height(), QImage::Format_RGB32);
    for(int i = 0; i < 6; i ++)
    {
        buttons[i].y1 = (i%2)? height() - init_struct.height_buttons : 0;
        buttons[i].y2 = (i%2)? height() : init_struct.height_buttons;
        buttons[i].x1 = (i/2) * (width()/3);
        buttons[i].x2 = (i/2+1) * (width()/3);
    }
    Paint();
}
