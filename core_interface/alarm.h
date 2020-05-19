#ifndef ALARM_H
#define ALARM_H

#include<memory>
#include<QObject>

class WrapFarAway;
class FreeBoy;

class Alarm : public QObject
{  
    Q_OBJECT
    // Simulator interface
public:
    explicit Alarm(QObject * parent_=nullptr);
    virtual ~Alarm();
    void set(std::shared_ptr<WrapFarAway> s_);
    void startD();
signals:
    void start();
    void completed();
private:
    enum struct Status{CanStart,Cannot};
    Status stat{Status::Cannot};
    FreeBoy * fb{nullptr};
    QThread * thread{nullptr};
    std::shared_ptr<WrapFarAway> wfa{};
};

#endif // CSIMULATOR_H


