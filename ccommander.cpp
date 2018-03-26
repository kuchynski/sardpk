#include "ccommander.h"

CCommander::CCommander(QWidget *parent): CAllWindows(parent),
    bd(this), input(this), options(this)
{
    QObject::connect(&input, SIGNAL(NewSostav()), this, SLOT(OnNewSostav()));
    QObject::connect(this, SIGNAL(NewWindow(unsigned)), this, SLOT(OnPressButton(unsigned)));
    SAllWindowsInit init_struct;
    init_struct.direction_frame = SAllWindowsInit::edHorizontal;
    init_struct.direction_widget = SAllWindowsInit::edHorizontal;
    init_struct.rewindow_time_all = 500;
    init_struct.rewindow_kol_step = 12;
    init_struct.frame_height = 25;
    init_struct.frame_width = 0;
    SetInitStruct(init_struct);

    QPixmap pix0("0.bmp");
    AddWidget(&bd, QString::fromLocal8Bit("архив"), pix0);
    QPixmap pix1("1.bmp");
    AddWidget(&input, QString::fromLocal8Bit("монитор"), pix1);
    QPixmap pix2("2.bmp");
    AddWidget(&options, QString::fromLocal8Bit("настройки"), pix2);
 //   QPixmap pix3("4.png");
 //   AddWidget(&options, QString::fromLocal8Bit("выход"), pix3);

}
void CCommander::OnNewSostav()
{
    CSostav sostav;
    input.GetSostav(sostav);
    bd.AddSostav(sostav);
}

void CCommander::OnPressButton(const unsigned n)
{
    if(n == 3)
        exit(1);
}
void CCommander::OnNewKoof(SElement12<unsigned> a, SElement12<unsigned> b, SElement12<int> c)
{
    param.SetKoofA(a);
    param.SetKoofB(b);
    param.SetKoofC(c);
    options.NewKoof();
}
