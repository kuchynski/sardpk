#include "csavefiletxt.h"

CSaveFileTxt::CSaveFileTxt(CString file_name, const bool &new_file, CString data)
{
    if(new_file)
        f = fopen(file_name.GetPtrChar(), "wt");
    else
        f = fopen(file_name.GetPtrChar(), "at");
    if(data != "")
        AddLineText(data);
}
bool CSaveFileTxt::GetState()
{
    return f != 0;
}
CSaveFileTxt::~CSaveFileTxt()
{
    if(f)
        fclose(f);
}
void CSaveFileTxt::AddLineText(CString d)
{
    if(f)
        fprintf(f, "%s\n", d.GetPtrChar());
}
