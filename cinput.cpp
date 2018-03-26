#include "cinput.h"

CInput::CInput(QWidget *parent): QWidget(parent), com_port(0), monitor(this),
    check_dat1(this), check_dat2(this), visiable(true), check_port(this),
    now_exit(false), dir_path(QDir::current().absolutePath())
{
    monitor.setGeometry(QRect(150, 200, 600, 300));
    QObject::connect(&monitor, SIGNAL(ClickGraph()), this, SLOT(on_center_monitor_clicked()));
    QObject::connect(&monitor, SIGNAL(WheelX(int)), this, SLOT(on_dial_x_monitor_valueChanged(int)));
    QObject::connect(&monitor, SIGNAL(WheelY(int)), this, SLOT(on_dial_y_monitor_valueChanged(int)));

    QPalette palette8;
    QBrush brush8(QColor(0, 30, 255, 255));
    palette8.setBrush(QPalette::Active, QPalette::Button, brush8);
    palette8.setBrush(QPalette::Inactive, QPalette::Button, brush8);
    palette8.setBrush(QPalette::Disabled, QPalette::Button, brush8);
    QPalette palette7;
    QBrush brush7(QColor(245, 245, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);


    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_text = Qt::white;
    check_dat1.setGeometry(QRect(150, 510, 600, 50));
    check_dat1.SetInitStruct(chek_init);
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ1")));
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ2")));
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ3")));
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ4")));
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ5")));
    check_dat1.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ6")));
    QObject::connect(&check_dat1, SIGNAL(PressButton(unsigned,bool)), this, SLOT(on_checked_dat1Changed(unsigned,bool)));
    QObject::connect(&check_dat1, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckDat1()));
    check_dat2.setGeometry(QRect(50, 610, 800, 50));
    check_dat2.SetInitStruct(chek_init);
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ДМ1")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ7")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ8")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ9")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ10")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ11")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ПБ12")));
    check_dat2.AddButton(CChecks::SButton(QString::fromLocal8Bit("ДМ2")));
    QObject::connect(&check_dat2, SIGNAL(PressButton(unsigned,bool)), this, SLOT(on_checked_dat2Changed(unsigned,bool)));
    QObject::connect(&check_dat2, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckDat2()));
    unsigned ch = param.GetMonitorValue(), ch2;
    if(ch < 6)
    {
        check_dat1.SetChecked(ch);
        on_checked_dat1Changed(ch, true);
    }
    else
    {
        switch(ch)
        {
            case 12: ch2 = 0; break;
            case 13: ch2 = 7; break;
            default: ch2 = ch - 5; break;
        }
        check_dat2.SetChecked(ch2);
        on_checked_dat2Changed(ch2, true);
    }


    label_com_state = new QLabel(this);
    label_com_state->setGeometry(QRect(50, 50, 300, 51));
    QFont font4;
   // font4.setFamily(QString::fromUtf8("Courier"));
    font4.setPointSize(30);
    label_com_state->setFont(font4);
   // label_com_state->setFrameShape(QFrame::Box);
   // label_com_state->setFrameShadow(QFrame::Raised);
  //  label_com_state->setLineWidth(3);
 //   label_com_state->setAlignment(Qt::AlignCenter);
    label_com_state->setPalette(palette7);

    on_checked_portChanged(param.GetComNumber(), true);
    check_port.setGeometry(QRect(50, 120, 900, 50));
    check_port.SetInitStruct(chek_init);
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("выкл")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM1")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM2")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM3")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM4")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM5")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM6")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM7")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM8")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("COM9")));
    check_port.AddButton(CChecks::SButton(QString::fromLocal8Bit("файл")));
    check_port.SetChecked(param.GetComNumber());
    QObject::connect(&check_port, SIGNAL(PressButton(unsigned,bool)), this, SLOT(on_checked_portChanged(unsigned,bool)));
    QObject::connect(&check_port, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckPort()));

    monitor.SetComPort(&com_port);
    QObject::connect(&com_port, SIGNAL(NewState(unsigned)), this, SLOT(OnNewComPortState(unsigned)));
    QObject::connect(&com_port, SIGNAL(NewSostav()), this, SLOT(OnNewSostav()));
    com_port.SetNumberPort(param.GetComNumber());
    com_port.start(QThread::NormalPriority);
    monitor.SetVisible(visiable);

    process_reload = new QProcess(this);
    QObject::connect(process_reload, SIGNAL(finished(int)), this, SLOT(OnEndReload(int)));
}
CInput::~CInput()
{
    now_exit = true;
    com_port.SetNumberPort(100);
    com_port.wait();
}
void CInput::OnNewComPortState(unsigned s)
{
    if(now_exit)
        return;
    switch(s)
    {
        case CComPort::cpsDisConnect:
            label_com_state->setText(QString::fromLocal8Bit("порт отключён"));
            QObject::killTimer(timer_new_second);
            break;
        case CComPort::cpsConnect:
            label_com_state->setText(QString::fromLocal8Bit("cвязь"));
            timer_new_second = QObject::startTimer(1000);
            break;
        case CComPort::cpsConnect1:
            label_com_state->setText(QString::fromLocal8Bit("cвязь"));
            break;
        case CComPort::cpsConnect2:
            label_com_state->setText(QString::fromLocal8Bit("нет данных"));
            break;
        case CComPort::cpsReload:
            label_com_state->setText(QString::fromLocal8Bit("перезагрузка порта"));
            process_reload->start(dir_path + "/usb_reload.bat");
            break;
        case CComPort::cpsError:
            label_com_state->setText(QString::fromLocal8Bit("ошибка порта"));
            break;
        case CComPort::cpsNoState:
            label_com_state->setText(QString::fromLocal8Bit(""));
            break;
        default:
            label_com_state->setText(QString(""));
    }
    //monitor.ClearImage();
}
void CInput::OnNewSostav()
{
    emit NewSostav();
}
void CInput::OnEndReload(int res)
{
    on_checked_portChanged(param.GetComNumber()+100, true);
}
void CInput::on_checked_dat1Changed(unsigned n, bool tmp)
{
    param.SetMonitorValue(n);
    monitor.NewParam();
    check_dat2.SetNotChecked();
}
void CInput::on_checked_dat2Changed(unsigned n, bool tmp)
{
    int n2;
    switch(n)
    {
        case 0: n2 = 12; break;
        case 7: n2 = 13; break;
        default: n2 = n + 5;
    }
    check_dat1.SetNotChecked();
    param.SetMonitorValue(n2);
    monitor.NewParam();
}
void CInput::on_checked_portChanged(unsigned n, bool tmp)
{
    int n2 = (n > 100)? n - 100 : n;
    com_port.SetNumberPort(n);
    param.SetComNumber(n2);
}
void CInput::SetVisiable(bool v)
{
    visiable = v;
    monitor.SetVisible(visiable);
}
void CInput::on_center_monitor_clicked()
{
}
void CInput::on_dial_x_monitor_valueChanged(int value)
{
    int tmp = param.GetMonitorCompressX();
    int tmp2 = tmp + value;
    if(tmp2 <= 0)
        tmp2 = 1;
    if(tmp2 > 100)
        tmp2 = 100;
    if(tmp == tmp2)
        return;
    param.SetMonitorCompressX(tmp2);
    monitor.NewParam();
}
void CInput::on_dial_y_monitor_valueChanged(int value)
{
    int tmp = param.GetMonitorCompressY();
    int tmp2 = tmp + value;
    if(tmp2 <= 0)
        tmp2 = 1;
    if(tmp2 > 300)
        tmp2 = 300;
    if(tmp == tmp2)
        return;
    param.SetMonitorCompressY(tmp2);
    monitor.NewParam();
}
//---------------------------------------------
void CInput::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void CInput::OnReSizeCheckDat1()
{
    QRect rcheck1 = check_dat1.rect();
    QRect rcheck2(check_dat1.x(), check_dat1.y(), rcheck1.width(), rcheck1.height());
    check_dat1.SetImageBack(image_my, rcheck1, rcheck2);
}
void CInput::OnReSizeCheckDat2()
{
    QRect rcheck1 = check_dat2.rect();
    QRect rcheck2(check_dat2.x(), check_dat2.y(), rcheck1.width(), rcheck1.height());
    check_dat2.SetImageBack(image_my, rcheck1, rcheck2);
}
void CInput::OnReSizeCheckPort()
{
    QRect rcheck1 = check_port.rect();
    QRect rcheck2(check_port.x(), check_port.y(), rcheck1.width(), rcheck1.height());
    check_port.SetImageBack(image_my, rcheck1, rcheck2);
}
void CInput::resizeEvent(QResizeEvent *r)
{
//    if(!QWidget::isVisible())
  //      return;
    image_my = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image_my);
    QLinearGradient gradient(0, 0, 0, height());
    QRect rect1(0, 0, width(), height());
    painter.setBrush(Qt::gray);
    painter.drawRect(rect1);
    QColor cg = QColor(0, 30, 185, 255);
    int stap_gr[8] = {160, 220, 220, 220, 220, 220, 255};
    for(int i = 0; i < 8; i ++)
    {
        cg.setAlpha(stap_gr[i]);
        gradient.setColorAt((double)i / 6.0, cg);
    }
    painter.setPen(Qt::NoPen);
  //  painter.setPen(QPen(cg));
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.end();
    OnReSizeCheckDat1();
    OnReSizeCheckDat2();
    OnReSizeCheckPort();
}


