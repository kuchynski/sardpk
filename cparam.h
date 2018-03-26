#ifndef CPARAM_H
#define CPARAM_H

#include <QString>
#include <QSettings>
#include <QMutex>
#include "CStackFourth.h"

#define size_param 80

class CParam
{
public:
    CParam();
    ~CParam();

    void SetArchCompressxPb(const unsigned &u) {arch_compressx_pb = u;}
    void SetArchCompressyPb(const unsigned &u) {arch_compressy_pb = u;}
    void SetComNumber(const unsigned &u) {com_number = u;}
    void SetMonitorValue(const unsigned &u) {monitor_value = u;}
    void SetMonitorCompressX(const unsigned &u) {monitor_compressx_pb = u;}
    void SetMonitorCompressY(const unsigned &u) {monitor_compressy_pb = u;}
    void SetonoffPb(const unsigned &n, const unsigned &u) {if(n < 12) onoff_pb[n] = u;}
    void SetNSPb(const unsigned &n, const unsigned &u) {if(n < 12) NS_PB[n] = u;}
    void SetArchonoffPb(const unsigned &n, const unsigned &u) {if(n < 12) arch_onoff_pb[n] = u;}
    void SetArchTogether(const bool &n) {arch_together = (n)? 1 : 0;}
    void SetKoofA(const unsigned &n, const unsigned &u) {QMutexLocker locker(&mutex); if(n < 12) koof_a[n] = u;}
    void SetKoofB(const unsigned &n, const unsigned &u) {QMutexLocker locker(&mutex); if(n < 12) koof_b[n] = u;}
    void SetKoofC(const unsigned &n, const int &u) {QMutexLocker locker(&mutex); if(n < 12) koof_c[n] = u;}
    void SetKoofA(SElement12<unsigned> u) {QMutexLocker locker(&mutex); koof_a = u;}
    void SetKoofB(SElement12<unsigned> u) {QMutexLocker locker(&mutex); koof_b = u;}
    void SetKoofC(SElement12<int> u) {QMutexLocker locker(&mutex); koof_c = u;}
    void SetPbNull(const SElement12<int> &u) {QMutexLocker locker(&mutex); pb_null = u;}
    void SetRastA(const unsigned &n, const unsigned &u) {if(n < 4) rast_a[n] = u;}

    unsigned GetArchCompressxPb() {return arch_compressx_pb;}
    unsigned GetArchCompressyPb() {return arch_compressy_pb;}
    unsigned GetComNumber() {return com_number;}
    unsigned GetMonitorValue() {return monitor_value;}
    unsigned GetMonitorCompressX() {return monitor_compressx_pb;}
    unsigned GetMonitorCompressY() {return monitor_compressy_pb;}
    unsigned GetonoffPb(const unsigned &n) {return (n < 12)? onoff_pb[n] : 0;}
    unsigned GetArchonoffPb(const unsigned &n) {return (n < 12)? arch_onoff_pb[n] : 0;}
    SElement12<unsigned> GetArchonoffPb() {return arch_onoff_pb;}
    bool GetArchTogether() {return (arch_together)? true : false;}
    SElementFourth<unsigned> GetRastA() {return rast_a;}
    unsigned GetRastA(const unsigned &n)const {return (n < 4)? rast_a[n] : 0;}
    unsigned GetNSPb(const unsigned &n) {return (n < 12)? NS_PB[n] : 0;}
    unsigned GetKoofA(const unsigned &n) {return (n < 12)? koof_a[n] : 0;}
    unsigned GetKoofB(const unsigned &n) {return (n < 12)? koof_b[n] : 0;}
    int GetKoofC(const unsigned &n) {return (n < 12)? koof_c[n] : 0;}
   // int GetPbNull(const unsigned &n) {QMutexLocker locker(&mutex); return (n < 4)? pb_null[n] : 0;}
    SElement12<int> GetPbNull() {return pb_null;}
    int GetPbNull(const unsigned &n) {return (n < 12)? pb_null[n] : 0;}

    SElement12<unsigned> GetKoofA() {return koof_a;}
    SElement12<unsigned> GetKoofB() {return koof_b;}
    SElement12<int> GetKoofAInt();
    SElement12<int> GetKoofC() {return koof_c;}

    void SetSettings();
private:

    QSettings settings;
    QMutex mutex;
    unsigned param_u[size_param];
    int param_i[size_param];

    unsigned count;
    unsigned arch_compressx_pb, arch_compressy_pb, com_number, arch_together;
    unsigned monitor_value, monitor_compressx_pb, monitor_compressy_pb;
    SElement12<unsigned> onoff_pb, arch_onoff_pb, NS_PB, koof_a, koof_b;
    SElementFourth<unsigned> rast_a;
    SElement12<int> koof_c, pb_null;

    void ReadSettings();
};
extern CParam param;

#endif // CPARAM_H
