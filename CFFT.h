//---------------------------------------------------------------------------
#ifndef CFFTH
#define CFFTH

//#include <StdCtrls.hpp>
//#include <ExtCtrls.hpp>
#include <stdio.h>
#include <math.h>
#include "CBuffer.h"
#include "CStackDual.h"
#include "CStackFourth.h"
//#include "CFilter.h"
#include "cparam.h"
using namespace std;
//---------------------------------------------------------------------------
class CFFT
{
    CBuffer<SElement12<double> > x, y;
    CBuffer<SElement12<double> > x1, y1;
    SElement12<int> begin;
    int size, real_size;
    int m;

    void RunForward(CBuffer<SElement12<int> > &in);
    void ClearFr(int fr1, int fr2);
    void ClearFr2(int b, int e);
    void RunBack(CBuffer<SElement12<short> > &out);
    void RunBackFr(CBuffer<SElement12<short> > &out);
  public:
    CFFT();
    void RunKoleso(CBuffer<SElement12<int> > &in, CBuffer<SElement12<short> > &out_fft, CBuffer<SElement12<short> > &out_real,
                   SElement12<int> &b, const int &r_size);
};
//---------------------------------------------------------------------------
#endif
