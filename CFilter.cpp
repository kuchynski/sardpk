//---------------------------------------------------------------------------
#include "CFilter.h"
//---------------------------------------------------------------------------
CFilter::CFilter(const CStack <double> &new_a, const CStack <double> &new_b)
{
  SetAB(new_a, new_b);
}
//---------------------------------------------------------------------------
CFilter::CFilter(const CFilter &f)
{
  *this = f;
}
//---------------------------------------------------------------------------
CFilter& CFilter::operator=(const CFilter& f)
{
  a = f.a;
  b = f.b;
  x = f.x;
  y = f.y;
  return *this;
}
//---------------------------------------------------------------------------
CFilter::~CFilter()
{
  Clear();
}
//---------------------------------------------------------------------------
void CFilter::Clear()
{
  a.Clear();
  b.Clear();
  x.Clear();
  y.Clear();
}
//---------------------------------------------------------------------------
void CFilter::SetAB(const CStack <double> &new_a, const CStack <double> &new_b)
{
  double q;
  a = new_a;
  b = new_b;
  while(a.GetSize() < b.GetSize()) { b >> q;}
  while(b.GetSize() < a.GetSize()) { a >> q;}
  SetNumber(0);
}
//---------------------------------------------------------------------------
void CFilter::MullA(const double& mul_a)
{
  for(unsigned i = 0; i < a.GetSize(); i ++)
    a[i] *= mul_a;
}
//---------------------------------------------------------------------------
void CFilter::MullB(const double& mul_b)
{
  for(unsigned i = 0; i < b.GetSize(); i ++)
    b[i] = b[i] * mul_b;
  b[0] = b[0];  
}
//---------------------------------------------------------------------------
void CFilter::SetNumber(const double& n)
{
  x.Clear();
  while(x.GetSize() < a.GetSize()) {x << n;}
  y.Clear();
  while(y.GetSize() < a.GetSize()) {y << n;}
}
//---------------------------------------------------------------------------
double CFilter::operator<<(const double& new_x)
{
  int size = a.GetSize() - 1;
  x[size] = new_x;
  y[size] = b[0] * x[size];
  for(int i = 0; i < size; i ++)
    y[size] += (b[i+1] * x[size-i-1] - a[i+1] * y[size-i-1]);
  for(int i = 0; i < size; i ++)
  { x[i] = x[i+1]; y[i] = y[i+1]; }
  return y[size];
}
//---------------------------------------------------------------------------
int CFilter::operator<<(const int& new_x)
{
  return static_cast<int> (*this << static_cast<double>(new_x));
}
//---------------------------------------------------------------------------

