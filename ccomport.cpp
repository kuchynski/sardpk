#include "ccomport.h"
#define port_speed 460800
unsigned st_time_koleso_old;
CComPort::CComPort(QObject *parent) : QThread(parent),
    number_port(0), new_number_port(0), hCom(0), //port(0),
    enabled_sostav(false), f(0)
{
    FILE *f = fopen("test_com.txt", "wt"); fprintf(f, ""); fclose(f);
}
CComPort::~CComPort()
{
    DisConnect(cpsDisConnect);
}

void CComPort::run()
{
    SetState(cpsNoState);
    st_koleso[0] = st_koleso[1] = 0;
    BYTE byte;
    bool ok = false;
    while(1)
    {
        if(IsNewNumberPort())
        {
            DisConnect(cpsDisConnect);
            if(number_port == 100)//завершить поток
                break;
            ok = false;
            Connect();
        }
        if(state == cpsConnect)
        {
            if(IsNewSecond())
            {
                bloc.IRQNewSecond();
                state_bloc.enabled = (st_packet > 25);
                st_packet = 0;
                if(!state_bloc.enabled && (number_port < 10))
                    emit NewState(cpsConnect2);
                else
                    emit NewState(cpsConnect1);
            }
            if(bloc.IsNewSostav())
                OnNewSostav();
            if(!ok)
                WaitEndMessage();
            ok = false;
            if(ReadByte(byte))
            {
                switch(byte)
                {
                    case 0x30: // Состояние
                        ok = PacketState();
                        break;
                    case 0x73: // Путевой блок
                        ok = PacketPB();
                        break;
                    default : break;
                }
            }
        }
        else
            msleep(100);
    }
}
void CComPort::Connect()
{
    if(!number_port)
    {
        SetState(cpsNoState);
        return;
    }
    int ok = false;
    if(number_port < 10)
    {
        do{
            WCHAR name[6] = {'C', 'O', 'M', '0' + number_port};
            COMMCONFIG ComCfg;
            COMMTIMEOUTS CommTimeouts;
            BOOL fSuccess;

            ComCfg.dwSize = sizeof(COMMCONFIG);
            ComCfg.wVersion = 1;
            ComCfg.dwProviderSubType = PST_RS232;
            ComCfg.dwProviderOffset = 0;
            ComCfg.dwProviderSize = 0;
            //t = (kol*ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant)
            CommTimeouts.ReadIntervalTimeout = 100;//между двумя символами
            CommTimeouts.ReadTotalTimeoutMultiplier = 1;
            CommTimeouts.ReadTotalTimeoutConstant = 100;
            CommTimeouts.WriteTotalTimeoutMultiplier = 0;
            CommTimeouts.WriteTotalTimeoutConstant = 0;
            SetDefaultCommConfig(name, &ComCfg, sizeof(ComCfg));
            hCom = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
            if(hCom == INVALID_HANDLE_VALUE)
                break;
            GetCommState(hCom, &ComCfg.dcb);
            ComCfg.dcb.ByteSize = 8;
            ComCfg.dcb.Parity = NOPARITY;
            ComCfg.dcb.StopBits = ONESTOPBIT;
            ComCfg.dcb.BaudRate = port_speed;
            SetCommState(hCom, &ComCfg.dcb);
            SetCommTimeouts(hCom, &CommTimeouts);
            SetupComm(hCom, 100000, 0);
            PurgeComm(hCom, PURGE_RXCLEAR);
            ok = true;
        }while(0);

      /*  port = new QextSerialPort("COM" + QString::number(number_port));//, QextSerialPort::EventDriven); //Polling);
        ok = port->open(QIODevice::ReadOnly);//QIODevice:: ReadWrite);
        port->setBaudRate(BAUD100000);
        port->setFlowControl(FLOW_OFF);//FLOW_HARDWARE);//FLOW_OFF FLOW_XONXOFF);
        port->setParity(PAR_NONE);
        port->setDataBits(DATA_8);
        port->setStopBits(STOP_1);
        port->setTimeout(1);*/
    }
    else
    {
        f = fopen("test.bin", "rb");
        ok = (f)? 1 : 0;
    }
    if(!ok)
    {
        CloseHandle(hCom);
        hCom = 0;
//        delete port;
//        port = 0;
        SetState(cpsError);
        return;
    }
    st_packet = 0;
    bad_second = 0;
    SetState(cpsConnect);
    return;
}
void CComPort::DisConnect(eComPortState new_state)
{
  //  if(port)
    if(hCom)
    {
        CloseHandle(hCom);
        hCom = 0;
//        port->close();
       // delete port;
  //      port = 0;
    }
    if(f)
    {
        fclose(f);
        f = 0;
    }
    data_bin.Clear();
    bloc.Clear(false);
    enabled_sostav = false;
    state_bloc.enabled = false;
    state_bloc.connect_pb = 0;
    state_bloc.state_pedal = 0;
    SetState(new_state);
    return;
}
bool CComPort::IsNewNumberPort(const bool &set)
{
    if(new_number_port == number_port)
        return false;
    if(set)
    {
        if(new_number_port > 100)
            new_number_port -= 100;
        number_port = new_number_port;
    }
    return true;
    }
