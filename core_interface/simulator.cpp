#include"simulator.h"
#include"wrap_far_away.h"
#include"freeze_painter.h"

Simulator::Simulator()
{

}

Simulator::Simulator(std::shared_ptr<FreezePainter> fp_, std::shared_ptr<WrapFarAway> fa_):
    animation{fp_},dataSource{fa_}
{

}

Simulator::~Simulator()
{

}

void Simulator::setInputData(std::vector<int> v_)
{
    st=Simulator::Status::UnCertain;
}

void Simulator::produceModelData()
{
    if(dataSource->status()!=FAStatus::GodJob){
        st=Status::UnCertain;
        return ;
    }
    animation->setInstructions(dataSource->getInstructions());
    afterProduceModelData();
    animation->initModelData();
    st=Status::HasModelData;
}

void Simulator::clearModelData()
{
    animation->clearAllModelDatas();
    st=Status::Empty;
}
