#ifndef CButtonsCheck_H
#define CButtonsCheck_H

#include "cchecks.h"

class CButtonsCheck : public CChecks
{
public:
    explicit CButtonsCheck(QWidget *parent = 0) : CChecks(parent){}

    void SetChecked(const unsigned &n);
    void SetNotChecked();
private:
protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void leaveEvent(QEvent *);
signals:
public slots:

};

#endif // CButtonsCheck_H
