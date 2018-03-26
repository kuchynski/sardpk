//---------------------------------------------------------------------------
#ifndef CStackH
#define CStackH

#include<stdio.h>
//---------------------------------------------------------------------------
template<class T>
class CStack
{
  private:
        struct SElement
        {
          T e;
          SElement *prev;
          SElement *next;
          SElement(const T &el):e(el), prev(0), next(0){};
        };
        SElement *begin;
        SElement *end;
        unsigned size;
        static T zagl;
        SElement *ptr;
  public:
        CStack();
        CStack(const T &element);
        CStack(const CStack<T>& st);
        ~CStack();
        bool Push(const T &element);
        bool PushMaxSize(const T &element, const unsigned &max_size);
        bool Pop(T &element);
        bool Erase(int b, int e = 0);
        bool Insert(const T &element, int n);
        bool Replace(const T &element, int n);
        bool Insert(const CStack<T>& st, int n);
        unsigned GetSize() const { return size; }
        void Clear();
        CStack<T>& operator=(const CStack<T>& st);
        bool operator==(const CStack<T>& st) const;
        bool operator!=(const CStack<T>& st) const;
        CStack<T>& operator<<(const T &element);
        CStack<T>& operator<<(const CStack<T>& elements);
        CStack<T>& operator>>(T &element);
        T& operator[](unsigned n) const;

        void ClearPtr() {ptr = begin;}
        void SetPtr(const unsigned &n);
        void AddPtr(const unsigned &n);
        T& GetElementPtr() {return ptr->e;}
        void SetElementPtr(const T &element) {ptr->e = element;}
        void AddElementPtr(const T &add) {ptr->e = ptr->e + add;}
};
template<class T> T CStack<T>::zagl;
//---------------------------------------------------------------------------
template<class T> CStack<T>::CStack(const T &element): size(1)
{
  begin = new SElement(element);
  begin->e = element;
  end = begin;
  size = 1;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>::CStack(): begin(0), end(0), size(0), ptr(0)
{}
//---------------------------------------------------------------------------
template<class T> CStack<T>::CStack(const CStack<T>& st): begin(0), end(0), size(0), ptr(0)
{
  *this = st;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>::~CStack()
{
  Clear();
}
//---------------------------------------------------------------------------
template<class T>void CStack<T>::Clear()
{
  while(end)
  {
    SElement *p = end;
    end = end->prev;
    delete p;
  }
  begin = 0;//NULL;
  size = 0;
}
//---------------------------------------------------------------------------
template<class T>bool CStack<T>::Push(const T &element)
{
  SElement *p = new SElement(element);
  p->prev = end;
  if(end)
    end->next = p;
  end = p;
  if(!begin)
    begin = p;
  size ++;
  return true;
}
//---------------------------------------------------------------------------
template<class T>bool CStack<T>::PushMaxSize(const T &element, const unsigned &max_size)
{
  if(!max_size)
    return false;
  Push(element);
  if(size > max_size)
  {
    SElement *tmp = begin;
    begin = begin->next;
    begin->prev = 0;
    size --;
    delete tmp;
  }
  return true;
}
//---------------------------------------------------------------------------
template<class T>
bool CStack<T>::Pop(T &element)
{
  if(end)
  {
    SElement *tmp = end;
    element = tmp->e;
    if(end->prev)
    {
      end = end->prev;
      end->next = 0;
    }
    else
      begin = end = 0;
    size --;
    delete tmp;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::Erase(int b, int e)
{
  if((b >= size) || (b < 0) || (e >= size) || (e < 0))
    return false;
  if(!e)
    e = b;  
  SElement *b1 = begin;
  for(int i = 0; i < b; i++)
    b1 = b1->next;
  SElement *b2 = b1->prev;
  for(int i = b; i <= e; i++)
  {
    SElement* b3 = b1->next;
    if(end == b1)
      end = b2;
    delete b1;
    b1 = b3;
    size --;
  }
  if(b1)
    b1->prev = b2;
  if(b2)
    b2->next = b1;
  else
    begin = b1;
  return true;  
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::Insert(const T &element, int n)
{
  if((n < 0) || (n >= size))
    return false;
  SElement *b1 = begin;
  for(int i = 0; i < n; i++)
    b1 = b1->next;
  SElement *b2 = new SElement(element);
  b2->prev = b1->prev;
  b2->next = b1;
  b1->prev = b2;
  if(b2->prev)
    b2->prev->next = b2;
  else
    begin = b2;
  size ++;
  return true;
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::Replace(const T &element, int n)
{
  if((n < 0) || (n >= size))
    return false;
  SElement *b1 = begin;
  for(int i = 0; i < n; i++)
    b1 = b1->next;
  b1->e = element;
  return true;
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::Insert(const CStack<T>& st, int n)
{
  if((n < 0) || (n >= size))
    return false;
  SElement *b = st.begin;
  while(b)
  {
    Insert(b->e, n++);
    b = b->next;
  }
  return true;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>& CStack<T>::operator=(const CStack<T>& st)
{
  Clear();
  for(SElement *b = st.begin; b; b = b->next)
    *this << b->e;
  return *this;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>& CStack<T>::operator<<(const T &element)
{
  Push(element);
  return *this;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>& CStack<T>::operator<<(const CStack<T>& elements)
{
  SElement *b = elements.begin;
  while(b)
  {
    Push(b->e);
    b = b->next;
  }
  return *this;
}
//---------------------------------------------------------------------------
template<class T> CStack<T>& CStack<T>::operator>>(T &element)
{
//  if(!Pop(element))
//    element = NULL;
  Pop(element);
  return *this;
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::operator==(const CStack<T>& st) const
{
  if(size != st.size)
    return false;
  SElement *b1 = st.begin;
  SElement *b2 = begin;
  while(b1)
  {
    if(b1->e != b2->e)
      return false;
    b1 = b1->next;
    b2 = b2->next;
  }
  return true;
}
//---------------------------------------------------------------------------
template<class T> bool CStack<T>::operator!=(const CStack<T>& st) const
{
  return !operator==(st);
}
//---------------------------------------------------------------------------
template<class T> T& CStack<T>::operator[](unsigned n) const
{
  if(n >= size)
    return zagl;
  SElement *b = begin;
  for(unsigned i = 0; i < n; i++)
    b = b->next;
  return b->e;
}
//---------------------------------------------------------------------------
template<class T>void CStack<T>::SetPtr(const unsigned &n)
{
  if(n >= size)
    return;
  ptr = begin;
  for(unsigned i = 0; i < n; i++)
    ptr = ptr->next;
}
//---------------------------------------------------------------------------
template<class T>void CStack<T>::AddPtr(const unsigned &n)
{
  if(!size)
    return;
  for(unsigned i = 0; i < n; i++)
    if(ptr->next)
      ptr = ptr->next;
    else
      break;  
}
//---------------------------------------------------------------------------
#endif
