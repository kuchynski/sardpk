#ifndef CBD_H
#define CBD_H

//#include<QIODevice>
#include<QDate>
#include<QDir>
#include<QFile>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QWidget>
#include "CSostav.h"
#include "cbuttons.h"
#include "ctable.h"
#include "cbdimage.h"

class CBD: public QWidget, public CStack <CSostav>
{
    Q_OBJECT
private:
    QCalendarWidget calendar;
    CButtons buttons;
    CTable table_sostavs;
    CTable table_vans;
    CBDImage bd_image;
    CButtonsCheck check_side;

    QString file_path, tek_file_path;
    QDate date;
    QDir dir;
    int activ_sostav;
    QImage image_my;
    QPainter painter;
    QWidget *w1, *w2, *w3;
    QPrinter printer;
    QProcess proc_pdf;

    bool YesOrNoYearMonDay(const QDate &_date);
    void Clear();
    bool SetDir(const QDate &_date);
    void PaintGridSostav();
public:
    explicit CBD(QWidget *parent = 0);

    void RePicGraph();
    bool AddSostav(CSostav &sostav);
    CSostav GetActivSostav();
    void PrintSostav();
    void PrintDiallog();
    void PrintPdfSostav();
    void CloseSostav();

    void resizeEvent(QResizeEvent *r);
    void paintEvent(QPaintEvent *p);
public slots:
    void TableSostavClicked(unsigned row, const bool &deactiv = true);
    void TableVanClicked(unsigned row);
    void OnButtonsClick(unsigned n, bool s);
    void CalendarChanged();
    void OnCheckedSide(unsigned n, bool state);
    void OnReSizeTableSostav();
    void OnReSizeTableVan();
    void OnReSizeBDImage();
    void OnReSizeCheckSide();
    void OnReSizeButtons();
    void DefectForButtonSide(bool s1, bool s2);
signals:
};

#endif // CBD_H
