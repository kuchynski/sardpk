#ifndef CFILEBIN_H
#define CFILEBIN_H

#include <stdio.h>
#include <QtGui>
//#include "CString.h"
#define size_buffer 1024

class CFileBin
{
public:
    enum eType {ETRead, ETWrite};

    CFileBin(QString file_name, eType t);
    ~CFileBin();

    bool GetState() {return (f);}
    bool Read(void *ptr, size_t size, size_t nmemb);
    bool Write(void *ptr, size_t size, size_t nmemb);
private:
    FILE *f;
    eType type;
    unsigned char mas[size_buffer];
    unsigned uk_buffer, end_buffer;

    bool NewBufferForRead();
    bool SaveBufferForWrite();
};

#endif // CFILEBIN_H
