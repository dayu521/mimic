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
    if(f->status()!=FAStatus::Ready){
        emit hasError("无数据,请准备数据");
        return ;
    }
    f->doWork();
    emit completed();
}

void FreeBoy::switchD(std::shared_ptr<FarAway> p_)
{
    f=p_;
}
