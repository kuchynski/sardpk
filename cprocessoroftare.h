#ifndef CPROCCESOROFTARE_H
#define CPROCCESOROFTARE_H

#include <QThread>
#include "CSostav.h"
#include "CBuffer.h"

class CProcessorOfTare : public QThread
{
    Q_OBJECT
public:
    enum eProcessorState {psWork, psNoSostav, psNewSostav, psEnd};
private:
    CSostav sostav;
    eProcessorState state;
    CBuffer<SElement12<int> > tdata;
    CBuffer<SElement12<int> > data_defect;
    CStack12 data_tmp;
    CBuffer<SElement12<short> > data;
    CStackFourth convert_data;

    bool ConvertDataEnd1();
    void ConvertDataEnd2();

public:
    explicit CProcessorOfTare(CSostav &s, CStack12 &d, CStack12 &dd, CStackFourth &cd, QObject *parent = 0);
    ~CProcessorOfTare();

    void run();
    eProcessorState GetSostav(CSostav &s);
    eProcessorState GetState()const {return state;}

signals:

public slots:

};

#endif // CPROCCESOROFTARE_H
