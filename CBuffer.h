//---------------------------------------------------------------------------
#ifndef CBufferH
#define CBufferH

#include <stdio.h>
#include "cfilebin.h"

template<class T>            
class CBuffer
{
  private:
        T *buf;
        unsigned size;
        static T zagl;
  public:
        CBuffer(): buf(NULL), size(0) {}
        CBuffer(const unsigned &s, T *b = NULL);
        CBuffer(const CBuffer &b): buf(NULL), size(0) {*this = b;}
        CBuffer(CFileBin &f): buf(NULL), size(0) {LoadFromFile(f);}
        ~CBuffer() {Clear();}
        CBuffer<T>& operator=(const CBuffer<T>& b);

        void Clear(const unsigned &new_size = 0);
        T& operator[](unsigned n) const {return (n >= size || !buf)? zagl : buf[n];}
        unsigned GetSize()const {return size;}
        bool LoadFromFile(CFileBin &f);
        void SaveToFile(CFileBin &f);
};
template<class T> T CBuffer<T>::zagl;
//---------------------------------------------------------------------------
template<class T> bool CBuffer<T>::LoadFromFile(CFileBin &f)
{
  Clear();
  if(!f.Read(&size, sizeof(size), 1))
    return false;
  if(size)
  {
    buf = new T[size];
    if(!f.Read(buf, sizeof(T)*size, 1))
      return false;
  }
  return true;
}
//---------------------------------------------------------------------------
template<class T> void CBuffer<T>::SaveToFile(CFileBin &f)
{
    f.Write(&size, sizeof(size), 1);
    if(size)
        f.Write(buf, sizeof(T)*size, 1);
}
//---------------------------------------------------------------------------
template<class T>CBuffer<T>::CBuffer(const unsigned &s, T *b): buf(NULL), size(s)
{
  if(!size)
    return;
  buf = new T[size];
  if(!b)
    return;
  T *uk1 = buf, *uk2 = b;
  for(unsigned i = 0; i < size; i ++, uk1 ++, uk2++)
    *uk1 = *uk2;
}
//---------------------------------------------------------------------------
template<class T> CBuffer<T>& CBuffer<T>::operator=(const CBuffer<T>& b)
{
  Clear();
  if(!b.size || !b.buf)
    return *this;
  size = b.size;
  buf = new T[size];
  T *uk1 = buf, *uk2 = b.buf;
  for(unsigned i = 0; i < size; i ++, uk1 ++, uk2++)
    *uk1 = *uk2;
  return *this;
}
//---------------------------------------------------------------------------
template<class T> void CBuffer<T>::Clear(const unsigned &new_size)
{
  size = new_size;
  if(buf)
    delete [] buf;
  buf = NULL;
  if(size)
    buf = new T[size];
}
//---------------------------------------------------------------------------
#endif
 
