//---------------------------------------------------------------------------
#ifndef CAutoschepkaH
#define CAutoschepkaH

#include "SAxel.h"
#include <math.h>
#include "CStackDual.h"
#include "cparam.h"
#include "cfilebin.h"

const int max_number_axel = 24;
//---------------------------------------------------------------------------
class CAutoschepka
{
  private:
    unsigned begin_axel;
    unsigned size;
    double nu[max_number_axel + 1][max_number_axel];
    int use_det[max_number_axel];
    double ret_recurs, det;
    int rast[max_number_axel][max_number_axel];

    void Recurs(const unsigned n, const unsigned col);

 //   void WriteTestFile(const int pb, const CStack <SAxel> &axels)const;
   // void WriteLineTestFile(AnsiString str, bool new_file, bool new_str = true) const;

  public:
    enum ETypeRun {ETRTeor = 0, ETRPractic = 1, ETRSimple = 2};
    CAutoschepka();
    CAutoschepka(CFileBin &f);
    CAutoschepka(const CAutoschepka &a);
    CAutoschepka(const int &b);
    ~CAutoschepka(){}
    CAutoschepka& operator=(const CAutoschepka &a);

    void SaveToFile(CFileBin &f);
    void SetSizeAxel(const int &s) {size = s;}
    void Clear();
    unsigned GetBegin()const {return begin_axel;}
    unsigned GetEnd()const {return begin_axel + size - 1;}
    bool IsMy(const unsigned &n)const;

    void RunClassic(const ETypeRun &type_run, CStack <SAxel> &axels, const SElementFourth<double> &koof_k, const SElementFourth<double> &koof_u,
             const SElementFourth<int> &koof_sm, const bool &direction);
//    void RunClassicTeor(CStack <SAxel> &axels, const SElementFourth<double> &koof_k, const SElementFourth<double> &koof_u,
  //           const SElementFourth<int> &koof_sm, const bool &direction);
//    void RunClassicPractic(CStack <SAxel> &axels, const SElementFourth<double> &koof_u, const SElementFourth<int> &koof_sm, const bool &direction);
    void RunSampel(CStack <SAxel> &axels, const SElementFourth<double> &koof_u, const SElementFourth<int> &koof_sm, const bool &direction);
};
//---------------------------------------------------------------------------
#endif
