#ifdef fuck
#include "demosim.h"
#include<QPixmap>
#include<QWidget>
#include<QPainter>

DemoSim::DemoSim(QString name_)
{
    k=10;
}

DemoSim::~DemoSim()
{
}

void DemoSim::currentSnapshot() const
{
    auto n_=this->n_;
    n_--;
    pix->fill(Qt::white);
    QPainter p_(pix.get());
    p_.setBrush(Qt::red);
    int y=n_/10;
    int x=n_-y*10;
    for(int i=0;i<y;i++)
        for(int j=0;j<10;j++)
            p_.drawRect(j*_diameter,i*_diameter,_diameter,_diameter);
    for(int i=0;i<=x;i++)
        p_.drawRect(i*_diameter,y*_diameter,_diameter,_diameter);
}

//此函数有待重构
QSize DemoSim::calculationMinPixSize()
{
    int w_=_diameter*k;
    int h_=w_;
    return {w_,h_};
}

void DemoSim::nextAction()
{
    if(n_==0)
        pix->fill();
    QPainter p_(pix.get());
    p_.setBrush(Qt::red);
    int y=n_/10;
    int x=n_-y*10;
    p_.drawRect(x*_diameter,y*_diameter,_diameter,_diameter);
}
#endif
