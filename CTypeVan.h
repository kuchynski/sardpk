//---------------------------------------------------------------------------
#ifndef CTypeVanH
#define CTypeVanH

#include "CStackFourth.h"
#include "CString.h"
#include <stdlib.h>
//#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class CTypeVan
{
  private:
    CStack12 rast;
    CStack<CString> name;
    CStack12 rv;
    CStack<CString> type;
    CStack<int> lokomotiv;
  public:
    CTypeVan();
    ~CTypeVan();
    CString GetType(SElement12<int> &r, int &lok);
    void Clear();
    void AddVan(SElement12<int> &r);
    void Run();
    CString GetType(const unsigned &n, int &lok);
};
extern CTypeVan type_of_van;
//---------------------------------------------------------------------------
#endif
