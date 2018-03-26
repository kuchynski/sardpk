//---------------------------------------------------------------------------
#ifndef CFilterH
#define CFilterH

#include "CStack.h"
//---------------------------------------------------------------------------
class CFilter   {
        CStack <double> a, b;
        CStack <double> x, y;
        static int str;
        void Clear();
  public:
        CFilter(){
        };
        CFilter(const CStack <double> &new_a, const CStack <double> &new_b);
        CFilter(const CFilter &f);
        ~CFilter();
        void SetAB(const CStack <double> &new_a, const CStack <double> &new_b);
        void MullA(const double& mul_a);
        void MullB(const double& mul_b);
        void SetNumber(const double& n);
        double operator<<(const double& new_x);
        int operator<<(const int& new_x);
        CFilter& operator=(const CFilter& f);
};
//---------------------------------------------------------------------------
#endif
