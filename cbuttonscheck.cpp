#include "cbuttonscheck.h"

void CButtonsCheck::SetChecked(const unsigned &n)
{
    if(n >= buttons.size())
        return;
    for(int i = 0; i < buttons.size(); i ++)
        buttons[i].state = (i == n)? esChecked : esFree;
    Paint();
}
void CButtonsCheck::SetNotChecked()
{
    for(int i = 0; i < buttons.size(); i ++)
        buttons[i].state = esFree;
    Paint();
}
void CButtonsCheck::mousePressEvent(QMouseEvent *pe)
{
    if(pe->button() != 1)
        return;
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    int ch = -1;
    for(int i = 0; i < buttons.size(); i ++)
    {
        eState s = buttons[i].state;
        if(buttons[i].IsMy(x, y))
        {
            if(buttons[i].state == esChecked)
                continue;
            buttons[i].state = esChecked;
            ch = i;
        }
        else
            buttons[i].state = esFree;
        if(s != buttons[i].state)
            change_state = true;
    }
    if(ch >= 0)
        emit PressButton(ch, true);
    if(change_state)
        Paint();
}
void CButtonsCheck::mouseMoveEvent(QMouseEvent *pe)
{
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    bool checks = false;
    for(int i = 0; i < buttons.size(); i ++)
    {
        eState s = buttons[i].state;
        if(buttons[i].state == esChecked)
        {
            if(buttons[i].IsMy(x, y))
                checks = true;
            continue;
        }
        buttons[i].state = (buttons[i].IsMy(x, y))? esFocus : esFree;
        if(s != buttons[i].state)
            change_state = true;
    }
    setCursor(QCursor((checks)? Qt::ArrowCursor : Qt::PointingHandCursor));
    if(change_state)
        Paint();
}
void CButtonsCheck::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
    for(int i = 0; i < buttons.size(); i ++)
        if(buttons[i].state == esFocus)
            buttons[i].state = esFree;
    setMouseTracking(false);
    focus = false;
    Paint();
}
