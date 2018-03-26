//---------------------------------------------------------------------------
#ifndef SAxelH
#define SAxelH
#include "CStackFourth.h"
//---------------------------------------------------------------------------
struct SAxel
{
        SElement12<int> uk_data_defect;
        unsigned distance;
        SElement12<unsigned> coordinate;
        double speed;
        SElement12<int> progib;
        SElement12<int> tare;
        SElement12<int> horda;// horda[0] - координата начала габарита
                              // horda[1] - радиус колеса1
                              // horda[2] - обод колеса1
                              // horda[3] - радиус колеса2
                              // horda[4] - обод колеса2
        SElement12<int> defect;
        int gabarit;
        double om;
        SAxel() {}
        SAxel(const SElement12<int> &ud, double o, const unsigned &d, const SElement12<unsigned> &c, double &s,
          const SElement12<int> &p, const SElement12<int> &t,
          const SElement12<int> &h, const SElement12<int> &de, int g);
        SAxel(const SAxel &a);
        SAxel& operator=(const SAxel& a);
};
//---------------------------------------------------------------------------
#endif
 
