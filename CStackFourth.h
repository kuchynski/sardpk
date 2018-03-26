//---------------------------------------------------------------------------
#ifndef CStackFourthH
#define CStackFourthH
#include "CStack.h"
//---------------------------------------------------------------------------
template<class T>
struct SElementFourth
{
        T e[4];
        SElementFourth(){}
        SElementFourth(const T &e0, const T &e1, const T &e2, const T &e3){e[0]=e0; e[1]=e1; e[2]=e2; e[3]=e3;}
        SElementFourth(const T &e0){for(int i = 0; i < 4; i ++) e[i]=e0;}
        SElementFourth(const T *el)
          { for(int i = 0; i < 4; i ++) e[i]=el[i];}
        SElementFourth(const SElementFourth &el)
          { for(int i = 0; i < 4; i = i + 1) e[i]=el.e[i];}

        T& operator[](const unsigned &n) { return (n < 4)? e[n] : e[0];}
        T operator[](const unsigned &n)const { return (n < 4)? e[n] : 0;}
        bool operator==(const SElementFourth &el)const { return ((e[0] == el.e[0])&&(e[1] == el.e[1])&&(e[2] == el.e[2])&&(e[3] == el.e[3]));}
        SElementFourth operator=(const SElementFourth &el)
        {
          if(this != &el)
            for(int i = 0; i < 4; i = i + 1)
              e[i] = el.e[i];
          return *this;
        }
        SElementFourth operator=(const T &n)
        { for(int i = 0; i < 4; i = i + 1)
            e[i] = n;
          return *this;}


