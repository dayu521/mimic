#include<stdexcept>
#include "wrap_far_away.h"

WrapFarAway::WrapFarAway()
{

}

void WrapFarAway::doWork()
{
    {
        QMutexLocker L(&dataMutex);
        stMutex.lock();
        st=FAStatus::Running;
        stMutex.unlock();
        output.clear();
        try {
            for(const auto & f :input)
                ms.at(f.method)();
        }  catch (const std::out_of_range &) {
            throw  std::runtime_error("can not execute this method.no such method");
        }
    }
//    QCoreApplication::postEvent(nullptr,nullptr);
    stMutex.lock();
    st=FAStatus::GodJob;
    stMutex.unlock();
}

bool WrapFarAway::setInput(std::vector<Input> p)
{
    if(dataMutex.tryLock()){
        Util::clearAllInput(input.begin(),input.end());
        input=std::move(p);
        index=0;
        dataMutex.unlock();
        stMutex.lock();
        st=FAStatus::Ready;
        stMutex.unlock();
        return true;
    }
    return false;
}

std::vector<Instruction> WrapFarAway::getOutput()
{
    QMutexLocker L(&dataMutex);
    return output;
}

