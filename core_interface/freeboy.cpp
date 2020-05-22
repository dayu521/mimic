#include "freeboy.h"
#include"far_away.h"

FreeBoy::FreeBoy(QObject *parent_):QObject(parent_)
{

}

FreeBoy::~FreeBoy()
{

}

void FreeBoy::run()
{
    f->doWork();
    emit completed();
}

void FreeBoy::switchD(std::shared_ptr<FarAway> p_)
{
    f=p_;
}
