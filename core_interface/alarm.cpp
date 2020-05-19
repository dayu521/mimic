
#include "alarm.h"
#include"wrap_far_away.h"
#include"freeboy.h"

Alarm::Alarm(QObject *parent_):QObject(parent_)
{
    fb=new FreeBoy();
    thread=new QThread(this);
    fb->moveToThread(thread);
    connect(this,&Alarm::start,fb,&FreeBoy::run);
    connect(thread,&QThread::finished,fb,&QObject::deleteLater);
    connect(fb,&FreeBoy::completed,this,&Alarm::completed);
    thread->start();
}

Alarm::~Alarm()
{
    thread->exit();
    thread->wait();
}

void Alarm::set(std::shared_ptr<WrapFarAway> s_)
{
    if(s_.get()==wfa.get())
        return ;
    wfa=s_;
    fb->switchD(wfa);
    stat=Status::CanStart;
}

void Alarm::startD()
{
    if(stat==Status::Cannot)
        throw std::runtime_error("can not start,please set WFA first");
    emit start();
}

