#include "cbd.h"

CBD::CBD(QWidget *parent): QWidget(parent),
    file_path(QDir::current().absolutePath()), bd_image(this), check_side(this),
    calendar(this), table_sostavs(this), table_vans(this), buttons(this), proc_pdf(this)
{
    QBoxLayout *pbx = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout *pbx1 = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *pbx2 = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *pbx3 = new QBoxLayout(QBoxLayout::TopToBottom);
    pbx->setMargin(0);
    pbx1->setMargin(0);
    pbx2->setMargin(0);
    pbx3->setMargin(0);
    pbx->setSpacing(0);
    pbx1->setSpacing(0);
    pbx2->setSpacing(0);
    pbx3->setSpacing(0);
    w1 = new QWidget();
    w2 = new QWidget();
    w3 = new QWidget();
    w1->setLayout(pbx1);
    w2->setLayout(pbx2);
    w3->setLayout(pbx3);
    w1->setMinimumWidth(230);
    w1->setMaximumWidth(230);
    w2->setMinimumWidth(260);
    w2->setMaximumWidth(260);
    pbx->addWidget(w1, 1);
    pbx->addWidget(w2, 2);
    pbx->addWidget(w3, 3);
    QWidget::setLayout(pbx);

    calendar.setMaximumHeight(150);
    calendar.setMinimumHeight(150);
    calendar.setFirstDayOfWeek(Qt::Monday);
    calendar.setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    QPalette palette = calendar.palette();
    palette.setColor(QPalette::Base, QColor(0, 30, 185, 100));
    palette.setColor(QPalette::Text, QColor(255, 255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(0, 0, 200, 155));
    //palette.setColor(QPalette::ToolTipText, QColor(255, 255, 0, 255));
    calendar.setPalette(palette);
    QObject::connect(&calendar, SIGNAL(selectionChanged()), this, SLOT(CalendarChanged()));
    pbx1->addWidget(&calendar, 1);
    calendar.show();

    CTable::STableInit table_init;
    table_init.color_backgraund = QColor(0, 30, 185, 255);
//    table_init.color_backgraund_checked = QColor(150, 155, 255, 255);
    table_init.color_backgraund_checked = QColor(100, 150, 255, 255);
    table_init.color_backgraund_button = QColor(0, 30, 185, 255);
    table_init.color_line_checked = QColor(200, 200, 255, 255);
    table_init.color_text_checked = QColor(255, 255, 0, 255);
    table_init.color_text_title = QColor(255, 255, 0, 255);
//    table_init.color_backgraund_button = QColor(50, 100, 255, 255);
    table_init.color_backgraund_title = QColor(0, 0, 200, 255);
    table_init.font = this->font();
    table_init.font.setPixelSize(12);
    table_init.hieght_line = 20;
    QList<unsigned> width_line;
    width_line << 48 << 43 << 40 << 40 << 54;
    table_init.width_line = width_line;
    table_sostavs.SetInitStruct(table_init);
    QList<QString> t;
    t << QString::fromLocal8Bit("Время") << QString::fromLocal8Bit("Напр.") << QString::fromLocal8Bit("Лок.") <<
         QString::fromLocal8Bit("Ваг.") << QString::fromLocal8Bit("Дефект");
    table_sostavs.SetTitle(t);
    QObject::connect(&table_sostavs, SIGNAL(ClickLine(unsigned)), this, SLOT(TableSostavClicked(unsigned)));
    QObject::connect(&table_sostavs, SIGNAL(IAmReSize()), this, SLOT(OnReSizeTableSostav()));
    pbx1->addWidget(&table_sostavs, 2);
    table_sostavs.resize(300, 2000);

    QList<unsigned> width_line2;
    width_line2 << 25 << 39 << 61 << 43 << 43 << 43;
    table_init.width_line = width_line2;
    table_vans.SetInitStruct(table_init);
//    table_vans.setFocusPolicy(Qt::ClickFocus);
    QList<QString> t2;
    t2 << QString::fromLocal8Bit("№") << QString::fromLocal8Bit("Осей") << QString::fromLocal8Bit("Скорость") <<
          QString::fromLocal8Bit("Вес") << QString::fromLocal8Bit("Деф1") << QString::fromLocal8Bit("Деф2");
    table_vans.SetTitle(t2);
    pbx2->addWidget(&table_vans, 1);
    table_vans.resize(300, 2000);
    QObject::connect(&table_vans, SIGNAL(ClickLine(unsigned)), this, SLOT(TableVanClicked(unsigned)));
    QObject::connect(&table_vans, SIGNAL(IAmReSize()), this, SLOT(OnReSizeTableVan()));

    buttons.setMaximumHeight(180);
    buttons.setMinimumHeight(180);
    CButtons::SButtonsInit but_init;
    but_init.font = this->font();
    but_init.font.setPixelSize(15);
    but_init.color_backgraund = QColor(0, 30, 185, 255);
    but_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    but_init.color_text = Qt::white;
    buttons.SetInitStruct(but_init);
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("скрыть")));
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("настроить печать")));
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("печатать")));
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("создать pdf")));
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("удалить")));
    QObject::connect(&buttons, SIGNAL(PressButton(unsigned, bool)), this, SLOT(OnButtonsClick(unsigned, bool)));
    QObject::connect(&buttons, SIGNAL(IAmReSize()), this, SLOT(OnReSizeButtons()));
    pbx1->addWidget(&buttons, 3);
    buttons.Paint();

    check_side.setMaximumHeight(40);
    check_side.setMinimumHeight(40);
    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_text = Qt::white;
    chek_init.color_text_checked = Qt::red;
    check_side.SetInitStruct(chek_init);
    check_side.AddButton(CChecks::SButton(QString::fromLocal8Bit("1 сторона")));
    check_side.AddButton(CChecks::SButton(QString::fromLocal8Bit("2 сторона")));
    check_side.SetChecked(0);
    QObject::connect(&check_side, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedSide(unsigned,bool)));
    QObject::connect(&check_side, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckSide()));

    pbx3->addWidget(&check_side, 1);
    pbx3->addWidget(&bd_image, 2);
    QObject::connect(&bd_image, SIGNAL(IAmReSize()), this, SLOT(OnReSizeBDImage()));
    QObject::connect(&bd_image, SIGNAL(DefectForButtonSide(bool, bool)), this, SLOT(DefectForButtonSide(bool , bool)));


    setAttribute(Qt::WA_PaintOnScreen);
    w1->setAttribute(Qt::WA_PaintOnScreen);
    w2->setAttribute(Qt::WA_PaintOnScreen);
    w3->setAttribute(Qt::WA_PaintOnScreen);
    CalendarChanged();

    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
}
void CBD::Clear()
{
    CStack<CSostav>::Clear();
    activ_sostav = -1;
    table_sostavs.Clear(true);
    table_vans.Clear(true);
    bd_image.CloseSostav();
    bd_image.setVisible(false);
    check_side.setVisible(false);
}
bool CBD::SetDir(const QDate &_date)
{
    int year = _date.year();
    int mon = _date.month();
    int day = _date.day();
    if(!dir.cd(file_path))
          return false;
    QString dir_name1(QString("%1_%2").arg(year, 4, 10, QLatin1Char('0')).arg(mon, 2, 10, QLatin1Char('0')));
    QString dir_name2(QString("%1").arg(day, 2, 10, QLatin1Char('0')));
    if(!dir.cd(dir_name1))
      return false;
    if(!dir.cd(dir_name2))
      return false;
    tek_file_path = dir.absolutePath();
    return true;
}
void CBD::CalendarChanged()
{
    date = calendar.selectedDate();
    Clear();
    if(!SetDir(date))
    {
        PaintGridSostav();
        return;
    }
    QStringList list(QString("*.so"));
    QStringList sr = dir.entryList(list, QDir::Files);
    for(int i = 0; i < sr.size(); i ++)
    {
        QString file_name = tek_file_path + "/" + sr[i];
        CSostav s;
        try
        {
            s = CSostav(file_name);
        }
        catch(CSostav *cs){}
        if(s.GetQDate() == date)
            Push(s);
    }
    activ_sostav = 0;
    PaintGridSostav();
}
bool CBD::YesOrNoYearMonDay(const QDate &date)
{
    if(!SetDir(date))
        return false;

    QStringList list(QString("*.so"));
    QStringList sr = dir.entryList(list, QDir::Files);

    return sr.size() != 0;
}
void CBD::OnCheckedSide(unsigned n, bool state)
{
    bd_image.SetSide(n);
}
//---------------------------------------------
void CBD::PaintGridSostav()
{
    bd_image.setVisible(true);
    check_side.setVisible(true);
    table_sostavs.Clear();
    for(int i = 0; i < GetSize(); i ++)
    {
        QList<QString> l;
        (*this)[i].PaintSostav(l);
        table_sostavs.AddLine(l);
    }
    table_sostavs.SetChecked(0);
    table_sostavs.Paint();
    TableSostavClicked(0);
}
//---------------------------------------------
void CBD::TableSostavClicked(unsigned row, const bool &deactiv)
{
    if(!GetSize())
    {
        Clear();
        buttons.setVisible(false);
        return;
    }
    buttons.setVisible(true);
    if(row >= GetSize())
        row = GetSize() - 1;
    if(row < 0)
        return;
    activ_sostav = row;
    bd_image.SetSostav((*this)[activ_sostav]);
    (*this)[activ_sostav].SetActiv(&table_vans);
}
//---------------------------------------------
void CBD::TableVanClicked(unsigned row)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    bd_image.SetActivVan(row);
}
//---------------------------------------------
void CBD::RePicGraph()
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    bd_image.RePicGraph();
}
//---------------------------------------------------------------------------
bool CBD::AddSostav(CSostav &sostav)
{
    QDir dir_s;
    CDateTime date_time = sostav.GetDateTime();
    int year = date_time.GetYear();
    int mon = date_time.GetMon();
    int day = date_time.GetDay();
    if(!dir_s.cd(file_path))
          return false;
    QString dir_name1(QString("%1_%2").arg(year, 4, 10, QLatin1Char('0')).arg(mon, 2, 10, QLatin1Char('0')));
    QString dir_name2(QString("%1").arg(day, 2, 10, QLatin1Char('0')));
    if(!dir_s.cd(dir_name1))
    {
        if(!dir_s.mkdir(dir_name1))
            return false;
        if(!dir_s.cd(dir_name1))
            return false;
    }
    if(!dir_s.cd(dir_name2))
    {
        if(!dir_s.mkdir(dir_name2))
            return false;
        if(!dir_s.cd(dir_name2))
            return false;
    }
    QString file_name = dir_s.absolutePath() + "/" + date_time.GetTime2();

    bool ret = true;
    if(!sostav.SaveToFile(file_name + ".so"))
        ret = false;
    if(!sostav.SaveToFileTxt(file_name + ".dtv"))
        ret = false;
    if(!sostav.SaveToFileBin(file_name + ".bin"))
        ret = false;

    if(ret && (date == sostav.GetQDate()))
    {
        bd_image.setVisible(true);
        check_side.setVisible(true);
        try
        {
            CSostav s(file_name + ".so");
            Push(s);
            QList<QString> l;
            (*this)[GetSize()-1].PaintSostav(l);
            table_sostavs.AddLine(l);
            table_sostavs.Paint();
            if(GetSize() == 1)
                TableSostavClicked(0);
        }
        catch(CSostav *cs){}
    }
    return ret;
}
//---------------------------------------------
void CBD::OnButtonsClick(unsigned n, bool s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    switch(n)
    {
        case 0: CloseSostav(); break;
        case 1: PrintDiallog(); break;
        case 2: PrintSostav(); break;
        case 3: PrintPdfSostav(); break;
        case 4: //удалить
            QString delete_file_name = (*this)[activ_sostav].GetFileName();
            CloseSostav();
            QFile::remove(delete_file_name);
            break;
    }
}
void CBD::PrintDiallog()
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    QPrintDialog print_diallog(&printer, this);
    print_diallog.setMinMax(1, 1);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
    print_diallog.exec();
}
void CBD::PrintSostav()
{
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    printer.setDocName((*this)[activ_sostav].GetFileName());
    (*this)[activ_sostav].Print(&printer);
}
void CBD::PrintPdfSostav()
{
    QPrinter printer_pdf;
    printer_pdf.setOrientation(QPrinter::Portrait);
    printer_pdf.setPageSize(QPrinter::A4);
    printer_pdf.setOutputFormat(QPrinter::PdfFormat);
    printer_pdf.setOutputFileName((*this)[activ_sostav].GetFileName() + ".pdf");
    printer_pdf.setDocName((*this)[activ_sostav].GetFileName());
    (*this)[activ_sostav].Print(&printer_pdf);
//    proc_pdf.start("cmd");
  //  proc_pdf.start((*this)[activ_sostav].GetFileName() + ".pdf");
}
void CBD::CloseSostav()
{
    (*this).Erase(activ_sostav);
    table_sostavs.SubLine(activ_sostav);
    TableSostavClicked(activ_sostav, false);
    table_sostavs.SetChecked(activ_sostav);
}
void CBD::DefectForButtonSide(bool s1, bool s2)
{
    check_side.SetChecked2(0, s1);
    check_side.SetChecked2(1, s2);
    check_side.Paint();
}
void CBD::OnReSizeTableSostav()
{
    QRect rsostav1 = table_sostavs.rect();
    QRect r1 = w1->frameGeometry();
    QRect rsostav2(r1.x(), r1.y() + calendar.height(), rsostav1.width(), rsostav1.height());
    table_sostavs.SetImageBack(image_my, rsostav1, rsostav2);
}
void CBD::OnReSizeTableVan()
{
    QRect rvan1 = table_vans.rect();
    QRect r2 = w2->frameGeometry();
    QRect rvan2(r2.x(), r2.y(), rvan1.width(), rvan1.height());
    table_vans.SetImageBack(image_my, rvan1, rvan2);
}
void CBD::OnReSizeBDImage()
{
    QRect rimage1 = bd_image.rect();
    QRect r3 = w3->frameGeometry();
    QRect rimage2(r3.x(), r3.y(), rimage1.width(), rimage1.height());
    bd_image.SetImageBack(image_my, rimage1, rimage2);
}
void CBD::OnReSizeCheckSide()
{
    QRect rimage1 = check_side.rect();
    QRect r3 = w3->frameGeometry();
    QRect rimage2(r3.x(), r3.y(), rimage1.width(), rimage1.height());
    check_side.SetImageBack(image_my, rimage1, rimage2);
}
void CBD::OnReSizeButtons()
{
    QRect rbuttons1 = buttons.rect();
    QRect rb = w1->frameGeometry();
    QRect rbuttons2(rb.x(), rb.height() - rbuttons1.height(), rbuttons1.width(), rbuttons1.height());
    buttons.SetImageBack(image_my, rbuttons1, rbuttons2);
}
//---------------------------------------------
void CBD::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void CBD::resizeEvent(QResizeEvent *r)
{
//    if(!QWidget::isVisible())
  //      return;
  //  QWidget::resizeEvent(r);
    image_my = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image_my);
//    QLinearGradient gradient(0, 0, width(), width());//height());
    QLinearGradient gradient(0, 0, 0, height());
    QRect rect1(0, 0, width(), height());
    painter.setBrush(Qt::gray);
    painter.drawRect(rect1);
    QColor cg = QColor(0, 30, 185, 255);
//    QColor cg = QColor(200, 30, 0, 255);
//    int stap_gr[8] = {255, 120, 140, 200, 230, 180, 200, 240};
    int stap_gr[8] = {160, 220, 220, 220, 220, 220, 255};
    for(int i = 0; i < 7; i ++)
    {
        cg.setAlpha(stap_gr[i]);
        gradient.setColorAt((double)i / 6.0, cg);
    }
    painter.setPen(QPen(cg));
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.end();

    RePicGraph();
    OnReSizeTableSostav();
    OnReSizeTableVan();
    OnReSizeBDImage();
    OnReSizeCheckSide();
    OnReSizeButtons();
}
