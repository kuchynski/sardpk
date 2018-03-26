#ifndef CCOMPORT_H
#define CCOMPORT_H

#include <QThread>
#include <QObject>
#include <windows.h>
#include <setupapi.h>
#include "CStackFourth.h"
//#include <qextserialport/qextserialport.h>
#include "SStateBloc.h"
#include "CBloc.h"

class CComPort : public QThread
{
    Q_OBJECT
public:
    enum eComPortState {cpsNoState, cpsConnect, cpsConnect1, cpsConnect2, cpsReload, cpsDisConnect, cpsError};
private:
    unsigned number_port, new_number_port;
    unsigned second, new_second;
    eComPortState state;
    HANDLE hCom;
    //QextSerialPort *port;
    CBloc bloc;
    unsigned st_packet, bad_second;
    FILE *f;

    CStack<BYTE> data_bin;
    bool enabled_sostav;
    unsigned st_koleso[2];

    SStateBloc state_bloc;

    void Connect();
    void DisConnect(eComPortState new_state);
    bool IsNewNumberPort(const bool &set = true);
    bool IsNewSecond();
    bool PacketState();
    bool PacketPB();
    bool WaitEndMessage();
    bool InByte(BYTE& byte);
    bool ReadByte(BYTE &byte);
    bool ReadPacket(BYTE *byte, const int &kol);
    void SetState(const eComPortState &s) {state = s; emit NewState(state);}
    void OnNewSostav() {emit NewSostav();}

public:
    CComPort(QObject *parent = 0);
    ~CComPort();

    void run();    
    void SetNumberPort(const unsigned &n) {new_number_port = n;}
    void GetData(SStateBloc &sb)const {sb = state_bloc;}
    void GetSostav(CSostav &s) {bloc.GetSostav(s);}
    void SetNewSecond() {new_second ++;}

signals:
    void NewState(unsigned s);
    void NewSostav();

public slots:

};
#endif // CCOMPORT_H
