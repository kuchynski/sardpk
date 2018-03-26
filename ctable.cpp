#include "ctable.h"

CTable::CTable(QWidget *parent) : QWidget(parent), first_line(0), end_line(0), kol_line_screen(0),
    visiable_title(false), visiable_buttons(false), style_back(esColor), number_checked_line(0)
{
    QWidget::setFocusPolicy(Qt::StrongFocus);//Qt::ClickFocus);
    for(int i = 0; i < 8; i ++)
    {
        SButton b;
        b.state = esFree;
        buttons << b;
    }
    title.x1 = 0;
    title.y1 = 0;
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
    Clear();
}
void CTable::Clear(const bool paint)
{
    lines.clear();
    first_line = 0;
    if(paint)
        ReSize();
}
void CTable::SetTitle(const QList<QString> &sl, const bool paint)
{
    title.field = sl;
    visiable_title = true;
    if(paint)
        ReSize();
}
void CTable::SetLine(const QList<QString> &sl, const unsigned &n, const bool paint)
{
    if(n >= lines.size())
        return;
    lines[n].field = sl;
    if(paint)
        ReSize();
}
void CTable::AddLine(const QList<QString> &sl, const bool paint)
{
    SLine l;
    l.field = sl;
    lines << l;
    lines[lines.size()- 1].state = esFree;
    if(paint)
        ReSize();
}
void CTable::SubLine(const unsigned &n, const bool paint)
{
    if(n >= lines.size())
        return;
    lines.removeAt(n);
    if(paint)
        ReSize();
}
void CTable::SetChecked(const unsigned &n)
{
    if(n >= lines.size() || !lines.size())
        return;
   // for(int i = 0; i < lines.size(); i ++)
     //   if(lines[i].state == esChecked)
       //     lines[i].state = esFree;
    if(number_checked_line < lines.size())
        lines[number_checked_line].state = esFree;
    lines[n].state = esChecked;
    if(number_checked_line != n)
        ClickLine(n);
    number_checked_line = n;
    if(number_checked_line < first_line)
        first_line = number_checked_line;
    if(number_checked_line >= end_line)
        first_line += (number_checked_line - end_line + 1);
    ReSize();
}
void CTable::SetInitStruct(const STableInit &set_struct)
{
    init_struct = set_struct;
}
void CTable::ReSize()
{
    unsigned width_pic = width();
    unsigned hieght_pic = height();
    unsigned hieght_line = init_struct.hieght_line;
    kol_line_screen = hieght_pic / hieght_line;
    if(first_line >= lines.size())
        first_line = lines.size() -1;
    if(first_line < 0)
        first_line = 0;
    if(visiable_title)
        kol_line_screen --;
    if(!lines.size() || kol_line_screen < 1)
    {
        first_line = 0;
        end_line = 0;
        visiable_buttons = false;
    }
    else if(kol_line_screen >= lines.size())
    {
         visiable_buttons = false;
         first_line = 0;
         end_line = lines.size();
    }
    else
    {
        visiable_buttons = true;
        kol_line_screen -= 2;
        if(kol_line_screen < 1)
        {
            end_line = first_line;
        }
        else
        {
            end_line = kol_line_screen + first_line;
            if(end_line > lines.size())
                end_line = lines.size();
            buttons[6].name = (first_line)? QString::number(first_line) : "";
            buttons[7].name = (lines.size() - end_line)? QString::number(lines.size() - end_line) : 0;
        }

    }
    title.x2 = width_pic;
    title.y2 = hieght_line;
    const int tmp_sm1 = (visiable_title)? hieght_line : 0;
    const int tmp_sm2 = (visiable_buttons)? hieght_line : 0;
    for(int i = first_line; i < end_line; i ++)
    {
        lines[i].x1 = 0;
        lines[i].x2 = width_pic;
        lines[i].y1 = (i-first_line) * hieght_line + tmp_sm1 + tmp_sm2;
        lines[i].y2 = (i-first_line + 1) * hieght_line + tmp_sm1 + tmp_sm2;
        lines[i].repic = true;
    }
//    int size_button = width_pic / 4;
    int size_button = 40;
    int title_y = (visiable_title)? hieght_line : 0;
    title.repic = true;
    for(int i = 0; i < 8; i++)
    {
//        buttons[i].x1 = (i/2) * size_button;
  //      buttons[i].x2 = (i/2+1) * size_button;
        buttons[i].x1 = width_pic - size_button*4 + (i/2) * size_button;
        buttons[i].x2 = width_pic - size_button*4 + (i/2+1) * size_button;
        buttons[i].y1 = (i%2)? hieght_pic - hieght_line : title_y;
        buttons[i].y2 = (i%2)? hieght_pic : title_y + hieght_line;
    }
    RePaint(true);
}
int CTable::PaintLine(SLine &line, QPainter &painter, const bool tit)
{
    if(!line.repic)
        return line.y2;
    int stap_gr0[5] = {20, 0, 0, 0, 30};
    int stap_gr1[5] = {255, 100, 200, 255, 180};
    int *stap_gr = stap_gr0;
    int sglaz = height() / 150;

    QColor color_backgraund = (tit)? init_struct.color_backgraund_title : init_struct.color_backgraund;
    QColor color_text = (tit/* && hasFocus()*/)? init_struct.color_text_title : init_struct.color_text;

   // QColor cg2 = (line.state == esChecked)? init_struct.color_line_checked : init_struct.color_line;
    painter.setPen(QPen(color_backgraund, 1));
    line.repic = false;
    QRect rect1(line.x1, line.y1, line.x2 - line.x1, line.y2 - line.y1);
    if(style_back == esColor || line.state == esFocus || line.state == esChecked || tit)
    {
        painter.setBrush(Qt::gray);
        painter.drawRect(rect1);
        stap_gr = stap_gr1;
    }
    else
        painter.drawImage(rect1, image_back, rect1);
    QRect rect2(line.x1+2, line.y1+2, line.x2 - line.x1 - 4, line.y2 - line.y1 - 4);
    QLinearGradient gradient((line.x1+line.x2)/2, line.y1, (line.x1+line.x2)/2, line.y2+1);
    QColor cg = (line.state == esChecked)? init_struct.color_backgraund_checked : color_backgraund;
    for(int i = 0; i < 5; i ++)
    {
        cg.setAlpha(stap_gr[i]);
        gradient.setColorAt((double)i / 4.0, cg);
    }
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    QColor cg2 = (line.state == esChecked)? init_struct.color_line_checked : init_struct.color_line;
    painter.setPen(QPen(cg2, 1));
    if(line.state == esFocus || line.state == esChecked)
    {
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawRoundedRect(rect2, sglaz, sglaz);
    }
    //painter.setPen(QPen(cg2));
    int x = 0;
    for(int i = 0; i < init_struct.width_line.size()-1; i++)
    {
        x += init_struct.width_line[i];
        QColor cg22 = (line.state == esChecked)? init_struct.color_line_checked : init_struct.color_line;
        painter.setPen(QPen(cg22, 1));
        if(line.state == esFree)
        {
            painter.setPen(QPen(init_struct.color_backgraund, 1));
            painter.drawLine(QPointF(x, line.y1), QPointF(x, line.y2));
        }
        else
            painter.drawLine(QPointF(x, line.y1+2), QPointF(x, line.y2-4));
    }
    x = 0;
    QColor cg3 = (line.state == esChecked)? init_struct.color_text_checked : color_text;
    painter.setPen(QPen(cg3));
//    if(tit)
  //  {
    //    QFont qf = painter.font();
      //  qf.setBold(true);
        //painter.setFont(qf);
    //}
    for(int i = 0; i < init_struct.width_line.size(); i++)
    {
        if(i >= line.field.size())
            break;
        QRect rect(x+5, line.y1, init_struct.width_line[i]-10, line.y2 - line.y1);
        painter.drawText(rect, init_struct.align_flags, line.field[i]);
        x += init_struct.width_line[i];
    }
    return line.y2;
}
void CTable::RePaint(const bool all)
{
  //  if(!QWidget::isVisible())
    //    return;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int sglaz = height() / 150;
    painter.setFont(init_struct.font);
    int end_y = 0;
    if(visiable_title && lines.size())
        end_y = PaintLine(title, painter, true);
    for(int i = first_line; i < end_line; i ++)
        if(i >= 0 && i < lines.size())
            end_y = PaintLine(lines[i], painter);
    if(all)
    {
        QRect rect1(0, end_y, width(), height() - end_y);
        QRect rect2(0, buttons[0].y1, width(), buttons[0].y2 - buttons[0].y1);
        if(style_back == esColor)
        {
            int stap_gr[4] = {255, 110, 255, 115};
            QLinearGradient gradient(0, 0, width(), height());
            painter.setBrush(Qt::gray);
            painter.drawRect(rect1);
            if(visiable_buttons)
                painter.drawRect(rect2);

            QColor cg = init_struct.color_backgraund;
            cg.setAlpha(stap_gr[0]);
            gradient.setColorAt(0, cg);
            cg.setAlpha(stap_gr[1]);
            gradient.setColorAt(0.4, cg);
            cg.setAlpha(stap_gr[2]);
            gradient.setColorAt(0.8, cg);
            cg.setAlpha(stap_gr[3]);
            gradient.setColorAt(1, cg);
            painter.setPen(QPen(cg));
            painter.setBrush(gradient);
            painter.drawRect(rect1);
            if(visiable_buttons)
                painter.drawRect(rect2);
        }
        else
        {
            painter.drawImage(rect1, image_back, rect1);
            if(visiable_buttons)
                painter.drawImage(rect2, image_back, rect2);
        }
    }
    if(visiable_buttons)
    {
        int stap_gr[5] = {255, 100, 200, 255, 180};
        for(int i = 0; i < 8; i ++)//buttons
        {
            QRect rect_but_all(buttons[i].x1, buttons[i].y1, buttons[i].x2 - buttons[i].x1, buttons[i].y2 - buttons[i].y1);
            QRect rect_but_all1(buttons[i].x1, buttons[i].y1, buttons[i].x2 - buttons[i].x1-2, buttons[i].y2 - buttons[i].y1-2);
            QRect rect_but_all2(buttons[i].x1+1, buttons[i].y1+1, buttons[i].x2 - buttons[i].x1-2, buttons[i].y2 - buttons[i].y1-2);
            int sglaz = (buttons[i].y2 - buttons[i].y1) / 4;
            painter.setPen(QPen(Qt::NoPen));
            painter.setBrush(init_struct.color_backgraund);
            painter.drawRect(rect_but_all);

            if(buttons[i].state != esFree)
            {
                QLinearGradient gradient((buttons[i].x1+buttons[i].x2)/2, buttons[i].y1, (buttons[i].x1+buttons[i].x2)/2, buttons[i].y2);
                QColor cg = (buttons[i].state == esChecked)? init_struct.color_backgraund_checked : init_struct.color_backgraund_button;
                for(int i = 0; i < 5; i ++)
                {
                    cg.setAlpha(stap_gr[i]);
                    gradient.setColorAt((double)i / 4.0, cg);
                }
                painter.setBrush(Qt::white);
                painter.drawRoundedRect((buttons[i].state == esFocus)? rect_but_all1 : rect_but_all2, sglaz, sglaz);
                painter.setPen(init_struct.color_line);
                painter.setBrush(gradient);
                painter.setRenderHint(QPainter::Antialiasing, false);
                painter.drawRoundedRect((buttons[i].state == esFocus)? rect_but_all1 : rect_but_all2, sglaz, sglaz);
            }
            painter.setPen(QPen(init_struct.color_text));
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawText(rect_but_all, Qt::AlignCenter, buttons[i].name);
            QColor cll = init_struct.color_line;
            //if(!is_my_focus)
              //  cll.setAlpha(100);
            painter.setPen(QPen(cll));
            int sm = (buttons[i].state == esFocus)? 0 : 1;
            int center_x = (buttons[i].x1+buttons[i].x2)/2+sm;
            int center_y = (buttons[i].y1+buttons[i].y2)/2+sm;
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
    }
    painter.end();
    update();
}
bool CTable::PlusMinus(int n)
{
    int tmp = first_line;
    if(n == -50000)
        first_line = 0;
    else if(n == 50000)
        first_line = lines.size() - 1;
    else
        first_line += n;
    if(first_line > (lines.size() - kol_line_screen))
        first_line = lines.size() - kol_line_screen;
    if(first_line < 0)
        first_line = 0;
    return (tmp != first_line);
}
bool CTable::NewStateOnButtons(const unsigned &n)
{
    if(n >= 6)
        return false;
    int step[6] = {-1, 1, -kol_line_screen, kol_line_screen, -50000, 50000};
    return PlusMinus(step[n]);
}
void CTable::timerEvent(QTimerEvent *event)
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
        ReSize();
    }
}
void CTable::mousePressEvent(QMouseEvent *pe)
{
    //QWidget::mousePressEvent(pe);
  //  releaseKeyboard();
    //grabKeyboard();
    setFocus();//Qt::MouseFocusReason);
    if(pe->button() != 1)
        return;
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    if(visiable_buttons)
    {
        for(int i = 0; i < 6; i ++)
        {
            eState s = buttons[i].state;
            if(buttons[i].IsMy(x, y))
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
        {
            ReSize();
            return;
        }
        else
        {
            if((y < buttons[0].y2 && y >= buttons[0].y1) || (y < buttons[2].y2 && y >= buttons[2].y1))
                return;
        }
    }
    if(title.IsMy(x, y))
        return;
    int new_checked = -1;
    for(int i = first_line; i < end_line; i ++)
//    for(int i = 0; i < lines.size(); i ++)
        if(lines[i].IsMy(x, y))
        {
//            if(lines[i].state != esChecked)
            {
                lines[i].state = esChecked;
                number_checked_line = i;
                lines[i].repic = true;
                new_checked = i;
                change_state = true;
                emit ClickLine(i);
            }
            break;
        }
    if(new_checked != -1)
        for(int i = 0; i < lines.size(); i ++)
        {
            if((lines[i].state == esChecked) && (i != new_checked))
            {
                lines[i].state = esFree;
                lines[i].repic = true;
            }
        }
    if(change_state)
        RePaint(false);
}
void CTable::mouseDoubleClickEvent(QMouseEvent *pe)
{
    QWidget::mouseDoubleClickEvent(pe);
}
void CTable::MouseMove(const int x, const int y, const bool repic)
{
    bool change_state = false;
    bool my = false;
    //for(int i = 0; i < lines.size(); i ++)
    for(int i = first_line; i < end_line; i ++)
    {
        if(lines[i].IsMy(x, y))
            my = true;
        if(lines[i].state == esChecked)
            continue;
        eState s = lines[i].state;
        lines[i].state = (lines[i].IsMy(x, y))? esFocus : esFree;
        if(s != lines[i].state)
        {
            change_state = true;
            lines[i].repic = true;
        }
    }
    if(visiable_buttons)
        for(int i = 0; i < 6; i ++)
        {
            if(buttons[i].IsMy(x, y))
                my = true;
            eState s = buttons[i].state;
            if(buttons[i].IsMy(x, y))
            {
                if(s != esChecked)
                    buttons[i].state = esFocus;
            }
            else
                buttons[i].state = esFree;
//            buttons[i].state = (buttons[i].IsMy(x, y))? esFocus : esFree;
            if(s != buttons[i].state)
                change_state = true;
        }

    setCursor(QCursor((my)? Qt::PointingHandCursor : Qt::ArrowCursor));
    if(change_state && repic)
        RePaint(false);
}
void CTable::mouseMoveEvent(QMouseEvent *pe)
{
    QWidget::mouseMoveEvent(pe);
    MouseMove(pe->x(), pe->y());
}
void CTable::mouseReleaseEvent(QMouseEvent *pe)
{
 //   QWidget::mousePressEvent(pe);
   // mousePressEvent(pe);
    //mouseMoveEvent(pe);

    if(timer_id)
        killTimer(timer_id);
    for(int i = 0; i < 6; i ++)
        if(buttons[i].state == esChecked)
            buttons[i].state = esFocus;
    RePaint(false);
}
void CTable::wheelEvent(QWheelEvent *pe)
{
    QWidget::wheelEvent(pe);
    int numDegrees = pe->delta() / 8;
    int numSteps = numDegrees / 15;
    if(PlusMinus(-numSteps))
    {
        ReSize();
        MouseMove(pe->x(), pe->y());
    }
}
void CTable::enterEvent(QEvent *pe)
{
    QWidget::enterEvent(pe);
    setMouseTracking(true);
    setFocus();
}
void CTable::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
    for(int i = 0; i < lines.size(); i ++)
        if(lines[i].state == esFocus)
        {
            lines[i].state = esFree;
            lines[i].repic = true;
        }
    for(int i = 0; i < 8; i ++)
        buttons[i].state = esFree;
    setMouseTracking(false);
    RePaint(false);
}
void CTable::paintEvent(QPaintEvent *p)
{
 //   QWidget::paintEvent(p);
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CTable::resizeEvent(QResizeEvent *r)
{
//    QWidget::resizeEvent(r);
 //   if(!QWidget::isVisible())
   //     return;
    image = QImage(width(), height(), QImage::Format_RGB32);
    emit IAmReSize();
    ReSize();
}
void CTable::Paint()
{
    ReSize();
}
void CTable::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    style_back = esImage;
    ReSize();
}
void CTable::focusInEvent(QFocusEvent *pe)
{
//    QWidget::focusInEvent(pe);
    //RePaint(false);
}
void CTable::focusOutEvent(QFocusEvent *pe)
{
//    releaseKeyboard();
  //  RePaint(false);
}
void CTable::keyPressEvent(QKeyEvent *pe)
{
 //   QWidget::keyPressEvent(pe);
    //first_line, end_line, kol_line_screen, number_checked_line
    int new_number_checked_line = number_checked_line;
    switch(pe->key())
    {
        case Qt::Key_Up: new_number_checked_line--; break;
        case Qt::Key_Down: new_number_checked_line++; break;
        case Qt::Key_PageUp: new_number_checked_line -= kol_line_screen; break;
        case Qt::Key_PageDown: new_number_checked_line += kol_line_screen; break;
        case Qt::Key_Home: new_number_checked_line = 0; break;
        case Qt::Key_End: new_number_checked_line = lines.size()-1; break;
    }
    if(new_number_checked_line < 0)
        new_number_checked_line = 0;
    if(new_number_checked_line >= lines.size())
        new_number_checked_line = lines.size() - 1;
    if(new_number_checked_line != number_checked_line)
    {
        SetChecked(new_number_checked_line);
    }
}
