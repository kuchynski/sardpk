#include "cfilebin.h"

CFileBin::CFileBin(QString file_name, eType t): type(t), f(0), uk_buffer(0), end_buffer(0)
{
    QByteArray str = file_name.toAscii();

    if(type == ETRead)
        f = fopen(str.data(), "rb");
    else
        f = fopen(str.data(), "wb");
//    f = fopen(str.data(), (type == ETRead)? "rb" : "wb");
    if(!f)
        return;
    if(type == ETRead)
        NewBufferForRead();
}
CFileBin::~CFileBin()
{
    if(f)
    {
        SaveBufferForWrite();
        fclose(f);
    }
}
bool CFileBin::Read(void *ptr, size_t size, size_t nmemb)
{
    if(!f || type != ETRead || size <= 0)
        return false;
    unsigned char *ptr_tmp = static_cast<unsigned char*>(ptr);
    for(int n = 0; n < nmemb; n ++)
    {
        unsigned uk = 0;
        do
        {
           *(ptr_tmp) = mas[uk_buffer];
           uk ++;
           ptr_tmp ++;
           uk_buffer++;
           if(uk_buffer >= size_buffer)
               NewBufferForRead();
           if(uk >= size)
               break;
        }while(1);
    }
    return true;
}
bool CFileBin::Write(void *ptr, size_t size, size_t nmemb)
{
//    fwrite(ptr, size, 1, f);
  //  return true;
    if(!f || type != ETWrite || size <= 0)
        return false;
    unsigned char *ptr_tmp = static_cast<unsigned char*>(ptr);
    for(int n = 0; n < nmemb; n ++)
    {
        unsigned uk = 0;
        do
        {
            mas[uk_buffer] = *ptr_tmp;
            uk ++;
            ptr_tmp ++;
            uk_buffer++;
            if(uk_buffer >= size_buffer)
                SaveBufferForWrite();
            if(uk >= size)
                break;
        }while(1);
    }
    return true;
}
bool CFileBin::NewBufferForRead()
{
    int w1 = sizeof(mas[0]);
    int w2 = size_buffer;
    int w3 = sizeof(mas);
    end_buffer = fread(mas, sizeof(mas), 1, f);
    uk_buffer = 0;
    return (end_buffer);
}
bool CFileBin::SaveBufferForWrite()
{
//    int r = sizeof(mas[0])*uk_buffer;
  //  int r2 = fwrite(mas, uk_buffer, 1, f);
  //  int r2 = ;
    bool ret = (fwrite(mas, sizeof(mas[0])*uk_buffer, 1, f) == uk_buffer);
    uk_buffer = 0;
    return ret;
}

