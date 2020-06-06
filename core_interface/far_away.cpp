#include "far_away.h"
#include "mary.h"
#include<stdexcept>
FarAway::FarAway()
{

}

FarAway::~FarAway()
{
    Util::clearAllInput(input.begin(),input.end());
}

void FarAway::registerMethod(int methodKey_, FarAway::Method m_)
{
    if(!ms.insert(std::make_pair(methodKey_,m_)).second)
        throw std::runtime_error(" methodKey_ duplicate,please use unique key");
}
