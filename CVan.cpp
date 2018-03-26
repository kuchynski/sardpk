//---------------------------------------------------------------------------
#include <stdio.h>
#include "CVan.h"

//---------------------------------------------
CVan::CVan()
{
  Clear();
}
//---------------------------------------------
CVan::CVan(CFileBin &f)
{
    if(!f.Read(&begin_axel, sizeof(begin_axel), 1)) throw;
    if(!f.Read(&size, sizeof(size), 1)) throw;
    type = CString(f);
    if(!f.Read(&lokomotiv, sizeof(lokomotiv), 1)) throw;
}
//---------------------------------------------
CVan::CVan(const CVan &v)
{
  *this = v;
}
//-------------------------------------------------------------
CVan::CVan(const int &b): begin_axel(b), size(1)
{}
//---------------------------------------------
CVan& CVan::operator=(const CVan& v)
{
  begin_axel = v.begin_axel;
  size = v.size;
  ps = v.ps;
  type = v.type;
  lokomotiv = v.lokomotiv;
  return *this;
}
//-------------------------------------------------------------
void CVan::SaveToFile(CFileBin &f)
{
    if(!f.Write(&begin_axel, sizeof(begin_axel), 1)) throw 0;
    if(!f.Write(&size, sizeof(size), 1)) throw 0;
    type.SaveToFile(f);
    if(!f.Write(&lokomotiv, sizeof(lokomotiv), 1)) throw 0;
}
//-------------------------------------------------------------
void CVan::Clear()
{
  begin_axel = 0;
  size = 0;
  type = "";
  lokomotiv = 0;
}
//-------------------------------------------------------------
bool CVan::IsMy(const unsigned &n)const
{
  return ((n >= begin_axel) && (n <= (begin_axel + size - 1)));
}
//---------------------------------------------
double CVan::GetSpeedDouble(const CStack <SAxel> &axels) const
{
  double speed = 0;
  for(unsigned i = 0; i < size; i ++)
    speed += axels[begin_axel+i].speed;
  speed = (size)? speed / size : 0;
  //double speed_mc = (speed)? (1728/(2*(double)speed)) : 0;
  double speed_kmch = speed * 3.6;
  return speed_kmch;
}
//---------------------------------------------
QString CVan::GetSpeed(const CStack <SAxel> &axels) const
{
  double speed = GetSpeedDouble(axels);
  if(speed)
      return QString::number(speed, 'f', 1);// GetFloatToStr(speed, 1);
  return "0";
}
//---------------------------------------------
unsigned CVan::GetCoordinate(const CStack <SAxel> &axels, const unsigned &n, unsigned na)const
{
  if(na > (size-1)) na = size - 1;
  return axels[begin_axel+na].coordinate[n];
}
unsigned CVan::GetKolDefect(const CStack <CDefect> &defects, const unsigned naxel, const unsigned ndat)const
{// naxel = 100(всё)
 // ndat  = 12(всё), 13(1 сторона), 14(2 сторона)
    if(ndat > 14)
        return 0;
    if((naxel > size) && (naxel != 100))
        return 0;
    int axel_begin = (naxel == 100)? begin_axel : begin_axel + naxel;
    int axel_end = (naxel == 100)? begin_axel + size : axel_begin + 1;
    unsigned ret = 0;
    for(unsigned i = axel_begin; i < axel_end; i ++)
        if(defects[i].GetKolDefect(ndat))
            ret ++;
    return ret;
}
unsigned CVan::GetTare(const CStack <SAxel> &axels, const unsigned ndat)const
{
    if(ndat > 14)
        return 0;
    const unsigned beg = (ndat == 12 || ndat == 13)? 0 : (ndat == 14)? 6 : ndat;
    const unsigned end = (ndat == 12 || ndat == 14)? 11 : (ndat == 13)? 5 : ndat;
    unsigned ret = 0;
    for(unsigned i = begin_axel; i < begin_axel + size; i ++)
    {
        unsigned tare_axel = 0;
        unsigned kol = 0;
        for(unsigned j = beg; j <= end; j ++)
        {
            int t = axels[i].tare[j];
            if(t)
            {
                tare_axel += t;
                kol ++;
            }
        }
        if(kol)
          ret += (tare_axel / kol);
    }
    return ret / 1000;
}
CString CVan::GetDefectFile(const CStack <CDefect> &defects) const
{
  CString ret1 = "";
  CString ret2 = "";
  for(int i = 0; i < size; i ++)
  {
    int d1 = defects[begin_axel+i].NewGetDefectProcent(13);// первая сторона
    int d2 = defects[begin_axel+i].NewGetDefectProcent(14);// вторая сторона
    ret1 += ((d1)? (CString("+") + CString(d1, 2)) : CString("-00"));
    ret2 += ((d2)? (CString("+") + CString(d2, 2)) : CString("-00"));
  }
  return ret1 + " " + ret2;
}
