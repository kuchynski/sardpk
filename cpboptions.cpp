#include "cpboptions.h"

CPBOptions::CPBOptions(bool ens, int h, QString n, QWidget *parent) : QWidget(parent), name(n),
    max_height(h), sm_height(h), visiable(false), timer_id(0), enabled_ns(ens)
{
    QWidget::setFixedHeight(0);
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
    for(int i = 0; i < 2; i++)
    {
        spin_box_koof[i] = new CSpinBox(this);
        spin_box_koof[i]->setGeometry(QRect(30, -h, 50, 60));
        spin_box_koof[i]->SetInitStruct(spin_init);
        spin_box_koof[i]->setCursor(QCursor(Qt::PointingHandCursor));
    }
    spin_box_koof[0]->SetMinMax(1, 99);
    spin_box_koof[1]->SetMinMax(1, 999);
    QObject::connect(spin_box_koof[0], SIGNAL(NewValue(int)), this, SLOT(OnKoofAChanged(int)));
    QObject::connect(spin_box_koof[1], SIGNAL(NewValue(int)), this, SLOT(OnKoofBChanged(int)));

    for(int i = 0; i < 3; i ++)
    {
        label_sb[i] = new QLabel(this);
        label_sb[i]->setGeometry(QRect(10, -h, 35, 21));
        label_sb[i]->setPalette(palette7);
        label_sb[i]->setFont(font2);
    }
    label_sb[0]->setText(name);
    label_sb[0]->setGeometry(QRect(10, -h, 35, 21));
    label_sb[1]->setText(QString::fromLocal8Bit("деф"));
    label_sb[2]->setText(QString::fromLocal8Bit("вес"));

    if(enabled_ns)
    {
        button_ns = new CChecks(this);
        button_ns->setGeometry(QRect(30, 100, 80, 40));
        CChecks::SButtonsInit chek_init;
        chek_init.style_background = CChecks::SButtonsInit::ebPicture;
        button_ns->SetInitStruct(chek_init);
        button_ns->setCursor(QCursor(Qt::PointingHandCursor));

        QImage image1("NS.bmp");
        QImage image2("SN.bmp");
        button_ns->AddButton(QString(""), image1, image2);//QString::fromLocal8Bit("ПБ") + QString::number(i+1)
        QObject::connect(button_ns, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedNSPB(unsigned,bool)));
    }

    SetGeometry();
}
void CPBOptions::SetKoof(unsigned a, unsigned b, int c)
{
    spin_box_koof[0]->SetValue(a);
    spin_box_koof[1]->SetValue(b);
}
void CPBOptions::SetNS(bool c)
{
    button_ns->SetChecked(0, c);
}
void CPBOptions::OnKoofAChanged(int value)
{
    emit ChangeKoof(0, value);
}
void CPBOptions::OnKoofBChanged(int value)
{
    emit ChangeKoof(1, value);
}
void CPBOptions::SetGeometry()
{
    int sb_y[2] = {145, 75};
    int h = height();
    for(int i = 0; i < 2; i++)
    {
        if(enabled_ns)
            sb_y[i] += 50;
        spin_box_koof[i]->move(40, h - sb_y[i]);
        label_sb[i+1]->move(5, h - sb_y[i] + 20);
    }
    label_sb[0]->move(5, h - ((enabled_ns)? 295 : 245));
    if(enabled_ns)
        button_ns->move(10, h - 55);
}
void CPBOptions::timerEvent(QTimerEvent *event)
{
    int tmp = sm_height;
    if(sm_height < 0 || sm_height > max_height || sm_height == sm_height_end)//что-то не так пошло в прорисовке
    {
        sm_height = (visiable)? 0 : max_height;
        killTimer(timer_id);
        timer_id = 0;
    }
    else
       sm_height += sm_height_step;
    QWidget::setFixedHeight(max_height - tmp);
    SetGeometry();
}
void CPBOptions::SetVisible(bool v)
{
    visiable = v;
    sm_height_step = (v)? -max_height / 10 : max_height / 10;
    sm_height_end = (v)? 0 : sm_height_step * 10;
    if(!timer_id)
        timer_id = QObject::startTimer(25);
}
//---------------------------------------------
void CPBOptions::paintEvent(QPaintEvent *p)
{
//    if(!QWidget::isVisible())
  //      return;
    QWidget::paintEvent(p);
    painter.begin(this);
    QRect r2(0, max_height - height(), width(), height());
    painter.drawImage(rect(), image_my, r2);
    painter.end();
}
void CPBOptions::resizeEvent(QResizeEvent *r)
{
//    if(!QWidget::isVisible())
  //      return;
    if(image_my.height() != max_height)
    {
        image_my = QImage(width(), max_height, QImage::Format_RGB32);
        painter.begin(&image_my);
        QLinearGradient gradient(0, 0, width(), width());//height());
        QRect rect1(0, 0, width(), max_height);
        QRect rect2(0, 0, width()-1, max_height-1);
        painter.setBrush(Qt::gray);
        painter.drawRect(rect1);
        QColor cg = QColor(0, 30, 185, 255);
        int stap_gr[4] = {255, 120, 140, 200};
        for(int i = 0; i < 4; i ++)
        {
            cg.setAlpha(stap_gr[i]);
            gradient.setColorAt((double)i / 3.0, cg);
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRect(rect1);
        painter.setPen(QPen(Qt::white));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect2, 5, 5);
        painter.end();
    }
}
void CPBOptions::OnCheckedNSPB(unsigned n, bool s)
{
    emit CheckedNSPB(s);
}
