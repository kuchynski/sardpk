//---------------------------------------------
#if !defined(__datetime)
#define __datetime

#include "CString.h"
#include <QDate>
#include <QTime>
#include <dos.h>
#include "cfilebin.h"
//---------------------------------------------
class CDateTime
{
    QDate date;
    QTime time;
  public:
    CDateTime();
    CDateTime(const CDateTime &);
    CDateTime(CFileBin &f);

    QString GetTime()const;
    QString GetTime2()const;
    QString GetDate()const;
    void SetDate(const QDate &d) {date = d;}
    void SetTime(const QTime &t) {time = t;}
    int GetYear()const {return date.year();}
    int GetMon()const {return date.month();}
    int GetDay()const {return date.day();}
    QDate GetQDate()const {return date;}
    void SaveToFile(CFileBin &f)const;
};
//---------------------------------------------
#endif
