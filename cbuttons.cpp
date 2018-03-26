#include "cbuttons.h"

void CButtons::mousePressEvent(QMouseEvent *pe)
{
    if(pe->button() != 1)
        return;
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    for(int i = 0; i < buttons.size(); i ++)
    {
        eState s = buttons[i].state;
        buttons[i].state = (buttons[i].IsMy(x, y))? esChecked : esFree;
        if(s != buttons[i].state)
            change_state = true;
        if(buttons[i].IsMy(x, y))
            emit PressButton(i, true);
    }
    if(change_state)
        Paint();
}
void CButtons::mouseMoveEvent(QMouseEvent *pe)
{
    int x = pe->x();
    int y = pe->y();
    bool change_state = false;
    for(int i = 0; i < buttons.size(); i ++)
    {
        eState s = buttons[i].state;
        buttons[i].state = (buttons[i].IsMy(x, y))? esFocus : esFree;
        if(s != buttons[i].state)
            change_state = true;
    }
    if(change_state)
        Paint();
}
void CButtons::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
    for(int i = 0; i < buttons.size(); i ++)
        buttons[i].state = esFree;
    setMouseTracking(false);
    focus = false;
    Paint();
}
