//---------------------------------------------------------------------------
#pragma hdrstop

#include "CFFT.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
CFFT::CFFT(): m(10)
{
  size = 1 << m;
  x = CBuffer<SElement12<double> >(size);
  y = CBuffer<SElement12<double> >(size);
  x1 = CBuffer<SElement12<double> >(size);
  y1 = CBuffer<SElement12<double> >(size);
}
//---------------------------------------------------------------------------
void CFFT::RunKoleso(CBuffer<SElement12<int> > &in, CBuffer<SElement12<short> > &out_fft,
                     CBuffer<SElement12<short> > &out_real,
                     SElement12<int> &b, const int &r_size)
{
  real_size = r_size;
  int new_m = 6;
  for(; new_m < 16; new_m ++)
    if((1 << new_m) > real_size)
      break;
  m = new_m;
  size = 1 << m;
  if(x.GetSize() != size)
  {
    x = CBuffer<SElement12<double> >(size);
    y = CBuffer<SElement12<double> >(size);
    x1 = CBuffer<SElement12<double> >(size);
    y1 = CBuffer<SElement12<double> >(size);
  }
  begin = b;
  if(!real_size)
    return;
  RunForward(in);
  RunBackFr(out_fft);

  SElementDual<int> fr_defect(3, 15);// = OptionsForm->GetFrDefect();
  ClearFr2((size/2) * fr_defect[0] / 64, (size/2) * fr_defect[1] / 64);
  RunBack(out_real);
}
//---------------------------------------------------------------------------
void CFFT::RunForward(CBuffer<SElement12<int> > &in)
{
  for(int i = 0; i < size; i ++)
  {
    y[i] = 0;
    if(i < real_size)
      for(int n = 0; n < 12; n ++)
        x[i][n] = static_cast<double>(in[i+begin[n]][n]);
    else
      x[i] = 0;
  }
  for(int b = 0; b < m; b ++)
  {
    double e = 1 << (m-b);
    double f = e / 2;
    double u = 1;
    double v = 0;
    double z = 3.14159 / f;
    double c = cos(z);
    double s = -sin(z);
    for(int j = 0; j < f; j ++)
    {
      for(int i = j; i < size; i += e)
      {
        int g = i + f;
        for(int n = 0; n < 12; n ++)
        {
          double d1 = x[i][n] + x[g][n];
          double d2 = y[i][n] + y[g][n];
          double r = x[i][n] - x[g][n];
          double t = y[i][n] - y[g][n];
          x[i][n] = d1;
          y[i][n] = d2;
          x[g][n] = r*u - t*v;
          y[g][n] = t*u + r*v;
        }
      }
      double w = u*c - v*s;
      v = (v*c + u*s);
      u = w;
    }
  }
  for(int i = 1; i < size - 1; i++)
  {
    int j = 0;
    for(int k = 0; k < m; k++)
      if(i & (0x01 << k))
        j += (size >> k+1);
    if((i >= size) || (j >= size) || (j >= i))
      continue;
    for(int n = 0; n < 12; n ++)
    {
      double d = x[j][n]; x[j][n] = x[i][n]; x[i][n] = d;
      d = y[j][n]; y[j][n] = y[i][n]; y[i][n] = d;
    }
  }
  for(int i = 0; i < size; i ++)
  {
    x[i] /= real_size;
    y[i] /= real_size;
    x1[i] = x[i];
    y1[i] = y[i];
  }
}
//---------------------------------------------------------------------------
void CFFT::ClearFr2(int b, int e)
{
  if(b <= 0)
    b = 0;
  if(e >= size/2)
    e = size/2 - 1;
  for(int n = 0; n < 12; n ++)
  {
    for(int i = 0; i <= b; i ++)
    {
      x1[i][n] = 0;
      y1[i][n] = 0;
      x1[size - i-1][n] = 0;
      y1[size - i-1][n] = 0;
    }
    for(int i = e; i < size/2; i ++)
    {
      x1[i][n] = 0;
      y1[i][n] = 0;
      x1[size - i-1][n] = 0;
      y1[size - i-1][n] = 0;
    }
  }
}
//---------------------------------------------------------------------------
void CFFT::ClearFr(int fr1, int fr2)
{
  int frequency = 480;//OptionsForm->GetFrequency();
  if((fr1 > fr2) || (fr1 > frequency/2) || (fr2 < 0))
    return;
  if(fr1 <= 0)
    fr1 = 0;
  else
    fr1 = fr1 * size / frequency;
  if(fr2 >= frequency/2)
    fr2 = size/2 - 1;
  else
    fr2 = fr2 * size / frequency;
  for(int n = 0; n < 12; n ++)
    for(int i = fr1; i <= fr2; i ++)
    {
      x1[i][n] = 0;
      y1[i][n] = 0;
      x1[size - i-1][n] = 0;
      y1[size - i-1][n] = 0;
    }
}
//---------------------------------------------------------------------------
void CFFT::RunBack(CBuffer<SElement12<short> > &out)
{
  for(int b = 0; b < m; b ++)
  {
    double e = 1 << (m-b);
    double f = e / 2;
    double u = 1;
    double v = 0;
    double z = 3.14159 / f;
    double c = cos(z);
    double s = -sin(z);
    for(int j = 0; j < f; j ++)
    {
      for(int i = j; i < size; i += e)
      {
        int g = i + f;
        for(int n = 0; n < 12; n ++)
        {
          double d1 = x1[i][n] + x1[g][n];
          double d2 = y1[i][n] + y1[g][n];
          double r = x1[i][n] - x1[g][n];
          double t = y1[i][n] - y1[g][n];
          x1[i][n] = d1;
          y1[i][n] = d2;
          x1[g][n] = r*u + t*v;
          y1[g][n] = t*u - r*v;
        }
      }
      double w = u*c - v*s;
      v = (v*c + u*s);
      u = w;
    }
  }
  for(int i = 1; i < size - 1; i++)
  {
    int j = 0;
    for(int k = 0; k < m; k++)
      if(i & (0x01 << k))
        j += (size >> k+1);
    if((i >= size) || (j >= size) || (j >= i))
      continue;
    for(int n = 0; n < 12; n ++)
    {
      double d = x1[j][n]; x1[j][n] = x1[i][n]; x1[i][n] = d;
      d = y1[j][n]; y1[j][n] = y1[i][n]; y1[i][n] = d;
    }
  }
  for(int i = 0; i < real_size; i ++)
    for(int n = 0; n < 12; n ++)
      out[i][n] = static_cast<short>(x1[i][n]/2);
}
//---------------------------------------------------------------------------
void CFFT::RunBackFr(CBuffer<SElement12<short> > &out)
{
  double z = static_cast<double>(size) / static_cast<double>(real_size);
  CBuffer<SElement12<short> > tmp(size/2);
  for(int i = 0; i < size/2; i ++)
    for(int n = 0; n < 12; n ++)
    {
      double xx = x1[i][n];
      double yy = y1[i][n];
      double s = sqrt(xx*xx + yy*yy);
      tmp[i][n] = static_cast<short>(s);
    }
  double compress = static_cast<double>(out.GetSize()) / static_cast<double>(tmp.GetSize());
  for(int i = 0; i < out.GetSize(); i ++)
    out[i] = 0;
  for(int i = 0; i < size/2; i ++)
    out[static_cast<int>(static_cast<double>(i)*compress)] += tmp[i];
}
//---------------------------------------------------------------------------
