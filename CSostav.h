//---------------------------------------------------------------------------
#ifndef CSostavH
#define CSostavH

//#include <Grids.hpp>
#include "CVan.h"
#include "CAutoschepka.h"
#include "CDateTime.h"
//#include "UOptions.h"
#include "CBuffer.h"
#include "CTypeVan.h"
#include <math.h>
#include "cimage.h"
#include <QtGui>
#include<QDial>
#include "ctable.h"
#include "CFFT.h"
#include "CDefect.h"
#include <windows.h>
#include "cimagedefect.h"
//---------------------------------------------
using namespace std;

class CSostav
{
    QString file_name;
    CBuffer<SElement12<short> > data;
    CBuffer<unsigned char > data_bin;
    CDateTime date_time;
    bool direction;
    SElement12<double> koof_k;
    SElement12<double> koof_u;
    CStack <SAxel> axels;    
    CStack <CVan> vans;    
    CStack <CAutoschepka> autoschepkas;
    SElementFourth<int> porog_defect;
    CStack<CDefect> defects;
    CFFT fft;

    unsigned activ_van;
    int uk_van, uk_autosch;    

    int GetBeginEndDefect(SElement12<int> &b, const double &om, SElement12<int> &begin, SElement12<int> &end);
    inline int GetDlpPos(const int &d) const;
    void PaintCells(QPainter &painter, const int &y1, const int &y2, double &mmpole, double &mmw, double &mmh);
    unsigned GetKolDefect();
  public:
    CSostav();
    CSostav(const CSostav &s);
    CSostav(const QString &fn);
    CSostav& operator=(const CSostav &s);
    ~CSostav();

    bool SaveToFile();
    bool SaveToFile(const QString &fn);
    bool SaveToFileTxt(const QString &fn)const;
    bool SaveToFileBin(const QString &fn);
    void Clear();
    void AddAxel(const SAxel &a, const bool &new_autoschepka, const bool &new_van);
    void SetLastAutoschepka();
    void End(CBuffer<SElement12<short> > &d, CBuffer<SElement12<int> > &data_defect);

    void PaintSostav(QList<QString> &list);
    void SetActiv(CTable *table_vans);
    void SetDeActiv();
    void SetActivVan(CImage *image_arch, unsigned side, int &dial_arch, const int n);
    unsigned GetActivVan()const {return activ_van;}
    void PaintVan(CTable *table_vans);
    void PicGraphik(CImage *image_arch, unsigned side, int &dial_arch, const bool new_pos = true);
    void PicDefect(CImageDefect *image_arch, unsigned side, unsigned axel);

    int GetNumberOfVan()const {return vans.GetSize();}
    int GetNumberOfAxels()const {return axels.GetSize();}
    int GetSizeData()const {return data.GetSize();}

    void SetDirection(const bool &d){ direction = d;}
    bool GetDirection()const {return direction;}

    bool GetDefect(const unsigned n, const unsigned a, const unsigned d);
    CDateTime GetDateTime()const {return date_time;}
    QString GetDate()const {return date_time.GetDate();}
    QDate GetQDate()const {return date_time.GetQDate();}
    QString GetTime()const {return date_time.GetTime();}
  //  SElementFourth<double> GetKoof_A()const {return koof_a;}
  //  SElementFourth<double> GetKoof_B()const {return koof_b;}
 //   SElementFourth<int> GetKoof_C()const {return koof_c;}
  //  void SetKoofA(const SElementFourth<double>& a) {koof_a = a;}
  //  void SetKoofB(const SElementFourth<double>& b) {koof_b = b;}
  //  void SetKoofC(const SElementFourth<int>& c) {koof_c = c;}
//    bool GetVan(int n, CVan &v);
    bool GetAxel(const unsigned &n, SAxel &a);
    void GetData(CBuffer<SElement12<short> > &d);

 //   void SetPS(const QString &ps_new1, const QString &ps_new2, const unsigned &n);
  //  QString GetPS(const unsigned &n, const unsigned &n2) const;
    unsigned GetSizeVan(const unsigned &n)const;
    void AddDataBin(CStack<unsigned char> &d);


//    int GetTare(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const;
//    QString GetTareStr(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const;
 //   int GetProgib(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const;
    double GetSpeedDouble(const unsigned &n) const;
    QString GetSpeed(const unsigned &n) const;
    bool GetLokomotiv(const unsigned &n)const;
 //   int GetTareAll(bool lok = false)const;
  //  int GetTareDatAll(int dat, bool lok = false)const;
    unsigned GetKolLokomotivs(){return vans.GetSize() - GetKolVans();}
    unsigned GetKolVans();
    QString GetTypeActivVan();
    QString GetFileName()const {return file_name;}
    bool LoadData();
    void Print(QPrinter *printer);
};
//---------------------------------------------------------------------------
#endif
