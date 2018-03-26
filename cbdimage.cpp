#include "cbdimage.h"

CBDImage::CBDImage(QWidget *parent): QWidget(parent), side(0),
    image(this), image_defect(this), check_pb(this), check_axel(this), label_type_of_van(this)
{
    QBoxLayout *pbx = new QBoxLayout(QBoxLayout::TopToBottom);
    pbx->setMargin(0);
    pbx->setSpacing(0);
    QWidget::setLayout(pbx);

 //   image.setMinimumHeight(250);
 //   image.setMaximumHeight(250);
    pbx->addWidget(&image, 1);
    QObject::connect(&image, SIGNAL(Drag(int)), this, SLOT(DragImage(int)));
    QObject::connect(&image, SIGNAL(WheelX(int)), this, SLOT(CompressXImage(int)));
    QObject::connect(&image, SIGNAL(WheelY(int)), this, SLOT(CompressYImage(int)));
    QObject::connect(&image, SIGNAL(DoubleClick()), this, SLOT(DoubleClickImage()));
    QObject::connect(&image, SIGNAL(IAmReSize()), this, SLOT(OnReSizeImage()));

    check_pb.setMaximumHeight(40);
    check_pb.setMinimumHeight(40);
    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_backgraund2 = QColor(185, 30, 185, 255);
    chek_init.color_backgraund_checked2 = QColor(255, 100, 255, 255);
//    chek_init.color_backgraund2 = QColor(250, 30, 50, 255);
  //  chek_init.color_backgraund_checked2 = QColor(255, 100, 100, 255);
    chek_init.color_text = Qt::white;
    chek_init.color_text_checked = Qt::red;
    check_pb.SetInitStruct(chek_init);
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡7" : "œ¡1")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡8" : "œ¡2")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡9" : "œ¡3")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡10" : "œ¡4")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡11" : "œ¡5")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡12" : "œ¡6")));
    for(unsigned i = 0; i < 6; i ++)
        check_pb.SetChecked(i, param.GetArchonoffPb(i));
    QObject::connect(&check_pb, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedPB(unsigned,bool)));
    QObject::connect(&check_pb, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheck()));
    pbx->addWidget(&check_pb, 2);

    QPalette palette7;
    QBrush brush7(QColor(245, 245, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);
    QFont font4;
    font4.setPointSize(10);
    label_type_of_van.setFont(font4);
    label_type_of_van.setPalette(palette7);
    label_type_of_van.setMaximumHeight(20);
    label_type_of_van.setMinimumHeight(20);
    label_type_of_van.setAlignment(Qt::AlignCenter);

    pbx->addWidget(&label_type_of_van, 3);

    check_axel.setMaximumHeight(40);
    check_axel.setMinimumHeight(40);
    check_axel.SetInitStruct(chek_init);
    QObject::connect(&check_axel, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedAxel(unsigned,bool)));
    QObject::connect(&check_axel, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckAxel()));
    pbx->addWidget(&check_axel, 4);

    image_defect.setMaximumHeight(300);
    image_defect.setMinimumHeight(300);
    pbx->addWidget(&image_defect, 5);
    QObject::connect(&image_defect, SIGNAL(IAmReSize()), this, SLOT(OnReSizeImageDefect()));

    setAttribute(Qt::WA_PaintOnScreen);

    check_pb.Paint();
}
void CBDImage::Clear()
{
    sostav = CSostav();
    check_axel.SubButton();
    image.ClearImage();
    dial_arch = 0;
}
void CBDImage::SetSide(unsigned s)
{
    side = s;
    check_pb.SubButton();
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡7" : "œ¡1")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡8" : "œ¡2")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡9" : "œ¡3")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡10" : "œ¡4")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡11" : "œ¡5")));
    check_pb.AddButton(CChecks::SButton(QString::fromLocal8Bit((side)? "œ¡12" : "œ¡6")));
    unsigned add = (side)? 6 : 0;
    for(unsigned i = 0; i < 6; i ++)
        check_pb.SetChecked(i, param.GetArchonoffPb(i+add));
    check_pb.Paint();
    SetActivVan();
    RePicGraph();
}
//---------------------------------------------
void CBDImage::RePicGraph()
{
    sostav.PicGraphik(&image, side, dial_arch, false);
    sostav.PicDefect(&image_defect, side, activ_axel);
}
//---------------------------------------------------------------------------
void CBDImage::SetSostav(CSostav &s)
{
    sostav = s;
    try
    {
        sostav.LoadData();
        SetActivVan(0);
    }
    catch(CSostav *cs){}
}
//---------------------------------------------------------------------------
void CBDImage::CloseSostav()
{
    sostav = CSostav();
    check_axel.SubButton();
    label_type_of_van.setText("");
    sostav.PicGraphik(&image, side, dial_arch, false);
    sostav.PicDefect(&image_defect, side, activ_axel);
}
//---------------------------------------------
void CBDImage::SetActivVan(unsigned row)
{
    if(row == 1000)
        row = sostav.GetActivVan();
    const unsigned kol_axel = sostav.GetSizeVan(row);
    unsigned first_os_with_defect = 1000;
    check_axel.SubButton();
    for(unsigned i = 0; i < kol_axel; i ++)
    {
        unsigned os = (sostav.GetDirection())? i : kol_axel - (i+1);
        check_axel.AddButton(CChecks::SButton(QString::number(os+1) + QString::fromLocal8Bit(" ÓÒ¸")));
    }
    for(unsigned i = 0; i < kol_axel; i ++)
    {
        unsigned os = (sostav.GetDirection())? i : kol_axel - (i+1);
        bool def = sostav.GetDefect(row, i, (side)? 14 : 13);
        check_axel.SetChecked2(os, def);
        if(def && (first_os_with_defect == 1000))
            first_os_with_defect = os;
    }
    if(first_os_with_defect == 1000)
        first_os_with_defect = (sostav.GetDirection())? 0 : kol_axel - 1;
    activ_axel = (sostav.GetDirection())? first_os_with_defect : kol_axel - (first_os_with_defect + 1);
    check_axel.SetChecked(first_os_with_defect);
    check_axel.Paint();
    emit DefectForButtonSide(sostav.GetDefect(row, 100, 13), sostav.GetDefect(row, 100, 14));
//FILE *f1 = fopen("12.txt", "at"); fprintf(f1, "-%u ", first_os_with_defect); fclose(f1);
    sostav.SetActivVan(&image, side, dial_arch, row);
    label_type_of_van.setText(sostav.GetTypeActivVan());
    sostav.PicDefect(&image_defect, side, activ_axel);
}
void CBDImage::OnCheckedPB(unsigned n, bool s)
{
    unsigned add = (side)? 6 : 0;
    param.SetArchonoffPb(n+add, (s)? 1 : 0);
    RePicGraph();
}
void CBDImage::OnCheckedAxel(unsigned n, bool s)
{
    const unsigned kol_axel = sostav.GetSizeVan(sostav.GetActivVan());
    activ_axel = (sostav.GetDirection())? n : kol_axel - (n + 1);
    sostav.PicDefect(&image_defect, side, activ_axel);
}
void CBDImage::DragImage(int s)
{
    int max_dial = sostav.GetSizeData();
    if(sostav.GetDirection())
        s = -s;
    const double compress_hor = param.GetArchCompressxPb() / 10.0;
    const double tmp = compress_hor * (double)s;
    dial_arch += tmp;
    if(dial_arch < 0)
        dial_arch = 0;
    if(dial_arch >= max_dial)
        dial_arch = max_dial - 1;
    RePicGraph();
}
void CBDImage::DoubleClickImage()
{
    param.SetArchTogether(!param.GetArchTogether());
    RePicGraph();
}
void CBDImage::CompressXImage(int s)
{
    int cx = param.GetArchCompressxPb() + s;
    if(cx < 1)
        cx = 1;
    if(cx > 100)
        cx = 100;
 //   int begin_pic = dial_arch;
    int center_pic = dial_arch + (image.GetWidth() * param.GetArchCompressxPb()/10) / 2;
  //  int center_pic = (begin_pic + end_pic)/2;
    param.SetArchCompressxPb(cx);
    dial_arch = center_pic - (image.GetWidth() * param.GetArchCompressxPb()/10) / 2;
    RePicGraph();
}
void CBDImage::CompressYImage(int s)
{
    int cy = param.GetArchCompressyPb() + s;
    if(cy < 50)
        cy = 50;
    if(cy > 500)
        cy = 500;
    param.SetArchCompressyPb(cy);
    RePicGraph();
}
void CBDImage::OnReSizeImage()
{
    QRect rimage1 = image.rect();
    QRect rimage2(0, image.y(), width(), rimage1.height());
    image.SetImageBack(image_back, rimage1, rimage2);
}
void CBDImage::OnReSizeCheck()
{
    QRect rcheck1 = check_pb.rect();
    QRect rcheck2(0, check_pb.y(), width(), rcheck1.height());
    check_pb.SetImageBack(image_back, rcheck1, rcheck2);
}
void CBDImage::OnReSizeImageDefect()
{
    QRect rcheck1 = image_defect.rect();
    QRect rcheck2(0, image_defect.y(), width(), rcheck1.height());
    image_defect.SetImageBack(image_back, rcheck1, rcheck2);
}
void CBDImage::OnReSizeCheckAxel()
{
    QRect rcheck1 = check_axel.rect();
    QRect rcheck2(0, check_axel.y(), width(), rcheck1.height());
    check_axel.SetImageBack(image_back, rcheck1, rcheck2);
}
void CBDImage::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    OnReSizeCheckAxel();
    OnReSizeImageDefect();
    OnReSizeImage();
    OnReSizeCheck();
}
void CBDImage::resizeEvent(QResizeEvent *r)
{
    QWidget::resizeEvent(r);
    emit IAmReSize();
    RePicGraph();
}