bool CComPort::IsNewSecond()
{
    if(new_second == second)
        return false;
    second = new_second;
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::PacketState()
{
    BYTE bytes[11];
    if(!ReadPacket(bytes, 11))
      return false;

    state_bloc.state_pedal = ((bytes[0] >> 2) & 0x03) ^ 0x03;
    state_bloc.connect_pb = (bytes[0] & 0xF0) >> 4;
    state_bloc.connect_pb += (((WORD)bytes[1]) << 4);
    state_bloc.connect_pb = state_bloc.connect_pb ^ 0xFFF;
    state_bloc.connect_pb = ((state_bloc.connect_pb & 0xFC0) >> 6) + ((state_bloc.connect_pb & 0x03F) << 6);
    SElementDual<unsigned> pedal_state(bytes[2], bytes[3]);
    for(int i = 0; i < 2; i++)
      state_bloc.data_dm[i] = (pedal_state[i] & 0x80)? static_cast<int>(pedal_state[i] | 0xFFFFFF00) : static_cast<int>(pedal_state[i]);
    BYTE kirunak = bytes[4] & 0x3;
    if(kirunak > 0x02)
        return false;
    enabled_sostav = (kirunak > 0);
    state_bloc.enabled = true;
    if(bloc.IRQState(state_bloc, kirunak, data_bin))
        data_bin.Clear();
    unsigned new_st_koleso[2] = {(unsigned)(bytes[8] & 0x0F), (unsigned)(bytes[8] >> 4)};
    if((new_st_koleso[0] != st_koleso[0]) || (new_st_koleso[1] != st_koleso[1]))
    {
        unsigned st_time_koleso = (unsigned)((DWORD)bytes[5] + ((DWORD)bytes[6] << 8) + ((DWORD)(bytes[7] & 0x0F) << 16));
      /*  if((new_st_koleso[0] != st_koleso[0]) && (new_st_koleso[1] != st_koleso[1]))
        {    FILE *f = fopen("test_com.txt", "at"); fprintf(f, "-----------------%d - %d, %d - %d | %d - %d \n", st_koleso[0], new_st_koleso[0], st_koleso[1], new_st_koleso[1], st_time_koleso, st_time_koleso_old); fclose(f);}
        else
        {    FILE *f = fopen("test_com.txt", "at"); fprintf(f, "%d - %d, %d - %d | %d - %d \n", st_koleso[0], new_st_koleso[0], st_koleso[1], new_st_koleso[1], st_time_koleso, st_time_koleso_old); fclose(f);}
        st_time_koleso_old = st_time_koleso;*/
        if(new_st_koleso[0] != st_koleso[0])
        {
            bloc.IRQKoleso(st_time_koleso, 0);
            st_time_koleso = 0;
        }
        if(new_st_koleso[1] != st_koleso[1])
            bloc.IRQKoleso(st_time_koleso, 1);
        for(int i = 0; i < 2; i++)
            st_koleso[i] = new_st_koleso[i];
    }
    st_packet ++;
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::PacketPB()
{
    BYTE bytes[30];

    if(!ReadPacket(bytes, 26))
        return false;
    unsigned ub[24];
    for(int i = 0; i < 24; i ++)
        ub[i] = static_cast<unsigned>(bytes[i]);
    SElement12<int> data_pb;
    for(int i = 0; i < 12; i ++)
    {
        unsigned z = (ub[i*2+1] << 8) + ub[i*2];
        data_pb[i] = static_cast<int>((z & 0x8000)? z | 0xFFFF0000 : z);
    }
    for(int i = 0; i < 6; i ++)
    {
        int tmp = data_pb[i+6];
        data_pb[i+6] = data_pb[i];
        data_pb[i] = tmp;
        data_pb[i] = (param.GetNSPb(i))? data_pb[i] : -data_pb[i];
    }
    for(int i = 0; i < 12; i ++)
        data_pb[i] = (param.GetNSPb(i))? data_pb[i] : -data_pb[i];
    state_bloc.data_pb = bloc.IRQPB(data_pb);
  //  state_bloc.data_pb[i] = data_pb[i];
 //   for(int i = 0; i < 12; i ++)
   //     state_bloc.data_pb[i] = data_pb[i];
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::WaitEndMessage()
{
    BYTE byte;
    int st_bad_mes = 0;
    int st_no_read = 0;
    do{
    //    if(!NewReadPort(byte))
        if(!ReadByte(byte))
            st_no_read ++;
        if(byte == 0x22)
            return true;
        if(IsNewNumberPort(false))
            return false;
        st_bad_mes ++;
        if((st_bad_mes > 50) || (st_no_read > 10))
        {
            return false;
        }
    }while(1);
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::InByte(BYTE& byte)
{
    char w;
    DWORD n1 = 1, n2 = 0;

    bool ret = ReadFile(hCom, &byte, n1, &n2, NULL);
//    int ret =  port->read(&w, 1);
    if((ret == true) && n2 == n1)
        n2 = 1;
    else
        n2 = 0;
//    byte = w;
    return ret && (n2 == n1);
//  return (ret == 1);
}
//-------------------------------------------------------------
bool CComPort::ReadByte(BYTE &byte)
{
    if(number_port < 10)
    {
        if(InByte(byte))
        {
            if(enabled_sostav)
                data_bin << byte;
            return true;
        }
    }
    else
    {
        if(fread(&byte, 1, 1, f) == 1)
          return true;
        else
          bloc.IRQEnd();
    }
    return false;
}
//---------------------------------------------------------------------------
bool CComPort::ReadPacket(BYTE *byte, const int &kol)
{
    BYTE crc = 0x55;
    for(int i = 0; i < kol; i ++)
      if(!ReadByte(byte[i]))
        return false;
    for(int i = 0; i < (kol-2); i ++)
      crc = crc ^ byte[i];
    if((byte[kol-2] != crc) || (byte[kol-1] != 0x22))
      return false;
    return true;
}
//---------------------------------------------------------------------------

