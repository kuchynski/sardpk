//---------------------------------------------------------------------------
#pragma hdrstop
#include "CDefect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
CDefect& CDefect::operator=(const CDefect &s)
{
  data_fft = s.data_fft;
  data_real = s.data_real;
  size_defect = s.size_defect;
  porog_defect = s.porog_defect;
  return *this;
}
//---------------------------------------------------------------------------
CDefect::CDefect(const CBuffer<SElement12<short> > &dr, const CBuffer<SElement12<short> > &df,
                 const double &speed, const int &tare):
     data_real(dr), data_fft(df)
{
    SElementDual<int> fr_defect(3, 15);
    int frequency = 480;
    Run(frequency / 2, fr_defect, speed, tare);
}
//---------------------------------------------------------------------------
CDefect::CDefect(CFileBin &f)
{
  data_real.LoadFromFile(f);
  data_fft.LoadFromFile(f);
  if(!f.Read(&size_defect, sizeof(size_defect), 1)) throw;
  if(!f.Read(&porog_defect, sizeof(porog_defect), 1)) throw;
}
//---------------------------------------------------------------------------
void CDefect::SaveToFile(CFileBin &f)
{
    data_real.SaveToFile(f);
    data_fft.SaveToFile(f);
    if(!f.Write(&size_defect, sizeof(size_defect), 1)) throw 0;
    if(!f.Write(&porog_defect, sizeof(porog_defect), 1)) throw 0;
}
//---------------------------------------------------------------------------
void CDefect::Run(const int &frequency, SElementDual<int> &fr_defect,
                  const double &speed, const int &tare)
{
  const double speed_kmch = speed * 3.6;
  SElementDual<int> kdst(1, 11);// = OptionsForm->GetCoofSpeedTare();
  SElementDual<double> koof_defect_speed_tare((double)kdst[0] / 10.0, (double)kdst[1] / 10.0);
  const double ks = (1.0 - koof_defect_speed_tare[0]) / (30.0 - 0.0);
  const double coof_speed = ks * (speed_kmch - 0.0) + koof_defect_speed_tare[0];
  const double kt = (1.0 - koof_defect_speed_tare[1]) / (20.0 - 5.0);
  const double coof_tare = kt * ((double)tare - 5.0) + koof_defect_speed_tare[1];
  double coof = (coof_tare + coof_speed) / 2;
  //double coof = coof_tare * coof_speed;
  if(coof < 0.4)
    coof = 0.4;

//  int begin = fr_defect[0] * data_fft.GetSize() / frequency;
//  int end = fr_defect[1] * data_fft.GetSize() / frequency;
  int begin = (data_fft.GetSize() * fr_defect[0]) / 64;
  int end = (data_fft.GetSize() * fr_defect[1]) / 64;
  SElement12<unsigned> p_defect = param.GetKoofA();// OptionsForm->GetPorogDefect();
    for(unsigned i = 0; i < 12; i ++)
    {
        if(param.GetonoffPb(i))
        {
            unsigned sum = 0;
            for(int j = begin + 1; j < end; j++)
            {
                sum += data_fft[j][i];
                data_fft[j][i] = -data_fft[j][i];
            }
            const double koof_shveler = static_cast<double>(21) / 10.0;
            double max = 0;
            double min = 0;
            double size_na2 = static_cast<double>(data_real.GetSize()) / 2.0;
            double k = (koof_shveler - 1) / size_na2;
            for(int j = data_real.GetSize()-1; j; j--)
            {
                double koof = 1.0 + k * fabs(size_na2 - static_cast<double>(j));
                double d_ch = static_cast<double>(data_real[j][i]) * koof;
                if(max < d_ch)
                    max = d_ch;
                if(min > d_ch)
                    min = d_ch;
            }
            porog_defect[i][0] = static_cast<short>(static_cast<double>(p_defect[i]) * coof);
            porog_defect[i][1] = static_cast<short>(static_cast<double>(p_defect[i]) * coof);

            short minmax = (max > -min)? max : -min;
            size_defect[i][0] = minmax / 10;
            size_defect[i][1] = sum / 20;
        }
        else
        {
            porog_defect[i][0] = 1;
            porog_defect[i][1] = 1;
            size_defect[i][0] = 0;
            size_defect[i][1] = 0;
        }
    }
}
//---------------------------------------------------------------------------
int CDefect::GetDefectProcent(const int &nd) const
{
  double max;                        // nd = 12 - все датчики
  if(nd < 0 || nd > 11)              // nd = 13 - первая сторона
  {                                  // nd = 14 - вторая сторона
    max = -1;
    const int beg = (nd == 12 || nd == 13)? 0 : 6;
    const int end = (nd == 12 || nd == 14)? 11 : 5;
    for(int i = beg; i <= end; i ++)
    {
    //  const int proc2 = (int)((double)(size_defect[i][0] - porog_defect[i][0])
   //                           * 100.0 / 10.0 / (double)porog_defect[i][0]);
      const double proc = ((double)(size_defect[i][0] - porog_defect[i][0])
                              * 100.0 / 10.0 / (double)porog_defect[i][0]);
      if((proc > max) && (proc >= 0))
        max = proc;
    }
    max ++;
  }
  else
    max = ((double)(size_defect[nd][0] - porog_defect[nd][0])
                    * 100.0 / 10.0 / (double)porog_defect[nd][0]) + 1.0;
  if(max < 0.0)
    max = 0.0;
  return (max > 99.0)? 99 : (int)max;
}
//---------------------------------------------------------------------------
unsigned CDefect::GetKolDefect(const unsigned &nd) const
{
    unsigned ret = 0;// nd = 12 - все датчики
    if(nd > 14)      // nd = 13 - первая сторона
        return 0;    // nd = 14 - вторая сторона
    const unsigned beg = (nd == 12 || nd == 13)? 0 : (nd == 14)? 6 : nd;
    const unsigned end = (nd == 12 || nd == 14)? 11 : (nd == 13)? 5 : nd;
    for(unsigned i = beg; i <= end; i ++)
        if(size_defect[i][0] >= porog_defect[i][0])
            ret ++;
    return ret;
}
//---------------------------------------------------------------------------
unsigned CDefect::NewGetDefectProcent(const int &nd) const
{
  double max;                        // nd = 12 - все датчики
  if(nd < 0 || nd > 11)              // nd = 13 - первая сторона
  {                                  // nd = 14 - вторая сторона
    max = -1;
    const int beg = (nd == 12 || nd == 13)? 0 : 6;
    const int end = (nd == 12 || nd == 14)? 11 : 5;
    for(int i = beg; i <= end; i ++)
    {
    //  const int proc2 = (int)((double)(size_defect[i][0] - porog_defect[i][0])
   //                           * 100.0 / 10.0 / (double)porog_defect[i][0]);
      const double proc = ((double)(size_defect[i][0] - porog_defect[i][0])
                              * 100.0 / 10.0 / (double)porog_defect[i][0]);
      if((proc > max) && (proc >= 0))
        max = proc;
    }
    max ++;
  }
  else
    max = ((double)(size_defect[nd][0] - porog_defect[nd][0])
                    * 100.0 / 10.0 / (double)porog_defect[nd][0]) + 1.0;
  if(max < 0.0)
    max = 0.0;
  return (max > 99.0)? 99 : (int)max;
}
//---------------------------------------------------------------------------
void CDefect::Paint(const unsigned &n, const bool &direction, const unsigned &width, QPointF* ptr_data1, QPointF* ptr_data2)
{
  double pr1 = static_cast<double>(data_real.GetSize()) / static_cast<double>(width);
  double pr2 = static_cast<double>(data_fft.GetSize()) / static_cast<double>(width);
  for(int x = 1; x < width-1; x ++)
  {
    int c1 = (static_cast<double>(x) * pr1);
    int c2 = (static_cast<double>(x) * pr2);
    int d2 = data_fft[c2][n];
    ptr_data1[x] = QPointF(x, abs(d2));
    ptr_data2[(!direction)? (width-x-1) : x] = QPointF(0, data_real[c1][n]/2);
  }
/*  int k = 1;
  double step = static_cast<double>(bitmap->Width) / static_cast<double>(OptionsForm->GetFrequency()/2) * 25;
  for(int x = step; x < bitmap->Width; x += step, k ++)
  {
    int s = (((k/4)*4) == k)? 20 : 10;
    bitmap->Canvas->MoveTo(x, bitmap->Height - 1);
    bitmap->Canvas->LineTo(x, bitmap->Height - s);
  }*/
}
//---------------------------------------------------------------------------
