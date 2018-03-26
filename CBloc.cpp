//---------------------------------------------------------------------------
#include "CBloc.h"

#define max_time_processor 60
//---------------------------------------------------------------------------
CBloc::CBloc(): processor_of_tare(0), time_work_of_processor(0)
{

  CStack<double> a3, b3;
  b3 << 0.97904252295968 <<  4.89521261479841 <<  9.79042522959681  // [b,a]=butter(5, 239*2/Fs)
     << 9.79042522959681 <<  4.89521261479841 <<  0.97904252295968;
  a3 << 1.00000000000000 <<  4.95764005747337 <<  9.83145602608809
     << 9.74851604793013 <<  4.83322434145496 <<  0.95852426176326;
  sm_defect_from_filter = 0;
  filter_ves[0].SetAB(a3, b3);
  CStack<double> a4, b4;
  a4 << 1.00000000000000 << -3.99657942627525 <<  5.98974412777420  //Fs = 480;   [b,a]=butter(4, 0.1*2/Fs,'high')
     << -3.98974997086573 <<  0.99658526936971;
  b4 << 0.99829117464281 << -3.99316469857123 <<  5.98974704785684
     << -3.99316469857123 <<  0.99829117464281;
  filter_ves2[0].SetAB(a4, b4);

  for(int i = 1; i < 12; i ++)
  {
    filter_ves[i] = filter_ves[0];
    filter_ves2[i] = filter_ves2[0];
  }
  Clear(false);
  FILE *f = fopen("test_bloc.txt", "wt"); fprintf(f, ""); fclose(f);
  SetOptions();
}
//-------------------------------------------------------------
CBloc::~CBloc()
{
  Clear(false);
  DeleteProcessor();
}
//-------------------------------------------------------------
void CBloc::SetOptions()
{
    SElementFourth<unsigned> a = param.GetRastA();
    from_opt_to_DLP_obl[0][0] = from_opt_to_DLP_obl[0][6] = a[0];
    for(int i = 1; i <= 5; i++)
        from_opt_to_DLP_obl[0][i] = from_opt_to_DLP_obl[0][i+6] = from_opt_to_DLP_obl[0][i-1] + 54;// cm
    from_opt_to_DLP_obl[1][5] = from_opt_to_DLP_obl[1][11] = a[1];
    for(int i = 4; i >= 0; i--)
        from_opt_to_DLP_obl[1][i] = from_opt_to_DLP_obl[1][i+6] = from_opt_to_DLP_obl[1][i+1] + 54;// cm
//    from_1pedal_to2 = (54.0 * 5.0 + (double)from_opt_to_DLP_obl[0][0] + (double)from_opt_to_DLP_obl[1][5]) / 100.0;
    from_1pedal_to2 = static_cast<double>(a[2]) / 100.0;
}
//-------------------------------------------------------------
void CBloc::IRQNewSecond()
{
    if(time_work_of_processor)
        time_work_of_processor ++;
    if(time_work_of_processor > max_time_processor)
        DeleteProcessor();
}
//-------------------------------------------------------------
void CBloc::Clear(const bool work)
{
    if(sostav.GetNumberOfVan())// && work)
    {
        FILE *f = fopen("test_bloc.txt", "at"); fprintf(f, "\nCleaR "); fclose(f);
        sostav.SetDirection(direction == 2);
    //    sostav.SetKoofA(param.GetKoofA());
    //    sostav.SetKoofB(param.GetKoofB());
    //    sostav.SetKoofC(param.GetKoofC());
        ConvertData(0);//speed_cm * 15.0);
        DeleteProcessor();
        f = fopen("test_bloc.txt", "at"); fprintf(f, "c1 "); fclose(f);
        processor_of_tare = new CProcessorOfTare(sostav, data_in, data_defect, convert_data);
        time_work_of_processor = 1;
        f = fopen("test_bloc.txt", "at"); fprintf(f, "c2 "); fclose(f);
        processor_of_tare->start(QThread::LowPriority);
        f = fopen("test_bloc.txt", "at"); fprintf(f, "c3 "); fclose(f);
    }
    sostav.Clear();
    data_in.Clear();
    data_defect.Clear();
    rast = 0;
    rast_autoschepka = 0;
    convert_data.Clear();
    direction = 0;
    st = 0;
    begin_koleso.Clear();
    uk_data_begin_defect.Clear();
    timer_convert = 0;
    incr = 1;
    koleso1 = koleso2 = koleso3 = koleso_van = 0;
    speed = 0;
    kol_van = 0;
}
//-------------------------------------------------------------
void CBloc::IRQKoleso(const unsigned st_time_koleso, const unsigned &pedal)
{
    st += st_time_koleso;
    FILE *f;
  //  f = fopen("test_bloc.txt", "at"); fprintf(f, "\n%dвагон %d - %d ", kol_van-1, pedal, st_time_koleso); fclose(f);
    timer_convert += st_time_koleso;
    switch(pedal)
    {
      case 0:
        koleso1 ++;
        break;
      case 1:
        koleso2 ++;
        break;
      default : return;
    }
    if(direction == 0)
    {
      if(pedal == 0)
        direction = 1;
      else if(pedal == 1)
        direction = 2;
      sostav.Clear();
    }
    if((direction == 1 && pedal == 0) || (direction == 2 && pedal == 1)) //начало
    {
        begin_koleso << st;
        uk_data_begin_defect << data_defect.GetSize();
    }
    else if((direction == 2 && pedal==0) || (direction == 1 && pedal == 1)) //конец
    {
      int begin;
      if(!begin_koleso.GetSize())
      {
FILE *f = fopen("test_bloc.txt", "at"); fprintf(f, "\nIRQend "); fclose(f);
        IRQEnd();
        return;
      }
      begin_koleso >> begin;
      double time = ((double)st - (double)begin) / 1004.65116279;//сек
      if(!time)
        time = 1;
      speed = from_1pedal_to2 / time; // м/c
      if((speed < 0.5) || (speed > 20))
      {
        Clear();
        return;
      }
      const double speed_cm = speed * 100; //см/с
      const double distance_cm = speed_cm * timer_convert / 1004.65116279;
     // if(distance_cm < 10)
       // speed = speed;

      timer_convert = 0;

      koleso_van += incr;
      const bool new_vagon = !koleso_van;
      const bool new_avt = (koleso1 == koleso2);// && koleso1;
      const double sm = 1 / (speed * 100.0); // секунд на сантиметр
      const double om = sm * 480.0;//отсчётов на сантиметр

      int begin_defect;
      uk_data_begin_defect >> begin_defect;
      int end_defect = data_defect.GetSize();
      int size_defect = end_defect - begin_defect;
      int size_defect_cm = 54*5 + from_opt_to_DLP_obl[0][0] + from_opt_to_DLP_obl[1][5];
      SElement12<int> coordinate_defect;
      for(int i = 0; i < 12; i++)
        coordinate_defect[i] = begin_defect + sm_defect_from_filter + (size_defect * from_opt_to_DLP_obl[direction-1][i]) / size_defect_cm;
      rast += ConvertData(distance_cm);
      int direction2 = (direction == 2)? 0 : 1;
      SElement12<unsigned> coordinate = SElement12<unsigned>(rast)-from_opt_to_DLP_obl[direction2];

      SAxel axel = SAxel(coordinate_defect, om, rast_autoschepka, coordinate, speed,
                         SElement12<int>(0), SElement12<int>(0), SElement12<int>(0), SElement12<int>(0), 0);
      sostav.SetDirection(direction == 2);
      sostav.AddAxel(axel, new_avt, new_vagon);

      rast_autoschepka += distance_cm;
      //if(!koleso_van)
      if(new_vagon)
      {
        incr = 1;
        kol_van ++;
      }
      else if(koleso1 == koleso2)
      {
        incr = -1;
        rast_autoschepka = 0;
      }
    }
}
//-------------------------------------------------------------
bool CBloc::IRQState(SStateBloc &sb, const unsigned &k, CStack<unsigned char> &data_bin)
{
    bool ret = false;
    if(!k && direction)
    {
        sostav.AddDataBin(data_bin);
        data_bin.Clear();
        ret = true;
FILE *f = fopen("test_bloc.txt", "at"); fprintf(f, "\nIRQEND "); fclose(f);
        IRQEnd();
    }
    sb.speed = speed;
    sb.direction = direction;
    sb.kol_of_van = kol_van;
    return ret;
}
//-------------------------------------------------------------
void CBloc::IRQEnd()
{
    Clear();
}
int CBloc::ConvertData(const double &distance_cm)
{
  if(!distance_cm || !data_in.GetSize())
    return 0;
  int begin = (convert_data.GetSize())? convert_data[convert_data.GetSize()-1][1] : 0;
  convert_data << SElementFourth<int>(begin, data_in.GetSize(), distance_cm, 0);
  return distance_cm;
}
//-------------------------------------------------------------
SElement12<int> CBloc::IRQPB(const SElement12<int> &z)
{
    SElement12<int> c, c_ves, current = z;

    for(int i = 0; i < 12; i ++)
    {
      current[i] = filter_ves2[i] << current[i];
      c_ves[i] = filter_ves[i] << current[i];
    }
    c = mas_in[sm_defect_from_filter] - c_ves;
    for(int j = sm_defect_from_filter; j; j--)
      mas_in[j] = mas_in[j-1];
    mas_in[0] = current;

    if(direction)
    {
      data_defect << c;
      data_in << current;
    }
    return current;
}
//-------------------------------------------------------------
bool CBloc::IsNewSostav()
{
    FILE *f;
    if(processor_of_tare == 0)
        return false;
    CProcessorOfTare::eProcessorState state = processor_of_tare->GetSostav(ready_sostav);
    if(state == CProcessorOfTare::psNewSostav)
    {
        DeleteProcessor();
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-END "); fclose(f);
        return true;
    }
    return false;
}
void CBloc::DeleteProcessor()
{
    FILE *f;
    if(processor_of_tare)
    {
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-d "); fclose(f);
        if(processor_of_tare->GetState() == CProcessorOfTare::psWork)
        {
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad1 "); fclose(f);
            processor_of_tare->terminate();
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad2 "); fclose(f);
            if(processor_of_tare->wait(5000))
            {
                f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad3 "); fclose(f);
                delete processor_of_tare;
            }
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad4 "); fclose(f);
        }
        else
        {
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood1 "); fclose(f);
            processor_of_tare->quit();// exit();
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood2 "); fclose(f);
            processor_of_tare->wait(2000);
          //  f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood3 "); fclose(f);
            delete processor_of_tare;
        }
        processor_of_tare = 0;
        time_work_of_processor = 0;
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-dend "); fclose(f);
        return;
    }
    time_work_of_processor = 0;
  //  f = fopen("test_bloc.txt", "at"); fprintf(f, "-derror "); fclose(f);
}
//-------------------------------------------------------------



