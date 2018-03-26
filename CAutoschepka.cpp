//---------------------------------------------------------------------------
#include "CAutoschepka.h"

//extern TOptionsForm *OptionsForm;
#define contol_datchik 1
//-------------------------------------------------------------
CAutoschepka::CAutoschepka()
{
  Clear();
}
//---------------------------------------------
CAutoschepka::CAutoschepka(CFileBin &f)
{
  if(!f.Read(&begin_axel, sizeof(begin_axel), 1)) throw;
  if(!f.Read(&size, sizeof(size), 1)) throw;
}
//-------------------------------------------------------------
CAutoschepka::CAutoschepka(const CAutoschepka &a)
{
  *this = a;
}
//-------------------------------------------------------------
CAutoschepka::CAutoschepka(const int &b): begin_axel(b), size(1)
{}
//-------------------------------------------------------------
CAutoschepka& CAutoschepka::operator=(const CAutoschepka &a)
{
  begin_axel = a.begin_axel;
  size = a.size;
  return *this;
}
//-------------------------------------------------------------
void CAutoschepka::Clear()
{
  begin_axel = 0;
  size = 0;
}
//-------------------------------------------------------------
void CAutoschepka::SaveToFile(CFileBin &f)
{
  if(!f.Write(&begin_axel, sizeof(begin_axel), 1)) throw 0;
  if(!f.Write(&size, sizeof(size), 1)) throw 0;
}
//-------------------------------------------------------------
bool CAutoschepka::IsMy(const unsigned &n)const
{
  return ((n >= begin_axel) && (n <= (begin_axel + size - 1)));
}
//-------------------------------------------------------------
void CAutoschepka::RunClassic(const ETypeRun &type_run, CStack <SAxel> &axels, const SElementFourth<double> &koof_k, const SElementFourth<double> &koof_u,
             const SElementFourth<int> &koof_sm, const bool &direction)
{
  SElementDual<double> sh (66.5, 66.5);

  for(unsigned i = 0; i < size; i ++)//заполн€ем таблицу рассто€ний между колЄсами
    for(unsigned j = 0; j < size; j ++)
      rast[i][j] = axels[j+begin_axel].distance - axels[i+begin_axel].distance;

  for(unsigned pb = 0; pb < 4; pb++)
  {
    if(!param.GetonoffPb(pb))
    {
      for(unsigned i = 0; i < size; i ++)
        axels[i+begin_axel].tare[pb] = -65000;
      continue;
    }

    for(unsigned chance = 0; chance < size; chance ++)//высчитываем nu
      for(unsigned axel = 0; axel < size; axel ++)
        if(type_run == ETRTeor)
        {
          double kx = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]));
          double kxsh1 = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]) + sh[0]);
          double kxsh2 = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]) - sh[1]);
          double pr = exp(-kx) * (cos(kx) + sin(kx));
          double prsh1 = exp(-kxsh1) * (cos(kxsh1) + sin(kxsh1));
          double prsh2 = exp(-kxsh2) * (cos(kxsh2) + sin(kxsh2));
          nu[chance][axel] = pr - (prsh1 + prsh2) / 2;
        }
      //  else
        //  nu[chance][axel] = param.GetKoofAReal( OptionsForm->GetNewK(rast[chance][axel], pb);

    for(unsigned i = 0; i < size; i ++)//решаем систему уравнений
    {
      use_det[i] = -1;
      if(type_run == ETRTeor)
        nu[max_number_axel][i] = static_cast<double>(axels[i+begin_axel].progib[pb] - koof_sm[pb])
                                 * koof_u[pb] / koof_k[pb];
      else
        nu[max_number_axel][i] = static_cast<double>(axels[i+begin_axel].progib[pb] - koof_sm[pb])
                                 * koof_u[pb] * 100.0;
    }
 //   if(contol_datchik == pb)//«апись в "test_autoschepka.txt"
   //   WriteTestFile(pb, axels);

    ret_recurs = 0;
    Recurs(size, 0);
    det = ret_recurs;
    for(unsigned i = 0; i < size; i ++)
    {
      ret_recurs = 0;
      Recurs(i, 0);
      axels[i+begin_axel].tare[pb] = (det)? ret_recurs / det : 0;
    }
  }
}
//-------------------------------------------------------------
/*void CAutoschepka::RunClassicTeor(CStack <SAxel> &axels, const SElementFourth<double> &koof_k,
                       const SElementFourth<double> &koof_u, const SElementFourth<int> &koof_sm, const bool &direction)
{
  SElementDual<double> sh (66.5, 66.5);

  for(unsigned i = 0; i < size; i ++)//заполн€ем таблицу рассто€ний между колЄсами
    for(unsigned j = 0; j < size; j ++)
      rast[i][j] = axels[j+begin_axel].distance - axels[i+begin_axel].distance;

  for(unsigned pb = 0; pb < 4; pb++)
  {
    if(!OptionsForm->GetWorkPB(pb))
    {
      for(unsigned i = 0; i < size; i ++)
        axels[i+begin_axel].tare[pb] = -65000;
      continue;
    }

    for(unsigned chance = 0; chance < size; chance ++)//высчитываем nu
      for(unsigned axel = 0; axel < size; axel ++)
      {
        double kx = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]));
        double kxsh1 = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]) + sh[0]);
        double kxsh2 = koof_k[pb] * fabs(static_cast<double>(rast[chance][axel]) - sh[1]);
        double pr = exp(-kx) * (cos(kx) + sin(kx));
        double prsh1 = exp(-kxsh1) * (cos(kxsh1) + sin(kxsh1));
        double prsh2 = exp(-kxsh2) * (cos(kxsh2) + sin(kxsh2));
        nu[chance][axel] = pr - (prsh1 + prsh2) / 2;
      }

    for(unsigned i = 0; i < size; i ++)//решаем систему уравнений
    {
      use_det[i] = -1;
      nu[max_number_axel][i] = static_cast<double>(axels[i+begin_axel].progib[pb]
                               - koof_sm[pb])* koof_u[pb] / koof_k[pb];
    }
    if(contol_datchik == pb)//«апись в "test_autoschepka.txt"
      WriteTestFile(pb, axels);

    ret_recurs = 0;
    Recurs(size, 0);
    det = ret_recurs;
    for(unsigned i = 0; i < size; i ++)
    {
      ret_recurs = 0;
      Recurs(i, 0);
      axels[i+begin_axel].tare[pb] = (det)? ret_recurs / det : 0;
    }
  }
}
//-------------------------------------------------------------
void CAutoschepka::RunClassicPractic(CStack <SAxel> &axels, const SElementFourth<double> &koof_u, const SElementFourth<int> &koof_sm, const bool &direction)
{
  for(unsigned i = 0; i < size; i ++)//заполн€ем таблицу рассто€ний между колЄсами
    for(unsigned j = 0; j < size; j ++)
    {
      rast[i][j] = axels[j+begin_axel].distance - axels[i+begin_axel].distance;
      if(direction)
        rast[i][j] = -rast[i][j];
    }

  for(unsigned pb = 0; pb < 4; pb++)
  {
    if(!OptionsForm->GetWorkPB(pb))
    {
      for(unsigned i = 0; i < size; i ++)
        axels[i+begin_axel].tare[pb] = -65000;
      continue;
    }
    for(unsigned chance = 0; chance < size; chance ++)//высчитываем nu
      for(unsigned axel = 0; axel < size; axel ++)
        nu[chance][axel] = OptionsForm->GetNewK(rast[chance][axel], pb);

    for(unsigned i = 0; i < size; i ++)//решаем систему уравнений
    {
      use_det[i] = -1;
      nu[max_number_axel][i] = static_cast<double>(axels[i+begin_axel].progib[pb]- koof_sm[pb])* koof_u[pb]*1000.0;
    }
  //  if(contol_datchik == pb)//«апись в "test_autoschepka.txt"
//      WriteTestFile(pb, axels);
    ret_recurs = 0;
    Recurs(size, 0);
    det = ret_recurs;
    for(unsigned i = 0; i < size; i ++)
    {
      ret_recurs = 0;
      Recurs(i, 0);
      double qw = ret_recurs / det;
      axels[i+begin_axel].tare[pb] = (det)? ret_recurs / det : 0;
    }
  }  
}   */
//-------------------------------------------------------------
void CAutoschepka::Recurs(const unsigned n, const unsigned col)
{
  unsigned i, j;

  if(col == size)
  {
    int I = 1;
    for(i = 0; i < size; i ++)
      for(j = i + 1; j < size; j ++)
	if(use_det[i] > use_det[j])
	  I *= -1;
    double res = 1;
    for(i = 0; i < size; i ++)
      res *= (n == i)? nu[max_number_axel][use_det[i]] : nu[i][use_det[i]];
    ret_recurs = ret_recurs - I * res;
    return;
  }
  for(i = 0; i < size; i ++)
    if(use_det[i] < 0)
    {
      use_det[i] = col;
      Recurs(n, col+1);
      use_det[i] = -1;
    }
}
//-------------------------------------------------------------
void CAutoschepka::RunSampel(CStack <SAxel> &axels, const SElementFourth<double> &koof_u, const SElementFourth<int> &koof_sm, const bool &direction)
{
    for(int pb = 0; pb < 4; pb ++)
    {
        if(!param.GetonoffPb(pb))
        {
            for(unsigned i = 0; i < size; i ++)
                axels[i+begin_axel].tare[pb] = -65000;
            continue;
        }
        for(unsigned axel = 0; axel < size; axel ++)
            axels[axel+begin_axel].tare[pb] = (axels[axel+begin_axel].progib[pb] - koof_sm[pb]) * koof_u[pb] * 100.0;}
}
//-------------------------------------------------------------
/*void CAutoschepka::WriteTestFile(const int pb, const CStack <SAxel> &axels)const  //«апись в "test_autoschepka.txt"
{
  WriteLineTestFile("–ассто€ни€", false);
  for(unsigned i = 0; i < size; i ++)
  {
    AnsiString str = "";
    for(unsigned j = 0; j < size; j ++)
      str += (IntToStr(rast[i][j]) + "\t");
    WriteLineTestFile(str, false);
  }
  WriteLineTestFile("", false);
  WriteLineTestFile("progib", false);
  AnsiString str = "";
  for(unsigned i = 0; i < size; i ++)
    str += (IntToStr(axels[i+begin_axel].progib[pb]) + "\t");
  WriteLineTestFile(str, false);
  WriteLineTestFile("", false);
  WriteLineTestFile("nu", false);
  for(unsigned i = 0; i < size; i ++)
  {
    AnsiString str = "";
    for(unsigned j = 0; j < size; j ++)
      str += (FloatToStr(nu[i][j]) + "\t");
    WriteLineTestFile(str, false);
  }
  WriteLineTestFile("", false);
  WriteLineTestFile("nu max_number_axel", false);
  str = "";
  for(unsigned i = 0; i < size; i ++)
    str += (FloatToStr(nu[max_number_axel][i]) + "\t");
  WriteLineTestFile(str, false);
  WriteLineTestFile("", false);
}*/
//-------------------------------------------------------------
/*void CAutoschepka::WriteLineTestFile(AnsiString str, bool new_file, bool new_str) const
{
  if(new_file)
  {
    FILE *f = fopen("test_autoschepka.txt", "wt");
    fclose(f);
    return;
  }
  FILE *f = fopen("test_autoschepka.txt", "at");
  if(new_str)
    fprintf(f, "%s\n", str.c_str());
  else
    fprintf(f, "%s", str.c_str());
  fclose(f);
}*/
//-------------------------------------------------------------