        SElementFourth& operator-=(const SElementFourth &el)
        { for(int i = 0; i < 4; i = i + 1)
            e[i] -= el.e[i];
          return *this;}
        SElementFourth& operator+=(const SElementFourth &el)
        { for(int i = 0; i < 4; i = i + 1)
            e[i] += el.e[i];
          return *this;}
        SElementFourth& operator*=(const SElementFourth &el)
        { for(int i = 0; i < 4; i = i + 1)
            e[i] *= el.e[i];
          return *this;}
        SElementFourth& operator/=(const SElementFourth &el)
        { for(int i = 0; i < 4; i = i + 1)
            e[i] /= el.e[i];
          return *this;}
        SElementFourth operator-(const SElementFourth &el)
        { SElementFourth ret = *this;
          return ret -= el;}
        SElementFourth operator+(const SElementFourth &el)
        { SElementFourth ret = *this;
          return ret += el;}
        SElementFourth operator*(const SElementFourth &el)
        { SElementFourth ret = *this;
          return ret *= el;}
        SElementFourth operator/(const SElementFourth &el)
        { SElementFourth ret = *this;
          return ret /= el;}
   /*     SElementFourth operator=(const T &n)
        {
          for(int i = 0; i < 4; i = i + 1)
            e[i] = n;
          return *this;
        }      */
};
class CStackFourth: public CStack<SElementFourth<int> >
{
        bool Push(const int &e0, const int &e1, const int &e2, const int &e3)
        { return CStack<SElementFourth<int> >::Push(SElementFourth<int>(e0, e1, e2, e3));}
        bool Pop(int &e0, int &e1, int &e2, int &e3)
        { SElementFourth<int> e;
          bool ret = CStack<SElementFourth<int> >::Pop(e);
          e0 = e[0];
          e1 = e[1];
          e2 = e[2];
          e3 = e[3];
          return ret;}
};
template<class T>
class CStack4: public CStack<SElementFourth<T> >
{
        bool Push(const T &e0, const T &e1, const T &e2, const T &e3)
        { return CStack<SElementFourth<T> >::Push(SElementFourth<T>(e0, e1, e2, e3));}
        bool Pop(int &e0, int &e1, int &e2, int &e3)
        { SElementFourth<T> e;
          bool ret = CStack<SElementFourth<T> >::Pop(e);
          e0 = e[0];
          e1 = e[1];
          e2 = e[2];
          e3 = e[3];
          return ret;}
};
template<class T>
struct SElement12
{
        T e[12];
        SElement12(){};
        SElement12(const T &e0, const T &e1, const T &e2, const T &e3, const T &e4, const T &e5,
                   const T &e6, const T &e7, const T &e8, const T &e9, const T &e10, const T &e11)
          {e[0]=e0; e[1]=e1; e[2]=e2; e[3]=e3; e[4]=e4; e[5]=e5;
           e[6]=e6; e[7]=e7; e[8]=e8; e[9]=e9; e[10]=e10; e[11]=e11;}
        SElement12(const T &e0){for(int i = 0; i < 12; i ++) e[i]=e0;}
        SElement12(const T *el)
          { for(int i = 0; i < 12; i ++) e[i]=el[i];}
        SElement12(const SElement12 &el)
          { for(int i = 0; i < 12; i = i + 1) e[i]=el.e[i];}
        T& operator[](const int &n) { return e[n];}
        T operator[](const int &n)const { return e[n];}
        bool operator==(const SElement12 &el)const
        {  return ((e[0] == el.e[0])&&(e[1] == el.e[1])&&(e[2] == el.e[2])&&(e[3] == el.e[3])
                 &&(e[4] == el.e[4])&&(e[5] == el.e[5])&&(e[6] == el.e[6])&&(e[7] == el.e[7])
                 &&(e[8] == el.e[8])&&(e[9] == el.e[9])&&(e[10] == el.e[10])&&(e[11] == el.e[11]));}
        SElement12 operator=(const SElement12 &el)
        { if(this != &el)
            for(int i = 0; i < 12; i = i + 1)
              e[i] = el.e[i];
          return *this;}
        SElement12 operator=(const T &n)
        { for(int i = 0; i < 12; i = i + 1)
            e[i] = n;
          return *this;}
        SElement12& operator-=(const SElement12 &el)
        { for(int i = 0; i < 12; i = i + 1)
            e[i] -= el.e[i];
          return *this;}
        SElement12& operator+=(const SElement12 &el)
        { for(int i = 0; i < 12; i = i + 1)
            e[i] += el.e[i];
          return *this;}
        SElement12& operator*=(const SElement12 &el)
        { for(int i = 0; i < 12; i = i + 1)
            e[i] *= el.e[i];
          return *this;}
        SElement12& operator/=(const SElement12 &el)
        { for(int i = 0; i < 12; i = i + 1)
            e[i] /= el.e[i];
          return *this;}
        SElement12 operator-(const SElement12 &el)
        { SElement12 ret = *this;
          return ret -= el;}
        SElement12 operator+(const SElement12 &el)
        { SElement12 ret = *this;
          return ret += el;}
        SElement12 operator*(const SElement12 &el)
        { SElement12 ret = *this;
          return ret *= el;}
        SElement12 operator/(const SElement12 &el)
        { SElement12 ret = *this;
          return ret /= el;}
};
class CStack12: public CStack<SElement12<int> >
{
        bool Push(const int &e0, const int &e1, const int &e2, const int &e3, const int &e4, const int &e5,
                  const int &e6, const int &e7, const int &e8, const int &e9, const int &e10, const int &e11)
        { return CStack<SElement12<int> >::Push(SElement12<int>(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11));}
        bool Pop(int &e0, int &e1, int &e2, int &e3, int &e4, int &e5, int &e6, int &e7, int &e8, int &e9, int &e10, int &e11)
        { SElement12<int> e;
          bool ret = CStack<SElement12<int> >::Pop(e);
          e0 = e[0];
          e1 = e[1];
          e2 = e[2];
          e3 = e[3];
          e4 = e[4];
          e5 = e[5];
          e6 = e[6];
          e7 = e[7];
          e8 = e[8];
          e9 = e[9];
          e10 = e[10];
          e11 = e[11];
          return ret;}
};
//---------------------------------------------------------------------------
#endif


