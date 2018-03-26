#include "CAllWindows.h"

CAllWindows::CAllWindows(QWidget *parent) : buttons(this),
    QWidget(parent), tek(0), new_tek(0), timer_id(0), mouse_press(false)
{
    box_layout = new QBoxLayout(QBoxLayout::LeftToRight);
    box_layout->setMargin(0);
    box_layout->setSpacing(0);

    QObject::connect(&buttons, SIGNAL(PressButton(unsigned, bool)), this, SLOT(OnButtonsClick(unsigned, bool)));
    box_layout->addWidget(&buttons);


    main_widget = new QWidget;
    main_widget->setAttribute(Qt::WA_PaintOnScreen);
    box_layout->addWidget(main_widget);
    QWidget::setLayout(box_layout);

    SetInitStruct(init_struct);
}
void CAllWindows::SetInitStruct(const SAllWindowsInit &set_struct)
{
    init_struct = set_struct;

    CButtonsCheck::SButtonsInit but_init;
    if(init_struct.direction_frame == SAllWindowsInit::edVertical)
    {
        but_init.direction = CButtonsCheck::SButtonsInit::edVertical;
        but_init.font = this->font();
        but_init.font.setPixelSize(15);
        but_init.color_backgraund = QColor(0, 30, 185, 255);
        but_init.color_backgraund_checked = QColor(100, 100, 255, 255);
        but_init.color_text = Qt::white;
        buttons.setMaximumWidth(init_struct.frame_width);
    }
    else
    {
        but_init.direction = CButtonsCheck::SButtonsInit::edHorizontal;
        but_init.font = this->font();
        but_init.font.setPixelSize(15);
        but_init.color_backgraund = QColor(0, 30, 185, 255);
        but_init.color_backgraund_checked = QColor(100, 100, 255, 255);
        but_init.color_text = Qt::white;
        buttons.setMaximumHeight(init_struct.frame_height);
    }
    buttons.SetInitStruct(but_init);
    if(init_struct.direction_frame == SAllWindowsInit::edVertical)
        box_layout->setDirection(QBoxLayout::LeftToRight);
    else
        box_layout->setDirection(QBoxLayout::TopToBottom);
}
void CAllWindows::AddWidget(QWidget *w, QString name, QPixmap &pix)
{
    AddWidget(w, name);
  //  buttons[buttons.size()-1]->setIcon(pix);
}
void CAllWindows::AddWidget(QWidget *w, QString name)
{
    w->setVisible(false);
    w->setParent(main_widget);
    list_windows << w;

    buttons.AddButton(CButtonsCheck::SButton(name));
    if(list_windows.size() == 1)
    {
        list_windows[0]->setVisible(true);
        buttons.SetChecked(0);
    }
}
void CAllWindows::RePlaceWidget(unsigned n, QWidget *w, QString name, QPixmap &pix)
{
    if(n >= list_windows.size())
        return;
    list_windows[n]->setVisible(false);
    list_windows[n] = w;
    list_windows[n]->setParent(main_widget);
    list_windows[n]->resize(main_widget->size());
    list_windows[n]->setVisible(true);
    list_windows[n]->move(0, 0);
    buttons.RenameButton(name, n);
}
void CAllWindows::paintEvent(QPaintEvent *p)
{
    QWidget::paintEvent(p);
}
void CAllWindows::resizeEvent(QResizeEvent *r)
{
    QWidget::resizeEvent(r);
    for(int i = 0; i < list_windows.count(); i ++)
        list_windows[i]->resize(main_widget->size());
}
void CAllWindows::OnButtonsClick(const unsigned n, bool s)
{
    if(n >= list_windows.size())
        return;
    if(timer_id)
    {
        buttons.SetChecked(new_tek);
        return;
    }
    new_tek = n;
    buttons.SetChecked(new_tek);
    if(tek!= new_tek)
    {
        timer_st = init_struct.rewindow_kol_step;
        timer_id = QObject::startTimer(init_struct.rewindow_time_all/init_struct.rewindow_kol_step);
    }
    emit NewWindow(new_tek);
}
void CAllWindows::timerEvent(QTimerEvent *event)
{
    timer_st --;
    int size = (init_struct.direction_widget == SAllWindowsInit::edHorizontal)? main_widget->width() : main_widget->height();
    const int old_window_visiable = (size * timer_st) / init_struct.rewindow_kol_step;
    int old_window_coordinate, new_window_coordinate;
    if(tek < new_tek)
    {
        old_window_coordinate = 0 - (size - old_window_visiable);
        new_window_coordinate = old_window_visiable;
    }
    else
    {
        old_window_coordinate = size - old_window_visiable;
        new_window_coordinate = 0 - old_window_visiable;
    }
    if(init_struct.direction_widget == SAllWindowsInit::edHorizontal)
    {
        list_windows[tek]->move(old_window_coordinate, 0);
        list_windows[new_tek]->move(new_window_coordinate, 0);
    }
    else
    {
        list_windows[tek]->move(0, old_window_coordinate);
        list_windows[new_tek]->move(0, new_window_coordinate);
    }
    list_windows[new_tek]->setVisible(true);

    if(!timer_st)
    {
        list_windows[tek]->setVisible(false);
        tek = new_tek;
        list_windows[tek]->setVisible(true);
        QObject::killTimer(timer_id);
        timer_id = 0;
     //   for(int i = 0; i < buttons.size(); i ++)
     //       buttons[i]->setChecked(false);
   //     buttons[tek]->setChecked(true);
    }
}
void CAllWindows::ChoiceStart()
{
}
void CAllWindows::ChoiceEnd()
{
}
void CAllWindows::mousePressEvent(QMouseEvent *pe)
{
    if((pe->x() > main_widget->x()) && (pe->y() > main_widget->y()))
    {
        mouse_press_x = pe->x();
        mouse_press_y = pe->y();
        mouse_press = true;
    }
}
void CAllWindows::mouseMoveEvent(QMouseEvent *pe)
{
    if(mouse_press)
    {
        int step_x = pe->x() - mouse_press_x;
        int step_y = pe->y() - mouse_press_y;
        if(init_struct.direction_widget == SAllWindowsInit::edVertical)
        {
            if(step_y > (main_widget->height() / 3))
            {
                if(tek)
                {
                    OnButtonsClick(tek-1);
                    mouse_press = false;
                }
            }
            if(-step_y > (main_widget->height() / 3))
            {
                if(tek < (list_windows.size()-1))
                {
                    OnButtonsClick(tek+1);
                    mouse_press = false;
                }
            }
        }
        else
        {
            if(step_x > (main_widget->width() / 3))
            {
                if(tek)
                {
                    OnButtonsClick(tek-1);
                    mouse_press = false;
                }
            }
            if(-step_x > (main_widget->width() / 3))
            {
                if(tek < (list_windows.size()-1))
                {
                    OnButtonsClick(tek+1);
                    mouse_press = false;
                }
            }
        }
    }
    else
    {
        if(pe->y() == 0)
            ChoiceStart();
    }
}
void CAllWindows::mouseReleaseEvent(QMouseEvent *pe)
{
    mouse_press = false;
}


