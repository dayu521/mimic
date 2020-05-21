#include "rbtree_sim.h"
#include "animation/rbtree_model.h"
#include "datasource/rbdata.h"
#include<QRandomGenerator>

Rbtree::Rbtree():rbtreeModel(std::make_shared<RbtreeModel>()),rbData(std::make_shared<RbData>())
{
    dataSource=rbData;
    animation=rbtreeModel;
}

Rbtree::~Rbtree()
{

}

void Rbtree::convertInput(const std::vector<int> &v)
{
    Input a={RbData::Insert,100};
    for(int i=0;i<a.dataLength;i++)
        a.data[i]=QRandomGenerator::global()->generate()%500;
    rbData->prepareWorks();
    rbData->setInput({a});
//    rbtreeModel->pullOriginInput({a});
}

void Rbtree::prepareReplay()
{
    rbtreeModel->initModelData();
}

void Rbtree::produceModelData()
{
    Simulator::produceModelData();
    rbtreeModel->setXYNodeNumber(rbData->treeMaxNumberX(),rbData->treeMaxNumberY());
    st=Status::HasModelData;
}

