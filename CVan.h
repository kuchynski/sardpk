//---------------------------------------------------------------------------
#ifndef CVanH
#define CVanH

#include <stdlib.h>
#include <stdio.h>
#include "SAxel.h"
#include "CString.h"
#include "CStackDual.h"
#include "cfilebin.h"
#include "CDefect.h"
//---------------------------------------------------------------------------
class CVan
{
    unsigned begin_axel;
    unsigned size;
    SElementDual<QString> ps;
    CString type;
    int lokomotiv;

  //  int GetTareProgib(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat, const bool &tp) const;

  public:
    CVan();
    CVan(const CVan &v);
    CVan(const int &b);
    CVan(CFileBin &f);
    ~CVan(){}
    CVan& operator=(const CVan& v);

    void SaveToFile(CFileBin &f);
    void SetSizeAxel(const int &s) {size = s;}
    void Clear();
    void SetPS(const SElementDual<QString> &ps_new) {ps = ps_new;}
    QString GetPS(const int &n) {return ps[n];}

    unsigned GetBegin()const {return begin_axel;}
    unsigned GetSize()const {return size;}
    unsigned GetEnd()const {return begin_axel + size - 1;}
    bool IsMy(const unsigned &n)const;

    QString GetSpeed(const CStack <SAxel> &axels) const;
    unsigned GetCoordinate(const CStack <SAxel> &axels, const unsigned &n, unsigned na = 0)const;
    void SetType(CString str, const int &lok) {type = str; lokomotiv = lok;}
    int GetLokomotiv()const {return lokomotiv;}
    QString GetType() {return type.GetQString();}

    unsigned GetKolDefect(const CStack <CDefect> &defects, const unsigned naxel, const unsigned ndat = 12) const;
    unsigned GetTare(const CStack <SAxel> &axels, const unsigned ndat = 12) const;
    double GetSpeedDouble(const CStack <SAxel> &axels) const;
    CString GetDefectFile(const CStack <CDefect> &defects) const;
};
//---------------------------------------------------------------------------
#endif
