//---------------------------------------------------------------------------
#include "SAxel.h"
//---------------------------------------------------------------------------
SAxel::SAxel(const SElement12<int> &ud, double o, const unsigned &d, const SElement12<unsigned> &c, double &s,
          const SElement12<int> &p, const SElement12<int> &t,
          const SElement12<int> &h, const SElement12<int> &de, int g):
  uk_data_defect(ud), distance(d), coordinate(c), speed(s), progib(p), tare(t),
  horda(h), defect(de), om(o), gabarit(g)
{}
//---------------------------------------------------------------------------
SAxel::SAxel(const SAxel &a)
{
    *this = a;
}
//---------------------------------------------------------------------------
SAxel& SAxel::operator=(const SAxel& a)
{
    uk_data_defect = a.uk_data_defect;
    distance = a.distance;
    coordinate = a.coordinate;
    speed = a.speed;
    progib = a.progib;
    tare = a.tare;
    horda = a.horda;
    defect = a.defect;
    om = a.om;
    gabarit = a.gabarit;
    return *this;
}
//---------------------------------------------------------------------------
