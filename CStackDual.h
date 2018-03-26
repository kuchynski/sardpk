//---------------------------------------------------------------------------
#ifndef CStackDualH
#define CStackDualH
#include "CStack.h"
//---------------------------------------------------------------------------
template<class T>
struct SElementDual
{
        T e[2];
        SElementDual(){}
//        SElementDual(const T &e0, const T &e1):e[0](e0), e[1](e1){};
        SElementDual(const T &e0, const T &e1){e[0]=e0; e[1]=e1;}
        SElementDual(const T &el){e[0] = el; e[1] = el;}
        SElementDual(const SElementDual &el){e[0] = (el.e[0]); e[1] = (el.e[1]);}
        T& operator[](const int &n) { return e[n];}
        bool operator==(const SElementDual &el)const { return ((e[0] == el.e[0])&&(e[1] == el.e[1]));}
        SElementDual operator=(const SElementDual &el)
        {
          if(this != &el)
          {
            e[0] = el.e[0];
            e[1] = el.e[1];
          }
          return *this;
        }
};
class CStackDual: public CStack<SElementDual<int> >
{
        bool Push(const int &e0, const int &e1)
        { return CStack<SElementDual<int> >::Push(SElementDual<int>(e0, e1));}
        bool Pop(int &e0, int &e1)
        { SElementDual<int> e;
          bool ret = CStack<SElementDual<int> >::Pop(e);
          e0 = e[0];
          e1 = e[1];
          return ret;}
};
//---------------------------------------------------------------------------
#endif
