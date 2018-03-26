#ifndef CBUTTONS_H
#define CBUTTONS_H

#include "cchecks.h"

class CButtons : public CChecks
{
public:
    explicit CButtons(QWidget *parent = 0) : CChecks(parent){}
private:
protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void leaveEvent(QEvent *);
signals:

public slots:

};

#endif // CBUTTONS_H
