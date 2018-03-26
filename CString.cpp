//---------------------------------------------------------------------------
#include "CString.h"
//---------------------------------------------------------------------------
CString::CString(const char *new_str)
{
  int size = strlen(new_str);

  if(size > 255)
    size = 255;

  str = new char[size+1];
  strncpy(str, new_str, size);
  str[size] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(int n)
{
  int size = 0;
  int n2 = abs(n);
  do{
    n2 /= 10;
    size ++;
  }while(n2);
  size += (n < 0)? 1 : 0;
  str = new char[size + 1];
  char i = size - 1;
  if(n < 0)
      str[0] = '-';
  n2 = abs(n);
  do{
    str[i--] = (char)(n2 % 10) + 0x30;
//    str[i++] = (char)(n2 % 10) + 0x30;
    n2 /= 10;
  }while(n2);
  str[size] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(const CString &s)
{
  str = new char[strlen(s.str)+1];
  strcpy(str, s.str);
  str[strlen(s.str)] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(CFileBin &f): str(NULL)
{
  unsigned char size;

  if(!f.Read(&size, 1, 1))
    throw this;
  str = new char[size+1];
  if(!f.Read(str, size+1, 1))
      throw this;
  str[size] = '\0';
}
//---------------------------------------------------------------------------
void CString::SaveToFile(CFileBin &f)
{
    unsigned char size = strlen(str);

    if(!f.Write(&size, 1, 1))
        throw 0;
    if(!f.Write(str, size+1, 1))
        throw 0;
}
//-------------------------------------------------------------
CString::CString (int n, int size)
{
  char Mas[7] = {0, 0, 0, 0, 0, 0, 0};
  int i, s = 0;

  do{
    Mas[s] = n % 10;
    s ++;
    n /= 10;
  }while(n);
  Mas[s] = 0;
  str = new char[size + 1];
  str[size] = '\0';
  for(s = size - 1, i = 0; s >= 0; s --, i ++)
    str[i] = Mas[s] + 0x30;
}
//---------------------------------------------------------------------------
CString::CString(const QString &s)
{
        int size = s.size();
        if(size > 255)
          size = 255;

        str = new char[size+1];
        for(int i = 0; i < size; i ++)
            str[i] = s[i].toAscii();
        str[size] = '\0';
}
//---------------------------------------------------------------------------
CString::~CString()
{
  if(str)
  {
    delete [] str;
    str = NULL;
  }  
}
//---------------------------------------------------------------------------
CString CString::operator+=(const CString &ks)
{
  int size1 = strlen(str);
  int size2 = strlen(ks.str);
  char *str_tmp = str;
  str = new char[size1 + size2 + 1];
  strcpy(str, str_tmp);
  strcpy(str + size1, ks.str);
  str[size1 + size2] = '\0';
  delete []str_tmp;
  return *this;
}
//---------------------------------------------------------------------------
//CString CString::operator+(const CString &ks1, const CString &ks2)
//{
  //return CString(ks1) += ks2;
//}
//---------------------------------------------------------------------------
CString CString::operator+(const CString &ks) const
{
  return CString(str) += ks;
}
//---------------------------------------------------------------------------
CString CString::operator=(const CString &ks)
{
  if(this != &ks)
  {
    if(str)
      delete []str;
    str = new char[strlen(ks.str) + 1];
    strcpy(str, ks.str);
  }
  return *this;
}
//---------------------------------------------------------------------------
QString CString::GetQString() const
{
  return QString(QString::fromLocal8Bit(str));
}
//-------------------------------------------------------------
int CString::GetInt(void)
{
  int ret = 0;

  for(int i = 0; str[i]; i ++)
    if((str[i] >= 0x30) && (str[i] <= 0x39))
      ret = ret*10 + (int)(str[i] - 0x30);
    else
      return 0;  
  return ret;
}
//---------------------------------------------------------------------------

 
