//---------------------------------------------------------------------------
#ifndef CBlocH
#define CBlocH
//---------------------------------------------------------------------------
#include "CFilter.h"
#include "CAutoschepka.h"
#include "CSostav.h"
#include "SStateBloc.h"
#include "cparam.h"
#include "cprocessoroftare.h"

#include "CTurn.h"

class CBloc
{
private:
    SElement12<unsigned> from_opt_to_DLP_obl[2];
    SElement12<int> mas_in[100];
    int sm_defect_from_filter;

    SElement12<CFilter> filter_ves, filter_ves2;

    CSostav sostav, ready_sostav;
    CStack12 data_in;
    CStack12 data_defect;
    unsigned direction;
    int st, koleso1, koleso2, koleso3, koleso_van;
    CTurn<int> begin_koleso;
    CTurn<int> uk_data_begin_defect;
    double speed;
    double timer_convert;
    int incr;
    unsigned kirunak;
    double from_1pedal_to2;
    int kol_van;
    CStackFourth convert_data;
    double speed_cm;

    int rast, rast_autoschepka;

    CProcessorOfTare *processor_of_tare;
    int time_work_of_processor;

    int ConvertData(const double &distance_cm);
    void DeleteProcessor();

public:
    CBloc();
    ~CBloc();

    bool IRQState(SStateBloc &sb, const unsigned &k, CStack<unsigned char> &data_bin);
    void IRQKoleso(const unsigned st_time_koleso, const unsigned &pedal);
    SElement12<int> IRQPB(const SElement12<int> &z);
    void IRQNewSecond();
    void IRQEnd();
    void Clear(const bool work = true);
    void SetOptions();
    bool IsNewSostav();
    void GetSostav(CSostav &s) {s = ready_sostav; ready_sostav.Clear();}
};
//-------------------------------------------------------------
#endif
