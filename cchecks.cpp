#include "cchecks.h"

CChecks::CChecks(QWidget *parent) : QWidget(parent), focus(false)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}
void CChecks::SetInitStruct(const SButtonsInit &set_struct)
{
    setMouseTracking(true);
    init_struct = set_struct;
    ReSize();
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
}
void CChecks::ReSize()
{
    if(!buttons.size())
    {
        Paint();
        return;
    }
    int width_pic, hieght_pic;
    if(init_struct.size_horizontal == SButtonsInit::esFixedSize)
        width_pic = init_struct.width_button;
    else if(init_struct.direction == SButtonsInit::edVertical)
            width_pic = width();
        else
            width_pic = width() / buttons.size();
    if(init_struct.size_vertical == SButtonsInit::esFixedSize)
        hieght_pic = init_struct.hieght_button;
    else if(init_struct.direction == SButtonsInit::edVertical)
            hieght_pic = height() / buttons.size();
        else
            hieght_pic = height();
    for(int i = 0; i < buttons.size(); i ++)
        if(init_struct.direction == SButtonsInit::edVertical)
        {
            buttons[i].x1 = 0;
            buttons[i].x2 = width_pic;
            buttons[i].y1 = i * hieght_pic;
            buttons[i].y2 = (i + 1) * hieght_pic;
        }
        else
        {
            buttons[i].x1 = i * width_pic;
            buttons[i].x2 = (i + 1) * width_pic;
            buttons[i].y1 = 0;
            buttons[i].y2 = hieght_pic;
        }
    Paint();
}
void CChecks::Paint()
{
    if(image.width() != width() || image.height() != height())
        image = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(init_struct.font);

    int stap_gr[5] = {255, 100, 200, 255, 180};
    int stap_focus[5] = {120, 50, 120, 150, 120};

    if(init_struct.style_background == SButtonsInit::ebColor)
    {
        if(style_back == esColor)
        {
            painter.setBrush(init_struct.color_backgraund);
            painter.drawRect(image.rect());
        }
        else
            painter.drawImage(image.rect(), image_back, image.rect());
    }
    for(int i = 0; i < buttons.size(); i ++)
    {
        QRect rect_all(buttons[i].x1, buttons[i].y1, buttons[i].x2 - buttons[i].x1, buttons[i].y2 - buttons[i].y1);
        QRect rect_no_check(buttons[i].x1, buttons[i].y1, buttons[i].x2 - buttons[i].x1-2, buttons[i].y2 - buttons[i].y1-2);
        QRect rect_check(buttons[i].x1+2, buttons[i].y1+2, buttons[i].x2 - buttons[i].x1-2, buttons[i].y2 - buttons[i].y1-2);
        int sglaz = (buttons[i].y2 - buttons[i].y1) / 5;
        QColor c1 = (buttons[i].state2 == esChecked)? init_struct.color_backgraund2 : init_struct.color_backgraund;
        QColor c2 = (buttons[i].state2 == esChecked)? init_struct.color_backgraund_checked2 : init_struct.color_backgraund_checked;
        if(init_struct.style_background == SButtonsInit::ebColor)
        {
            painter.setPen(QPen(Qt::NoPen));
        /*    if(style_back == esColor)
            {
                painter.setBrush(init_struct.color_backgraund);
                painter.drawRect(rect_all);
            }
            else
                painter.drawImage(rect_all, image_back, rect_all);*/
            if(buttons[i].state == esCheckedFocus || buttons[i].state == esFocus)
            {
                QLinearGradient gradient((buttons[i].x1+buttons[i].x2)/2, buttons[i].y1, (buttons[i].x1+buttons[i].x2)/2, buttons[i].y2);
//                QColor cg = (buttons[i].state == esCheckedFocus)? c2 : c1;
                QColor cg = (buttons[i].state == esCheckedFocus)? init_struct.color_backgraund_checked : init_struct.color_backgraund;
                for(int i = 0; i < 5; i ++)
                {
                    cg.setAlpha(stap_gr[i]);
                    gradient.setColorAt((double)i / 4.0, cg);
                }
                painter.setBrush(Qt::white);
                painter.drawRoundedRect((buttons[i].state == esFocus)? rect_no_check : rect_check, sglaz, sglaz);
                painter.setPen(init_struct.color_line);
                painter.setBrush(gradient);
                painter.drawRoundedRect((buttons[i].state == esFocus)? rect_no_check : rect_check, sglaz, sglaz);
            }
            else if(focus)
            {
                QLinearGradient gradient((buttons[i].x1+buttons[i].x2)/2, buttons[i].y1, (buttons[i].x1+buttons[i].x2)/2, buttons[i].y2);
  //              QColor cg = (buttons[i].state == esChecked)? c2 : c1;
                QColor cg = (buttons[i].state == esChecked)? init_struct.color_backgraund_checked : init_struct.color_backgraund;
                for(int i = 0; i < 5; i ++)
                {
                    cg.setAlpha(stap_focus[i]);
                    gradient.setColorAt((double)i / 4.0, cg);
                }
                painter.setPen(Qt::NoPen);
                painter.setBrush(gradient);
                painter.drawRoundedRect((buttons[i].state == esFree)? rect_no_check : rect_check, sglaz, sglaz);
            }
            else if(buttons[i].state == esChecked)
            {
//                painter.setBrush(c2);
                painter.setBrush(init_struct.color_backgraund_checked);
                painter.drawRect(rect_check);
            }
            else if(buttons[i].state2 == esChecked)
            {
          //      painter.setBrush(c1);
          //      painter.drawRect(rect_check);
            }
        }
        else// SButtonsInit::ebPicture
        {
            if(buttons[i].state == esFree || buttons[i].state == esFocus)
                painter.drawImage(rect_all, buttons[i].image, buttons[i].image.rect());
            else
                painter.drawImage(rect_all, buttons[i].image_check, buttons[i].image_check.rect());
            QColor cg2 = init_struct.color_line;
            painter.setPen(QPen(cg2, 1));
            painter.setBrush(QBrush(Qt::NoBrush));
            if(buttons[i].state == esFocus || buttons[i].state == esCheckedFocus)
                painter.drawRoundedRect(rect_all, 1, 1);
            else
            {
                int stap_picture[5] = {60, 20, 60, 70, 60};
                QLinearGradient gradient((buttons[i].x1+buttons[i].x2)/2, buttons[i].y1, (buttons[i].x1+buttons[i].x2)/2, buttons[i].y2);
                QColor cg = init_struct.color_backgraund;
                for(int i = 0; i < 5; i ++)
                {
                    cg.setAlpha(stap_picture[i]);
                    gradient.setColorAt((double)i / 4.0, cg);
                }
                painter.setPen(Qt::NoPen);
                painter.setBrush(gradient);
                painter.drawRoundedRect(rect_all, 1, 1);
            }
        }
        QFont qf = painter.font();
        QColor col = init_struct.color_text;
    //    QColor col = (buttons[i].state2 == esChecked)? init_struct.color_text_checked : init_struct.color_text;
        if((buttons[i].state2 == esChecked))
        {
            qf.setBold(true);
            col = init_struct.color_text_checked;
        }
        else if(buttons[i].state == esFree || buttons[i].state == esChecked)
        {
            col.setAlpha(150);
            qf.setBold(false);
        }
        else
            qf.setBold(true);
        painter.setFont(qf);
        painter.setPen(QPen(col));
        painter.drawText((buttons[i].state == esChecked || buttons[i].state == esCheckedFocus)? rect_check : rect_no_check, init_struct.align_flags, buttons[i].name);
    }
    painter.end();
    update();
}
void CChecks::resizeEvent(QResizeEvent *r)
{
  //  QWidget::resizeEvent(r);
    emit IAmReSize();
    ReSize();
}
void CChecks::paintEvent(QPaintEvent *p)
{
 //   QWidget::paintEvent(p);
 //   if(!QWidget::isVisible())
  //      return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CChecks::AddButton(SButton sb, const int n)
{
    sb.state = esFree;
    if(n >= 0)
        buttons.insert(n, sb);
    else
        buttons << sb;
    ReSize();
}
void CChecks::AddButton(SButton sb, QImage &i, QImage &ic, const int n)
{
    sb.state = esFree;
    sb.state2 = esFree;
    int k = n;
    if(n < 0)
    {
        buttons << sb;
        k = buttons.size()-1;
    }
    else
        buttons.insert(n, sb);
    buttons[k].image = i;
    buttons[k].image_check = ic;
    ReSize();
}
void CChecks::RenameButton(const QString str, const unsigned &n)
{
    if(n < buttons.size())
        buttons[n].name = str;
    Paint();
}
void CChecks::SubButton(const int n)
{
    if(n < 0)
        buttons.clear();
    else
        buttons.removeAt(n);
    ReSize();
}
void CChecks::SetChecked(const unsigned &n, const bool &state)
{
    if(n < buttons.size())
        buttons[n].state = (state)? esChecked : esFree;
}
void CChecks::SetChecked2(const unsigned &n, const bool &state)
{
    if(n < buttons.size())
        buttons[n].state2 = (state)? esChecked : esFree;
}
void CChecks::mousePressEvent(QMouseEvent *pe)
{
    if(pe->button() != 1)
        return;
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    for(int i = 0; i < buttons.size(); i ++)
        if(buttons[i].IsMy(x, y))
        {
            if(buttons[i].state == esCheckedFocus)
                buttons[i].state = esFocus;
            else
                buttons[i].state = esCheckedFocus;
            emit PressButton(i, buttons[i].state == esCheckedFocus);
            change_state = true;
        }
    if(change_state)
        Paint();
}
void CChecks::mouseMoveEvent(QMouseEvent *pe)
{
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    for(int i = 0; i < buttons.size(); i ++)
    {
        eState s = buttons[i].state;
        switch(buttons[i].state)
        {
            case esChecked: case esCheckedFocus:
                buttons[i].state = buttons[i].IsMy(x, y)? esCheckedFocus : esChecked; break;
            case esFocus: case esFree:
                buttons[i].state = buttons[i].IsMy(x, y)? esFocus : esFree; break;
        }
        if(s != buttons[i].state)
            change_state = true;
    }
    if(change_state)
        Paint();
}
void CChecks::mouseReleaseEvent(QMouseEvent *pe)
{
    mouseMoveEvent(pe);
}
void CChecks::enterEvent(QEvent *)
{
    setMouseTracking(true);
    focus = true;
}
void CChecks::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
    for(int i = 0; i < buttons.size(); i ++)
    {
        if(buttons[i].state == esFocus)
            buttons[i].state = esFree;
        if(buttons[i].state == esCheckedFocus)
            buttons[i].state = esChecked;
    }
    setMouseTracking(false);
    focus = false;
    Paint();
}
void CChecks::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    style_back = esImage;
    ReSize();
}
