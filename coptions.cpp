#include "coptions.h"

COptions::COptions(QWidget *parent) : QWidget(parent),
    label_about(this)
{
    QPalette palette7;
    QBrush brush7(QColor(200, 200, 200, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);
    QFont font2;
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);

    CLabelChange::SLabelInit struct_label;
    struct_label.color_backgraund = QColor(0, 30, 185, 255);
    struct_label.color_line = QColor(0, 30, 185, 255);
    struct_label.color_text = QColor(255, 255, 255, 255);
    struct_label.font = font2;
    label_about.SetInitStruct(struct_label);
    label_about.AddString(QString::fromLocal8Bit("индикатор весовой нагрузки"));
    label_about.AddString(QString::fromLocal8Bit("ИВН-2-30"));
    label_about.AddString(QString::fromLocal8Bit("версия вторая.ноль"));
    label_about.AddString("kuchynski@tut.by");
    label_about.AddString("+375 447302607");
    label_about.setGeometry(QRect(0, height() - 35, width(), 20));
    label_about.Start();


    CChecks::SButtonsInit chek_init;
    chek_init.style_background = CChecks::SButtonsInit::ebPicture;
    QImage imagea1("OK1.bmp");
    QImage imagea2("OK2.bmp");

    int b_x[12] = {50, 165, 280, 395, 510, 625, 50, 165, 280, 395, 510, 625};
    int b_y[12] = {15, 15, 15, 15, 15, 15, 330, 330, 330, 330, 330, 330};
    for(int i = 0; i < 12; i++)
    {
        buttons_enabled_pb[i] = new CChecks(this);
        buttons_enabled_pb[i]->setGeometry(QRect(b_x[i], b_y[i]+25, 60, 60));
        buttons_enabled_pb[i]->SetInitStruct(chek_init);
        buttons_enabled_pb[i]->AddButton(QString(""), imagea1, imagea2);
        buttons_enabled_pb[i]->SetChecked(0, (param.GetonoffPb(i)));
    }
    QObject::connect(buttons_enabled_pb[0], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb0(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[1], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb1(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[2], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb2(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[3], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb3(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[4], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb4(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[5], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb5(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[6], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb6(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[7], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb7(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[8], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb8(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[9], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb9(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[10], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb10(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[11], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb11(unsigned,bool)));

    QLabel *label_e[5];
    for(int i = 0; i < 12; i ++)
    {
        label_e[i] = new QLabel(this);
        label_e[i]->setGeometry(QRect(b_x[i]+15, b_y[i], 45, 25));
        label_e[i]->setPalette(palette7);
        label_e[i]->setFont(font2);
        label_e[i]->setText(QString::fromLocal8Bit("ПБ") + QString::number(i+1));
    }

    for(int i = 0; i < 12; i++)
    {
        pb_options[i] = new CPBOptions(true, 210, "", this);
//        pb_options[i] = new CPBOptions(true, 300, QString::fromLocal8Bit("кооф. ПБ") + QString::number(i+1), this);
        pb_options[i]->setGeometry(QRect(b_x[i]-20, b_y[i]+90, 100, 210));
        pb_options[i]->SetKoof(param.GetKoofA(i), param.GetKoofB(i), param.GetKoofC(i));
        pb_options[i]->SetNS((param.GetNSPb(i))? true : false);
        pb_options[i]->SetVisible((param.GetonoffPb(i)));
    }
    QObject::connect(pb_options[0], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB0(unsigned, int)));
    QObject::connect(pb_options[1], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB1(unsigned, int)));
    QObject::connect(pb_options[2], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB2(unsigned, int)));
    QObject::connect(pb_options[3], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB3(unsigned, int)));
    QObject::connect(pb_options[4], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB4(unsigned, int)));
    QObject::connect(pb_options[5], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB5(unsigned, int)));
    QObject::connect(pb_options[6], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB6(unsigned, int)));
    QObject::connect(pb_options[7], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB7(unsigned, int)));
    QObject::connect(pb_options[8], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB8(unsigned, int)));
    QObject::connect(pb_options[9], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB9(unsigned, int)));
    QObject::connect(pb_options[10], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB10(unsigned, int)));
    QObject::connect(pb_options[11], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB11(unsigned, int)));
    QObject::connect(pb_options[0], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB0(bool)));
    QObject::connect(pb_options[1], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB1(bool)));
    QObject::connect(pb_options[2], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB2(bool)));
    QObject::connect(pb_options[3], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB3(bool)));
    QObject::connect(pb_options[4], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB4(bool)));
    QObject::connect(pb_options[5], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB5(bool)));
    QObject::connect(pb_options[6], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB6(bool)));
    QObject::connect(pb_options[7], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB7(bool)));
    QObject::connect(pb_options[8], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB8(bool)));
    QObject::connect(pb_options[9], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB9(bool)));
    QObject::connect(pb_options[10], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB10(bool)));
    QObject::connect(pb_options[11], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB11(bool)));


    CSpinBox::SSpinBoxInit spin_init;
    spin_init.height_buttons = 20;
    spin_init.color_backgraund = QColor(30, 30, 155, 255);
    spin_init.color_backgraund_buttons = QColor(2, 20, 105, 255);
    QFont fo;
    fo.setBold(true);
    fo.setPointSize(16);
    fo.setWeight(30);
    spin_init.font = fo;
    spin_init.height_buttons = 14;
    int sb_y[3] = {120, 190, 260};
    for(int i = 0; i < 3; i++)
    {
        spin_box_rast[i] = new CSpinBox(this);
        spin_box_rast[i]->setGeometry(QRect(900, sb_y[i], 50, 60));
        spin_box_rast[i]->SetInitStruct(spin_init);
        spin_box_rast[i]->SetMinMax(0, 150);
        spin_box_rast[i]->SetValue(param.GetRastA(i));
    }
    spin_box_rast[2]->SetMinMax(300, 400);
    spin_box_rast[2]->SetValue(param.GetRastA(2));
    QObject::connect(spin_box_rast[0], SIGNAL(NewValue(int)), this, SLOT(OnRastA0ValueChanged(int)));
    QObject::connect(spin_box_rast[1], SIGNAL(NewValue(int)), this, SLOT(OnRastA1ValueChanged(int)));
    QObject::connect(spin_box_rast[2], SIGNAL(NewValue(int)), this, SLOT(OnRastA2ValueChanged(int)));

    QLabel *label_sb[4];
    int label_sb_y[4] = {15, sb_y[0]+20, sb_y[1]+20, sb_y[2]+20};
    for(int i = 0; i < 4; i ++)
    {
        label_sb[i] = new QLabel(this);
        label_sb[i]->setGeometry(QRect(740, label_sb_y[i], 160, 21));
        label_sb[i]->setPalette(palette7);
        label_sb[i]->setFont(font2);
    }
    label_sb[0]->setText(QString::fromLocal8Bit("расстояния"));
    label_sb[1]->setText(QString::fromLocal8Bit("между ДМ1 и ПБ1"));
    label_sb[2]->setText(QString::fromLocal8Bit("между ДМ2 и ПБ6"));
    label_sb[3]->setText(QString::fromLocal8Bit("между ДМ1 и ДМ2"));


}
void COptions::NewKoof()
{
    for(int i = 0; i < 4; i++)
        pb_options[i]->SetKoof(param.GetKoofA(i), param.GetKoofB(i), param.GetKoofC(i));
}
void COptions::SetKoof(unsigned pb, unsigned n, int value)
{
    switch(n)
    {
        case 0: param.SetKoofA(pb,  value); break;
        case 1: param.SetKoofB(pb,  value); break;
        case 2: param.SetKoofC(pb,  value); break;
    }
}
void COptions::OnChangedKoofPB0(unsigned n, int value)
{
    SetKoof(0, n, value);
}
void COptions::OnChangedKoofPB1(unsigned n, int value)
{
    SetKoof(1, n, value);
}
void COptions::OnChangedKoofPB2(unsigned n, int value)
{
    SetKoof(2, n, value);
}
void COptions::OnChangedKoofPB3(unsigned n, int value)
{
    SetKoof(3, n, value);
}
void COptions::OnChangedKoofPB4(unsigned n, int value)
{
    SetKoof(4, n, value);
}
void COptions::OnChangedKoofPB5(unsigned n, int value)
{
    SetKoof(5, n, value);
}
void COptions::OnChangedKoofPB6(unsigned n, int value)
{
    SetKoof(6, n, value);
}
void COptions::OnChangedKoofPB7(unsigned n, int value)
{
    SetKoof(7, n, value);
}
void COptions::OnChangedKoofPB8(unsigned n, int value)
{
    SetKoof(8, n, value);
}
void COptions::OnChangedKoofPB9(unsigned n, int value)
{
    SetKoof(9, n, value);
}
void COptions::OnChangedKoofPB10(unsigned n, int value)
{
    SetKoof(10, n, value);
}
void COptions::OnChangedKoofPB11(unsigned n, int value)
{
    SetKoof(11, n, value);
}
void COptions::OnRastA0ValueChanged(int value)
{
    param.SetRastA(0, value);
}
void COptions::OnRastA1ValueChanged(int value)
{
    param.SetRastA(1, value);
}
void COptions::OnRastA2ValueChanged(int value)
{
    param.SetRastA(2, value);
}
void COptions::OnCheckedEnabledPb0(unsigned n, bool s)
{
    param.SetonoffPb(0, (s)? 1 : 0);
    pb_options[0]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb1(unsigned n, bool s)
{
    param.SetonoffPb(1, (s)? 1 : 0);
    pb_options[1]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb2(unsigned n, bool s)
{
    param.SetonoffPb(2, (s)? 1 : 0);
    pb_options[2]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb3(unsigned n, bool s)
{
    param.SetonoffPb(3, (s)? 1 : 0);
    pb_options[3]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb4(unsigned n, bool s)
{
    param.SetonoffPb(4, (s)? 1 : 0);
    pb_options[4]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb5(unsigned n, bool s)
{
    param.SetonoffPb(5, (s)? 1 : 0);
    pb_options[5]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb6(unsigned n, bool s)
{
    param.SetonoffPb(6, (s)? 1 : 0);
    pb_options[6]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb7(unsigned n, bool s)
{
    param.SetonoffPb(7, (s)? 1 : 0);
    pb_options[7]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb8(unsigned n, bool s)
{
    param.SetonoffPb(8, (s)? 1 : 0);
    pb_options[8]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb9(unsigned n, bool s)
{
    param.SetonoffPb(9, (s)? 1 : 0);
    pb_options[9]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb10(unsigned n, bool s)
{
    param.SetonoffPb(10, (s)? 1 : 0);
    pb_options[10]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb11(unsigned n, bool s)
{
    param.SetonoffPb(11, (s)? 1 : 0);
    pb_options[11]->SetVisible(s);
}
void COptions::OnCheckedNSPB0(bool s)
{
    param.SetNSPb(0, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB1(bool s)
{
    param.SetNSPb(1, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB2(bool s)
{
    param.SetNSPb(2, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB3(bool s)
{
    param.SetNSPb(3, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB4(bool s)
{
    param.SetNSPb(4, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB5(bool s)
{
    param.SetNSPb(5, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB6(bool s)
{
    param.SetNSPb(6, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB7(bool s)
{
    param.SetNSPb(7, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB8(bool s)
{
    param.SetNSPb(8, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB9(bool s)
{
    param.SetNSPb(9, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB10(bool s)
{
    param.SetNSPb(10, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB11(bool s)
{
    param.SetNSPb(11, (s)? 1 : 0);
}
//---------------------------------------------
void COptions::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void COptions::resizeEvent(QResizeEvent *r)
{
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
//    painter.setPen(QPen(cg));
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.setPen(QPen(Qt::white));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(QRect(720, 105, 250, 230), 5, 5);
    painter.end();

    label_about.setGeometry(QRect(0, height() - 40, width(), 20));
    label_about.SetImageBack(image_my, label_about.rect(), QRect(0, height() - 40, width(), 20));
}
