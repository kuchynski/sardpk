//---------------------------------------------------------------------------
#ifndef CString_H
#define CString_H
#include <QString>

//#include <stdio.h>
//#include <stdlib.h>
#include "cfilebin.h"
//---------------------------------------------------------------------------
class CString
{
  private:
    char *str;
  public:
    CString(CFileBin &f);
    CString(const char *new_str = "");
    CString(int n);
    CString(const CString &s);
    CString(int n, int size);
    CString(const QString &s);
    ~CString();
    CString operator=(const CString &ks);
    bool operator==(const CString &ks)const  {return strcmp(str, ks.str) == 0;}
    bool operator!=(const CString &ks)const  {return !(*this == ks);}
    bool operator>(const CString &ks)const  {return strcmp(str, ks.str) > 0;}
    bool operator<(const CString &ks)const  {return strcmp(str, ks.str) < 0;}
    bool operator<=(const CString &ks)const  {return !(*this > ks);}
    bool operator>=(const CString &ks)const  {return !(*this < ks);}
    CString operator+(const CString &ks) const;
    CString operator+=(const CString &ks);
    void SaveToFile(CFileBin &f);
    QString GetQString() const;
    int GetInt(void);
    char* GetPtrChar(){return str;}
};
//CString operator+(const CString &ks1, const CString &ks2);
//---------------------------------------------------------------------------
#endif
 
