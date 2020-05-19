#include"simulator.h"
#include"datasource/wrap_far_away.h"
#include"animation/abstract_animation.h"

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

void Simulator::setInputData(const std::vector<int> & v_)
{
    st=Simulator::Status::UnCertain;
    convertInput(v_);
}

void Simulator::produceModelData()
{
    if(dataSource->status()!=FAStatus::GodJob){
        st=Status::UnCertain;
        return ;
    }
    animation->setInput(dataSource->getOutput());
    animation->initModelData();
    st=Status::HasModelData;
}

void Simulator::clearModelData()
{
    animation->clearAllModelDatas();
    st=Status::Empty;
}
