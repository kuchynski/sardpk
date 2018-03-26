//---------------------------------------------
#include "CDateTime.h"

//---------------------------------------------
CDateTime::CDateTime(): date(QDate::currentDate()), time(QTime::currentTime())
{}
//---------------------------------------------
CDateTime::CDateTime(const CDateTime &dt): date(dt.date), time(dt.time)
{}
//---------------------------------------------
CDateTime::CDateTime(CFileBin &f)
{
    unsigned char min = 0;
    unsigned char hour = 0;
    int day = 1;
    int mon = 1;
    int year = 2012;
    f.Read(&min, sizeof(min), 1);
    f.Read(&hour, sizeof(hour), 1);
    f.Read(&day, sizeof(day), 1);
    f.Read(&mon, sizeof(mon), 1);
    f.Read(&year, sizeof(year), 1);
    date.setDate(year, mon, day);
    time.setHMS(hour, min, 0);
}
//---------------------------------------------
void CDateTime::SaveToFile(CFileBin &f)const
{
    unsigned char min = time.minute();
    unsigned char hour = time.hour();
    int day = date.day();
    int mon = date.month();
    int year = date.year();
    f.Write(&min, sizeof(min), 1);
    f.Write(&hour, sizeof(hour), 1);
    f.Write(&day, sizeof(day), 1);
    f.Write(&mon, sizeof(mon), 1);
    f.Write(&year, sizeof(year), 1);
}
//---------------------------------------------
QString CDateTime::GetTime()const
{
    return QString::number(time.hour()) + ":" + CString(static_cast<int>(time.minute()), 2).GetQString();
}
QString CDateTime::GetTime2()const
{
    return CString(static_cast<int>(time.hour()), 2).GetQString() + "_" + CString(static_cast<int>(time.minute()), 2).GetQString();
}
QString CDateTime::GetDate()const
{
    return CString(date.day(), 2).GetQString() + "." + CString(static_cast<int>(date.month()), 2).GetQString() +
         "." + CString(static_cast<int>(date.year()), 4).GetQString() + QString::fromLocal8Bit(" ã.");
}
