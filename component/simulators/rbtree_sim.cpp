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

void Rbtree::setInputData(std::vector<int> v)
{
    ModelInput a={RbData::Insert,50};
    for(int i=0;i<a.dataLength;i++)
        a.data[i]=QRandomGenerator::global()->generate()%500;
    rbtreeModel->pullModelsFromUserInputFirst();
    rbData->prepareWorks();
    rbData->pullInputFromModel({a});
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

void Rbtree::afterProduceModelData()
{
    animation->completeModelsFromSourceAfter(dataSource->generateModelOutputLeft());
}

