#include "cparam.h"

CParam param;

CParam::CParam(): settings("Кучинский", "САРДПК")
{
    ReadSettings();
    count ++;
}
CParam::~CParam()
{
    SetSettings();
}
void CParam::SetSettings()
{
    param_u[0] = arch_compressx_pb;
    param_u[1] = arch_compressy_pb;
    param_u[2] = com_number;
    param_u[3] = monitor_value;
    param_u[4] = monitor_compressx_pb;
    param_u[5] = monitor_compressy_pb;
    param_u[70] = arch_together;
    for(unsigned i = 0; i < 12; i++)
    {
        param_i[i] = pb_null[i];
        param_i[12+i] = koof_c[i];
        param_u[6+i] = onoff_pb[i];
        param_u[18+i] = koof_a[i];
        param_u[30+i] = koof_b[i];
        param_u[42+i] = arch_onoff_pb[i];
        param_u[54+i] = NS_PB[i];
    }
    for(unsigned i = 0; i < 4; i++)
        param_u[66+i] = rast_a[i];

    settings.setValue("/count", count);
    for(unsigned i = 0; i < size_param; i ++)
    {
        settings.setValue("/uparam" + QString::number(i, 16), param_u[i]);
        settings.setValue("/iparam" + QString::number(i, 16), param_i[i]);
    }
}
void CParam::ReadSettings()
{
    count = settings.value("/count", 0).toUInt();
    for(unsigned i = 0; i < size_param; i ++)
    {
        param_u[i] = settings.value("/uparam" + QString::number(i, 16), 0).toUInt();
        param_i[i] = settings.value("/iparam" + QString::number(i, 16), 0).toInt();
    }
    arch_compressx_pb = (!param_u[0])? 1 : (param_u[0] > 100)? 100 : param_u[0];
    arch_compressy_pb = (!param_u[1])? 1 : (param_u[1] > 500)? 500 : param_u[1];
    com_number = (param_u[2] > 10)? 10 : param_u[2];
    monitor_value = (param_u[3] > 13)? 0 : param_u[3];
    monitor_compressx_pb = (!param_u[4])? 1 : (param_u[4] > 100)? 100 : param_u[4];
    monitor_compressy_pb = (!param_u[5])? 1 : (param_u[5] > 300)? 300 : param_u[5];
    arch_together = param_u[70];
    for(unsigned i = 0; i < 12; i++)
    {
        pb_null[i] = param_i[i];
        koof_c[i] = (param_i[12+i] < -999)? -999 : (param_i[12+i] > 999)? 999 : param_i[12+i];
        onoff_pb[i] = param_u[6+i];
        koof_a[i] = (param_u[18+i] > 99)? 99 : param_u[18+i];
        koof_b[i] = (param_u[30+i] < 1)? 1 : (param_u[30+i] > 999)? 999 : param_u[30+i];
        arch_onoff_pb[i] = param_u[42+i];
        NS_PB[i] = param_u[54+i];
    }
    for(unsigned i = 0; i < 4; i++)
        rast_a[i] = param_u[66+i];
}
SElement12<int> CParam::GetKoofAInt()
{
    SElement12<int> ret;
    for(unsigned i = 0; i < 12; i++)
        ret[i] = koof_a[i];
    return ret;
}

