//---------------------------------------------------------------------------
#include "CSostav.h"
#include "csavefiletxt.h"

#define frequency 480
//---------------------------------------------------------------------------
CSostav::CSostav():file_name("")
{
  Clear();
}
//---------------------------------------------
CSostav::CSostav(const CSostav &s)
{
  *this = s;
}
//---------------------------------------------
CSostav::~CSostav()
{
  Clear();
}
//---------------------------------------------------------------------------
CSostav& CSostav::operator=(const CSostav &s)
{
  Clear();
  axels = s.axels;
  vans = s.vans;
  autoschepkas = s.autoschepkas;
  koof_k = s.koof_k;
  koof_u = s.koof_u;
  date_time = s.date_time;
  direction = s.direction;
  activ_van = s.activ_van;
  data = s.data;
  data_bin = s.data_bin;
  file_name = s.file_name;
  uk_autosch = s.uk_autosch;
  porog_defect = s.porog_defect;
  defects = s.defects;
  return *this;
}
//---------------------------------------------
CSostav::CSostav(const QString &fn)
{
    file_name = fn;
    CFileBin f(file_name, CFileBin::ETRead);
    if(!f.GetState())
        throw this;
    try
    {
        date_time = CDateTime(f);
        if(!f.Read(&direction, sizeof(direction), 1)) throw this;
        if(!f.Read(&koof_k, sizeof(koof_k), 1)) throw this;
        if(!f.Read(&koof_u, sizeof(koof_u), 1)) throw this;
        int n;
        SAxel a;
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        for(int i = 0; i < n; i ++)
        {
            if(!f.Read(&a, sizeof(a), 1)) throw this;
            axels << a;
            defects << CDefect(f);
        }
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        for(int i = 0; i < n; i ++)
            vans << CVan(f);
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        for(int i = 0; i < n; i ++)
            autoschepkas << CAutoschepka(f);

        if(!f.Read(&porog_defect, sizeof(porog_defect), 1)) throw this;
    }
    catch(int)
    {
        Clear();
        throw this;
    }

    activ_van = 0;
}
//---------------------------------------------
bool CSostav::LoadData()
{
    CFileBin f(file_name, CFileBin::ETRead);
    bool ret = false;
    if(f.GetState())
    {
        CDateTime d(f);
        SElement12<double> k_a;
        SElement12<double> k_b;
        if(!f.Read(&direction, sizeof(direction), 1)) throw this;
        if(!f.Read(&k_a, sizeof(k_a), 1)) throw this;
        if(!f.Read(&k_b, sizeof(k_b), 1)) throw this;
        int n;
        SAxel a;
        CDefect de;
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        for(int i = 0; i < n; i ++)
        {
            if(!f.Read(&a, sizeof(a), 1)) throw this;
            de = CDefect(f);
          //  axels << a;
        }
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        CVan v;
        for(int i = 0; i < n; i ++)
          v = CVan(f);
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        CAutoschepka au;
        for(int i = 0; i < n; i ++)
          au = CAutoschepka(f);
        SElement12<int> p;
        if(!f.Read(&p, sizeof(p), 1)) throw this;
        ret = data.LoadFromFile(f);
    }
    return ret;
}
//---------------------------------------------
bool CSostav::SaveToFile()
{
    if(file_name != "")
        SaveToFile(file_name);
}
bool CSostav::SaveToFile(const QString &fn)
{
    file_name = fn;
   // CFileBin f(file_name, CFileBin::ETWrite);
    CFileBin f(fn, CFileBin::ETWrite);
    if(!f.GetState())
        return false;
    try
    {
        date_time.SaveToFile(f);
        if(!f.Write(&direction, sizeof(direction), 1)) throw 0;
        if(!f.Write(&koof_u, sizeof(koof_u), 1)) throw 0;
        if(!f.Write(&koof_k, sizeof(koof_k), 1)) throw 0;
        int n = axels.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
        {
            if(!f.Write(&axels[i], sizeof(axels[i]), 1)) throw 0;
            defects[i].SaveToFile(f);
        }
        n = vans.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            vans[i].SaveToFile(f);
        n = autoschepkas.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            autoschepkas[i].SaveToFile(f);
        if(!f.Write(&porog_defect, sizeof(porog_defect), 1)) throw 0;
        data.SaveToFile(f);
    }
    catch(int)
    {
        return false;
    }
    return true;
}
//---------------------------------------------
bool CSostav::SaveToFileTxt(const QString &fn)const
{
    CString str(fn);
    FILE *f = fopen(str.GetPtrChar(), "wb");
    if(!f)
        return false;
    char c[100];
    CString str1 = (direction)? "<" : ">";

    CString str2;//(vans.GetSize());
    int kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
        if(!vans[i].GetLokomotiv())
            kol_van ++;
    fprintf(f, "001\n%s %d\n", str1.GetPtrChar(), kol_van);
    kol_van = 0;

    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        if(vans[i].GetLokomotiv())
            str1 = "Л";
        else
            str1 = CString(++kol_van, 2);
        str2 = CString(vans[i].GetSize(), 2);
        CString str3(vans[i].GetSpeedDouble(axels), 2);
        CString str4 = CString(vans[i].GetTare(axels), 2);
        CString str5 = vans[i].GetDefectFile(defects);
        QString qstr7 = vans[i].GetType();
        fprintf(f, "%s %s %s %s %s -00/00 -00/00 - \n", str1.GetPtrChar(), str2.GetPtrChar(), str3.GetPtrChar(), str4.GetPtrChar(), str5.GetPtrChar());
        //CString str4(vans[i].GetTareFile(axels));
       // fprintf(f, "%s %s %s %s\n", str1.GetPtrChar(), str2.GetPtrChar(), str3.GetPtrChar(), str4.GetPtrChar());
    }
    fclose(f);
    return true;
}
//---------------------------------------------------------------------------
bool CSostav::SaveToFileBin(const QString &fn)
{
    return true;
    CFileBin f(fn, CFileBin::ETWrite);
    if(!f.GetState())
        return false;
    data_bin.SaveToFile(f);
    data_bin.Clear();
    return true;
}
//---------------------------------------------------------------------------
void CSostav::Clear()
{
    date_time  = CDateTime();
    axels.Clear();
    data.Clear();
    data_bin.Clear();
    vans.Clear();
    autoschepkas.Clear();
    defects.Clear();
    direction = false;
    activ_van = 0;
    uk_van = 0;
    uk_autosch = 0;
}
//---------------------------------------------------------------------------
void CSostav::AddDataBin(CStack<unsigned char> &d)
{
  int size = d.GetSize();
  unsigned char *tdata = new unsigned char [size];
  unsigned char *uk = tdata;
  d.ClearPtr();
  for(int i = 0; i < size; i ++, uk ++, d.AddPtr(1))
    *uk = d.GetElementPtr();
  data_bin = CBuffer<unsigned char>(size, tdata);
  delete [] tdata;
  CFileBin f("last.bin", CFileBin::ETWrite);
  if(f.GetState())
    data_bin.SaveToFile(f);
}
//---------------------------------------------------------------------------
void CSostav::SetLastAutoschepka()
{
  int kol_axel_in_autoschepka = axels.GetSize()- uk_autosch;
  if(kol_axel_in_autoschepka)
  {
    autoschepkas << CAutoschepka(uk_autosch);
    autoschepkas[autoschepkas.GetSize()-1].SetSizeAxel(axels.GetSize() - uk_autosch);
  //  autoschepkas[autoschepkas.GetSize()-1].Run(axels, koof_a, koof_b, koof_c, direction);
  }
}
//---------------------------------------------------------------------------
void CSostav::AddAxel(const SAxel &a, const bool &new_autoschepka, const bool &new_van)
{
  axels << a;
  if(new_autoschepka)
  {
    autoschepkas << CAutoschepka(uk_autosch);
    int kol_axel_in_autoschepka = axels.GetSize()- uk_autosch;
    autoschepkas[autoschepkas.GetSize()-1].SetSizeAxel(kol_axel_in_autoschepka);
    uk_autosch = axels.GetSize();
  }
  if(new_van)
  {
    vans << CVan(uk_van);
    vans[vans.GetSize()-1].SetSizeAxel(axels.GetSize() - uk_van);
    uk_van = axels.GetSize();
  }
}
//---------------------------------------------------------------------------
void CSostav::End(CBuffer<SElement12<short> > &d,CBuffer<SElement12<int> > &data_defect)
{
FILE *f = fopen("test_end.txt", "wt"); fprintf(f, "begin "); fclose(f);
    data = d;
    int size = data.GetSize();
    unsigned size_dd = data_defect.GetSize();
f = fopen("test_end.txt", "at"); fprintf(f, "-1 "); fclose(f);
    if(!size || !size_dd)
        return;
f = fopen("test_end.txt", "at"); fprintf(f, "-ves*%d* ", axels.GetSize()); fclose(f);
  //Вес
  for(int i = 0; i < axels.GetSize(); i ++)
  {
    SElement12<unsigned> coordinate = axels[i].coordinate;
    SElement12<int> progib(0), tare(0);
    SElement12<unsigned> coof_tare = param.GetKoofB();//OptionsForm->GetCoofTare();
    for(int j = 0; j < 12; j ++)
    {
      if(!param.GetonoffPb(j))// OptionsForm->GetWorkPB(j))
        continue;
      short max = data[coordinate[j]][j];
      unsigned center = coordinate[j];
      for(unsigned x = coordinate[j] - 10; x < coordinate[j] + 10; x ++)
      {
        if(max < data[x][j])
        {
          max = data[x][j];
          center = x;
        }
      }
      progib[j] = max;
      int t = abs(max - (data[center-20][j] + data[center+20][j])/2);
      tare[j] = t * coof_tare[j];
    }
    axels[i].progib = progib;
    axels[i].tare = tare;
  }
f = fopen("test_end.txt", "at"); fprintf(f, "-3 "); fclose(f);
  // Дефект
  int tek_van = 0;
  defects.Clear();
f = fopen("test_end.txt", "at"); fprintf(f, "-4 "); fclose(f);
  for(int i = 0; i < axels.GetSize(); i ++)
  {
f = fopen("test_end.txt", "at"); fprintf(f, "\n%d", i); fclose(f);
    int tare = 20;
    for(int j = tek_van; j < vans.GetSize(); j ++)
      if(vans[j].IsMy(i))
      {
        tare = (vans[j].GetSize())? vans[j].GetTare(axels) / vans[j].GetSize() : 10;
        tek_van = j;
        break;
      }
f = fopen("test_end.txt", "at"); fprintf(f, " "); fclose(f);
    double om = axels[i].om; // отсчётов / cm
    double speed = axels[i].speed;
    SElement12<int> begin, end;
    int size = GetBeginEndDefect(axels[i].uk_data_defect, om, begin, end);
f = fopen("test_end.txt", "at"); fprintf(f, "-"); fclose(f);

    CBuffer<SElement12<short> > out_real(size);
f = fopen("test_end.txt", "at"); fprintf(f, "1"); fclose(f);
    CBuffer<SElement12<short> > out_fft(frequency/2/5/2);
f = fopen("test_end.txt", "at"); fprintf(f, "2"); fclose(f);
    fft.RunKoleso(data_defect, out_fft, out_real, begin, size);
f = fopen("test_end.txt", "at"); fprintf(f, "3"); fclose(f);
    defects << CDefect(out_real, out_fft, speed, tare);
f = fopen("test_end.txt", "at"); fprintf(f, "4"); fclose(f);
  }
f = fopen("test_end.txt", "at"); fprintf(f, "\n-type*%d* ", vans.GetSize()); fclose(f);
  //Тип вагона
    type_of_van.Clear();
    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        const unsigned axel[2] = {vans[i].GetBegin(), vans[i].GetBegin()+vans[i].GetSize()-1};
        SElement12<int> tv(vans[i].GetSize(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if((vans[i].GetSize() <= 8) && (axel[0] < axels.GetSize()) && (axel[1] < axels.GetSize()))
            for(unsigned i = axel[0] + 1; i <= axel[1]; i ++)
            {
                unsigned ch = i - axel[0] + 1;
                if(ch < 12)
                    tv[ch] = axels[i].coordinate[0] - axels[i-1].coordinate[0];
            }
        type_of_van.AddVan(tv);
    }
f = fopen("test_end.txt", "at"); fprintf(f, "-7 "); fclose(f);
    type_of_van.Run();
f = fopen("test_end.txt", "at"); fprintf(f, "-8 "); fclose(f);
    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        int lokomotiv;
        CString str = type_of_van.GetType(i, lokomotiv);
        vans[i].SetType(str, lokomotiv);
    }
f = fopen("test_end.txt", "at"); fprintf(f, " end"); fclose(f);
}
//---------------------------------------------
int CSostav::GetBeginEndDefect(SElement12<int> &b, const double &om, SElement12<int> &begin, SElement12<int> &end)
{
    const int lishny = static_cast<int>(42.0 * om);
    for(int j = 0; j < 12; j ++)
    {
        begin[j] = b[j] - lishny;
        end[j]   = b[j] + lishny;
        if(begin[j] < 0)
            begin[j] = 0;
    }
    return end[1] - begin[1];
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
void CSostav::SetActiv(CTable *table_vans)
{
    LoadData();
    PaintVan(table_vans);
}
void CSostav::PaintSostav(QList<QString> &list)
{
    list.clear();
    list << date_time.GetTime();
    list << ((direction)? "<-" : "->");
    unsigned k = GetKolLokomotivs();
    list << ((k)? (QString::number(k)) : (""));
    k = GetKolVans();
    list << ((k)? (QString::number(k)) : (""));
    k = GetKolDefect();
    list << ((k)? (QString::number(k)) : (""));
}
//---------------------------------------------
void CSostav::PaintVan(CTable *table_vans)
{
    table_vans->Clear();
    unsigned kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        QList<QString> l;
        l << ((vans[i].GetLokomotiv())? vans[i].GetType() : QString::number(++kol_van));
        l << QString::number(vans[i].GetSize());
        l << vans[i].GetSpeed(axels);
        l << QString::number(vans[i].GetTare(axels));
        unsigned k = vans[i].GetKolDefect(defects, 100, 13);
//        l << ((k)? (QString::number(k)) : (""));
        l << ((k)? ("    X") : (""));
        k = vans[i].GetKolDefect(defects, 100, 14);
//        l << ((k)? (QString::number(k)) : (""));
        l << ((k)? ("    X") : (""));
        table_vans->AddLine(l);
    }
    if(vans.GetSize())
        table_vans->SetChecked(activ_van);
}
void CSostav::SetActivVan(CImage *image_arch, unsigned side, int &dial_arch, const int n)
{    
    if(n >= vans.GetSize())
        return;
    if(n >= 0)
        activ_van = n;
    PicGraphik(image_arch, side, dial_arch);
}
void CSostav::PicGraphik(CImage *image_arch, unsigned side, int &dial_arch, const bool new_pos)
{
    const int size = data.GetSize();
    if(!size)
    {
        image_arch->ClearImage();
        return;
    }
    if((activ_van < 0) || (activ_van >= vans.GetSize()))
    {
        image_arch->ClearImage();
        return;
    }
    if(new_pos)
    {
      int pos_new = vans[activ_van].GetCoordinate(axels, 0) - 100;
      dial_arch = (pos_new >= 0)? pos_new : 0;
    }

    const int sm_coord = dial_arch - vans[activ_van].GetCoordinate(axels, 0);
    const unsigned width = image_arch->GetWidth();
    const double compress_hor = param.GetArchCompressxPb() / 10.0;
    const double compress_vert = (double)param.GetArchCompressyPb() / 10.0;
    unsigned beg = (side)? 6 : 0;
    unsigned end = (side)? 11 : 5;
    for(int n = beg; n <= end; n ++)
    {
        QPointF* ptr_data = image_arch->GetPtrData((side)? n - 6 : n);
        ptr_data[0] = QPointF(0, 0);
        if(!param.GetArchonoffPb(n))
            continue;
        ptr_data[0] = QPointF(1, 1);
        bool *my_van = image_arch->GetBoolMyVan();
        int check = (param.GetArchTogether())? n : 0;
        int begin = vans[activ_van].GetCoordinate(axels, check) - 200 + sm_coord;
        int begin_my_van = vans[activ_van].GetCoordinate(axels, check) - 150;
        int end_my_van = vans[activ_van].GetCoordinate(axels, check, vans[activ_van].GetSize()-1) + 150;
        for(double x = 0; x < width; x += 1.0)
        {
            bool koleso = false;
            int end = (begin + (x+1) * compress_hor);
            int beg = begin + (x) * compress_hor;
            if(beg < 0) beg = 0;
            double pr = (beg < size)? data[beg][n] : 0;
            for(int j = beg+1; j < end; j ++)
                if(j < size)
                    pr += data[j][n];
            if(end - beg)
                pr /= (end - beg);
            unsigned x2 = (!direction)? width-x : x;
            double y2 = (pr) / compress_vert;
            ptr_data[x2+1] = QPointF(x2, y2);
            my_van[x2] = (beg >= begin_my_van && end <= end_my_van);
        }
    }
    image_arch->Paint();
}
void CSostav::PicDefect(CImageDefect *image_arch, unsigned side, unsigned axel)
{
    if(activ_van >= vans.GetSize())
    {
        image_arch->ClearImage();
        return;
    }
    int na1 = vans[activ_van].GetBegin();
    int na2 = axel + na1;
    if(na2 >= defects.GetSize())
    {
        image_arch->ClearImage();
        return;
    }
    unsigned beg = (side)? 6 : 0;
    unsigned end = (side)? 11 : 5;
    const unsigned width = image_arch->GetWidth();
    for(int n = beg; n <= end; n ++)
    {
        unsigned n2 = (side)? n - 6 : n;
        if(!param.GetArchonoffPb(n))
        {
            image_arch->SetDefectSize(n2, 9876);
            continue;
        }
        QPointF* ptr_data1 = image_arch->GetPtrData1(n2);
        QPointF* ptr_data2 = image_arch->GetPtrData2(n2);
        image_arch->SetPorog(n2, defects[na2].GetPorogDefect(n));
        image_arch->SetDefectSize(n2, defects[na2].GetDefectSize(n));
        image_arch->SetTare(n2, vans[activ_van].GetTare(axels, n));
        image_arch->SetPbNumber(n2, n);
        defects[na2].Paint(n, direction, width, ptr_data1, ptr_data2);
    }
    image_arch->Paint();
 /*   Graphics::TBitmap *bitmap = new Graphics::TBitmap;
    bitmap->Width = (image_defect->Width - 0) / 2;
    bitmap->Height = (image_defect->Height -0) / 6;
    TRect Rec1 = TRect(0, 0, bitmap->Width - 1, bitmap->Height - 1);
    int na1 = vans[activ_van].GetBegin();
    int na2 = vans[activ_van].GetActivAxel() + na1;
    for(int n = 0; n < OptionsForm->GetKolDatchik(); n ++)
    {
      int begin_x = (n < 6)? 0 : (image_defect->Width / 2 + 1);
      int begin_y = (n < 6)? image_defect->Height / 6 * n : image_defect->Height / 6 * (n-6);
      int end_x = (n < 6)? image_defect->Width / 2 - 1 : (image_defect->Width - 1);
      int end_y = (n < 6)? image_defect->Height / 6 * (n+1) - 1 : image_defect->Height / 6 * (n-6+1) - 1;
      defects[na2].Paint(bitmap, n, color_font, color_grid, color_dlp[n], direction);
      TRect Rec2 = TRect(begin_x, begin_y, end_x, end_y);
      image_defect->Canvas->CopyRect(Rec2, bitmap->Canvas, Rec1);
    }*/
}
//---------------------------------------------------------------------------
unsigned CSostav::GetSizeVan(const unsigned &n)const
{
  return (n>=vans.GetSize())? 0 : vans[n].GetSize();
}
//---------------------------------------------------------------------------
unsigned CSostav::GetKolVans()
{
    unsigned ret = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
      if(!vans[i].GetLokomotiv())
        ret ++;
    return ret;
}
//---------------------------------------------------------------------------
unsigned CSostav::GetKolDefect()
{
    unsigned ret = 0;
    for(unsigned i = 0; i < defects.GetSize(); i ++)
        if(defects[i].GetKolDefect())
            ret ++;
    return ret;
}
bool CSostav::GetDefect(const unsigned n, const unsigned a, const unsigned d)
{
    if(n >= vans.GetSize())
        return false;
    return vans[n].GetKolDefect(defects, a, d) != 0;
}
//---------------------------------------------
/*void CSostav::SetPS(const QString &ps_new1, const QString &ps_new2, const unsigned &n)
{
  if(vans.GetSize() && (n < vans.GetSize()))
    vans[n].SetPS(SElementDual<QString>(ps_new1, ps_new2));
}
//---------------------------------------------
QString CSostav::GetPS(const unsigned &n, const unsigned &n2)const
{
  if(n2>1 || n>=vans.GetSize())
    return "";
  return vans[n].GetPS(n2);
}
//---------------------------------------------------------------------------
int CSostav::GetTare(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
    return 0;
  //if(n>=vans.GetSize())
  //  return 0;
  //return vans[n].GetTare(axels, naxel, ndat);
}
//---------------------------------------------------------------------------
int CSostav::GetTareAll(bool lok)const
{
  int ret = 0;
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    if(!lok && GetLokomotiv(i))
      continue;
    ret += vans[i].GetTare(axels, 100, 100);
  }
  return ret;
}
//---------------------------------------------------------------------------
int CSostav::GetTareDatAll(int dat, bool lok)const
{
  int ret = 0;
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    if(!lok && GetLokomotiv(i))
      continue;
    ret += vans[i].GetTare(axels, 100, dat);
  }
  return ret / 1000;
}
//---------------------------------------------------------------------------
QString CSostav::GetTareStr(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetTareStr(axels, naxel, ndat);
}
//---------------------------------------------------------------------------
int CSostav::GetProgib(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetProgib(axels, naxel, ndat);
}*/
//---------------------------------------------------------------------------
double CSostav::GetSpeedDouble(const unsigned &n) const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetSpeedDouble(axels);
}
//---------------------------------------------
QString CSostav::GetSpeed(const unsigned &n) const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetSpeed(axels);
}
//---------------------------------------------------------------------------
bool CSostav::GetLokomotiv(const unsigned &n)const
{
  if(n >= vans.GetSize())
    return false;
  return (vans[n].GetLokomotiv())? true : false;
}
//---------------------------------------------------------------------------
void CSostav::GetData(CBuffer<SElement12<short> > &d)
{
  d = data;
}
//---------------------------------------------------------------------------
bool CSostav::GetAxel(const unsigned &n, SAxel &a)
{
  if(n >= axels.GetSize())
    return false;
  a = axels[n];
  return true;
}
void CSostav::Print(QPrinter *printer)//QPaintDevice *printer)
{
    QPainter painter(printer);
    QRect rect(painter.viewport());

    double bw = rect.width();// GetDeviceCaps(prn->Handle, HORZRES);
    double bh = rect.height();// GetDeviceCaps(prn->Handle, VERTRES);
    double width = 200;//GetDeviceCaps(prn->Handle, HORZSIZE);
    double height = 285;//GetDeviceCaps(prn->Handle, VERTSIZE);
    double mmw = bw / width;
    double mmh = bh / height;
    double mmpole = width / 21;
    QFont font(painter.font());
//    font.setBold(true);

    font.setPointSize(16);
    painter.setFont(font);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);



    unsigned kol_van = GetNumberOfVan();
    unsigned kol_van2 = 0;
    int y_line = 30;
    int y_step = 6;
    bool new_page = true;
    int kol_page = 1;
    for(unsigned i = 0; i < kol_van; i ++)
    {
        if(GetLokomotiv(i))
            continue;
        kol_van2 ++;
    }
    painter.drawText(mmpole*6*mmw, 7*mmh, QString::fromLocal8Bit("Протокол взвешивания подвижного состава"));
    painter.drawText(mmpole*3*mmw, 15*mmh, GetDate() + " " + GetTime());
    font.setPointSize(14); painter.setFont(font);
    painter.drawText(mmpole*3*mmw, 22*mmh, QString::fromLocal8Bit("Количество вагонов: ") +
                     QString::number(kol_van2) +
                     QString::fromLocal8Bit(". Общая масса ") +
                     //QString::number(GetTareAll()/1000) +
                     QString::fromLocal8Bit(" т."));
    kol_van2 = 0;
    font.setPointSize(11); painter.setFont(font);
    for(unsigned i = 0; i < kol_van; i ++)
    {
        if(GetLokomotiv(i))
            continue;
        kol_van2 ++;
        if(new_page)//шапка
        {
            painter.drawText(mmpole*2*mmw, y_line*mmh, QString::fromLocal8Bit("№"));
            painter.drawText((mmpole*7)*mmw, y_line*mmh, QString::fromLocal8Bit("Скор., км/ч"));
            painter.drawText(mmpole*10*mmw, y_line*mmh, QString::fromLocal8Bit("Вес 1 сторона,т"));
            painter.drawText(mmpole*14*mmw, y_line*mmh, QString::fromLocal8Bit("Вес 2 сторона,т"));
            painter.drawText(mmpole*18*mmw, y_line*mmh, QString::fromLocal8Bit("Вес,т"));
            PaintCells(painter, y_line-y_step/2, y_line+y_step/2, mmpole, mmw, mmh);
            painter.drawText(mmpole*20*mmw, 280*mmh, QString::number(kol_page++));
            new_page = false;
        }
        QString str_speed = GetSpeed(i);
        //QString str_tare = GetTareStr(i, 100, 100);
      //  QString str_tare1 = GetTareStr(i, 100, 103);
      //  QString str_tare2 = GetTareStr(i, 100, 104);
        y_line+=y_step;
        painter.drawText(mmpole*2*mmw,  y_line*mmh, QString::number(kol_van2));
        painter.drawText(mmpole*7*mmw, y_line*mmh, str_speed);
      //  painter.drawText(mmpole*10*mmw, y_line*mmh, str_tare1);
     //   painter.drawText(mmpole*14*mmw, y_line*mmh, str_tare2);
      //  painter.drawText(mmpole*18*mmw, y_line*mmh, str_tare);
        PaintCells(painter, y_line-y_step/2, y_line+y_step/2, mmpole, mmw, mmh);
        if(y_line > 277)
        {
            printer->newPage();
            y_line = 10;
            new_page = true;
        }
    }
}
QString CSostav::GetTypeActivVan()
{
    if(activ_van >= vans.GetSize())
        return "";
    return vans[activ_van].GetType();
}
void CSostav::PaintCells(QPainter &painter, const int &y1, const int &y2, double &mmpole, double &mmw, double &mmh)
{
    painter.drawLine((mmpole*2 - 2)*mmw,  (y1-1)*mmh, (mmpole*20 - 2)*mmw, (y1-1)*mmh);
    painter.drawLine((mmpole*2 - 2)*mmw,  (y2-1)*mmh, (mmpole*20 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*2 - 2)*mmw,  (y1-1)*mmh, (mmpole*2 - 2)*mmw,  (y2-1)*mmh);
    painter.drawLine((mmpole*7 - 2)*mmw,  (y1-1)*mmh, (mmpole*7 - 2)*mmw,  (y2-1)*mmh);
    painter.drawLine((mmpole*10 - 2)*mmw, (y1-1)*mmh, (mmpole*10 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*14 - 2)*mmw, (y1-1)*mmh, (mmpole*14 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*18 - 2)*mmw, (y1-1)*mmh, (mmpole*18 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*20 - 2)*mmw, (y1-1)*mmh, (mmpole*20 - 2)*mmw, (y2-1)*mmh);
}
