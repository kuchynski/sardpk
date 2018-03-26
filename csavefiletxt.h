#ifndef CSAVEFILETXT_H
#define CSAVEFILETXT_H

#include <stdio.h>
#include "CString.h"

class CSaveFileTxt
{
private:
    FILE *f;
public:
    CSaveFileTxt(CString file_name, const bool &new_file, CString data = "");
    ~CSaveFileTxt();
    void AddLineText(CString d);
    bool GetState();
};

#endif // CSAVEFILETXT_H
