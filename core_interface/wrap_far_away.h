#ifndef RBTREEDATASOURCE_H
#define RBTREEDATASOURCE_H

#include"far_away.h"
#include<memory>
#include<QMutex>

//包装然后放到线程里执行
class WrapFarAway : public FarAway
{
public:
    explicit WrapFarAway();
    ~WrapFarAway()=default;
    // FarAway interface
public:
    virtual void doWork() final override;
    virtual bool setInput(std::vector<Input>) override;
    virtual std::vector<Instruction> getOutput() override;
    virtual FAStatus status() override
    {
        QMutexLocker L{&stMutex};
        return FarAway::status();
    }
private:
    QMutex dataMutex{};
    QMutex stMutex{};
};

#endif // RBTREEDATASOURCE_H

