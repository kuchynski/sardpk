#ifndef CCOMMANDER_H
#define CCOMMANDER_H

#include <QWidget>
#include <QtGui>
#include "cbd.h"
#include "cmonitor.h"
#include "cinput.h"
#include "coptions.h"
#include "CAllWindows.h"
#include "cmanywindows.h"

class CCommander : public CAllWindows
{
public:
    explicit CCommander(QWidget *parent = 0);
private:
    CBD bd;
    CInput input;
    COptions options;

private slots:
    void OnNewSostav();
    void OnPressButton(unsigned n);
  //  void OnBeginTarirovka(CSostav &s);
  //  void OnEndTarirovka(CSostav &s, bool res);
    void OnNewKoof(SElement12<unsigned> a, SElement12<unsigned> b, SElement12<int> c);
};

#endif // CCOMMANDER_H
