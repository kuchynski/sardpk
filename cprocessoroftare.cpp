#include "cprocessoroftare.h"
#include "csavefiletxt.h"

CProcessorOfTare::CProcessorOfTare(CSostav &s, CStack12 &d, CStack12 &dd, CStackFourth &cd, QObject *parent) :
    sostav(s), convert_data(cd), QThread(parent), state(psWork), data_defect(dd.GetSize()),
    tdata(d.GetSize()), data(0)
{
    FILE *f = fopen("test_proc.txt", "wt"); fprintf(f, "beg"); fclose(f);
    if(tdata.GetSize())
    {
        unsigned size = tdata.GetSize();
        d.ClearPtr();
        for(unsigned i = 0; i < size; i ++, d.AddPtr(1))
            tdata[i] = d.GetElementPtr();
    }
    f = fopen("test_proc.txt", "at"); fprintf(f, "i"); fclose(f);
    if(data_defect.GetSize())
    {
        unsigned size = data_defect.GetSize();
        dd.ClearPtr();
        for(unsigned i = 0; i < size; i ++, dd.AddPtr(1))
            data_defect[i] = dd.GetElementPtr();
    }
    f = fopen("test_proc.txt", "at"); fprintf(f, "n "); fclose(f);
}
CProcessorOfTare::~CProcessorOfTare()
{
    FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "-en"); fclose(f);
    data_tmp.Clear();
    tdata.Clear();
    data_defect.Clear();
    convert_data.Clear();
    sostav.Clear();
    f = fopen("test_proc.txt", "at"); fprintf(f, "d"); fclose(f);
}
void CProcessorOfTare::run()
{
    state = psWork;
    FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "-1 "); fclose(f);
    if(ConvertDataEnd1())
    {
        f = fopen("test_proc.txt", "at"); fprintf(f, "-2 "); fclose(f);
        ConvertDataEnd2();
        f = fopen("test_proc.txt", "at"); fprintf(f, "-3 "); fclose(f);
        sostav.End(data, data_defect);
        data_tmp.Clear();
        data.Clear();
        tdata.Clear();
        data_defect.Clear();
        convert_data.Clear();
//f = fopen("test_proc.txt", "at"); fprintf(f, "\n*%d* ", a); fclose(f);
        f = fopen("test_proc.txt", "at"); fprintf(f, "-4 *%d*", sostav.GetKolVans()); fclose(f);
        //while(1){}
        state = psNewSostav;
    }
    else
    {
        state = psNoSostav;
        f = fopen("test_proc.txt", "at"); fprintf(f, "-5 "); fclose(f);
    }

    f = fopen("test_proc.txt", "at"); fprintf(f, "-6 "); fclose(f);
    exec();
    f = fopen("test_proc.txt", "at"); fprintf(f, "-7 "); fclose(f);
}
CProcessorOfTare::eProcessorState CProcessorOfTare::GetSostav(CSostav &s)
{
    if(state == psWork)
        return psWork;
    if(state == psEnd)
        return psEnd;

    if(state == psNewSostav)
    {
        state = psEnd;
        s = sostav;
        return  psNewSostav;
    }
    state = psEnd;
    return psNoSostav;
}
//-------------------------------------------------------------
bool CProcessorOfTare::ConvertDataEnd1()
{
    //while(1){};
    unsigned size = tdata.GetSize();
    if(!size)
        return false;
    bool ret = true;
FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "\nc1 "); fclose(f);
    data_tmp.Clear();
f = fopen("test_proc.txt", "at"); fprintf(f, "c2 "); fclose(f);
    for(unsigned n = 0; n < convert_data.GetSize(); n ++)
    {
f = fopen("test_proc.txt", "at"); fprintf(f, "\n%d ", n); fclose(f);
        double begin = static_cast<double>(convert_data[n][0]);
f = fopen("test_proc.txt", "at"); fprintf(f, "%d ", begin); fclose(f);
        if(begin < 0)
            begin = 0;
        const double end = static_cast<double>(convert_data[n][1]);
        const unsigned end_k = convert_data[n][2];
        const double step = (end - begin) / static_cast<double>(convert_data[n][2]);
        int j, j_prev = -1;        
f = fopen("test_proc.txt", "at"); fprintf(f, "%f %d %f ", end, end_k, step); fclose(f);
        if((begin >= size))// || (step >= 7))
        {
          ret = false;
          break;
        }
f = fopen("test_proc.txt", "at"); fprintf(f, "- "); fclose(f);
        SElement12<int> d = tdata[static_cast<int>(begin)];
        double i = begin;
f = fopen("test_proc.txt", "at"); fprintf(f, "- "); fclose(f);
        for(unsigned k = 0; k < end_k; i += step, k ++)
        {
            j = static_cast<int>(i);
            if(j >= size)
                break;
            if(j != j_prev)
            {
                d = tdata[j];
                j_prev = j;
            }
            data_tmp << d;
        }
f = fopen("test_proc.txt", "at"); fprintf(f, "| "); fclose(f);
    }
f = fopen("test_proc.txt", "at"); fprintf(f, "c3 "); fclose(f);
    convert_data.Clear();
f = fopen("test_proc.txt", "at"); fprintf(f, "c4\n"); fclose(f);
    return ret;
}
void CProcessorOfTare::ConvertDataEnd2()
{
FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "\n convert2"); fclose(f);
    unsigned size = data_tmp.GetSize();
f = fopen("test_proc.txt", "at"); fprintf(f, " *%d*", size); fclose(f);
    if(!size)
        return;
    data = CBuffer<SElement12<short> >(size);
    data_tmp.ClearPtr();
    for(int i = 0; i < size; i ++, data_tmp.AddPtr(1))//, g.AddPtr(1))
    {
      SElement12<int> tmp_i(data_tmp.GetElementPtr());
      SElement12<short> tmp_sh(tmp_i[0], tmp_i[1], tmp_i[2], tmp_i[3], tmp_i[4], tmp_i[5],
                               tmp_i[6], tmp_i[7], tmp_i[8], tmp_i[9], tmp_i[10], tmp_i[11]);
      data[i] = tmp_sh;
    }
f = fopen("test_proc.txt", "at"); fprintf(f, " - "); fclose(f);
    data_tmp.Clear();
f = fopen("test_proc.txt", "at"); fprintf(f, " end"); fclose(f);
}
