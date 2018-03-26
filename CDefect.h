//---------------------------------------------------------------------------
#ifndef CDefectH
#define CDefectH

#include "CBuffer.h"
#include "CStackDual.h".h"
#include "CStackFourth.h"
#include "cparam.h"
#include <math.h>

//---------------------------------------------------------------------------
class CDefect
{
        CBuffer<SElement12<short> > data_fft;
        CBuffer<SElement12<short> > data_real;
        SElement12<SElementDual<short> > size_defect;
        SElement12<SElementDual<short> > porog_defect;

        void Run(const int &frequency, SElementDual<int> &fr_defect,
                 const double &speed, const int &tare);
  public:
        CDefect(){}
        CDefect(CFileBin &f);
        CDefect(const CBuffer<SElement12<short> > &dr, const CBuffer<SElement12<short> > &df,
                const double &speed, const int &tare);
        CDefect& operator=(const CDefect &s);

        void SaveToFile(CFileBin &f);

        void Paint(const unsigned &n, const bool &direction, const unsigned &width, QPointF* ptr_data1, QPointF* ptr_data2);
        int GetDefectProcent(const int &nd = 12) const;
        unsigned GetKolDefect(const unsigned &nd = 12) const;
        unsigned GetPorogDefect(const unsigned &nd) const {return (nd < 12)? porog_defect[nd][0] : 0;}
        unsigned NewGetDefectProcent(const int &nd) const;
        unsigned GetDefectSize(const unsigned &nd) const {return (nd < 12)? size_defect[nd][0] : 0;}
};
//---------------------------------------------------------------------------
#endif
