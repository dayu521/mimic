#ifndef RBTREE_H
#define RBTREE_H
#include"simulator.h"
#include"far_away.h"

class RbtreeModel;
class RbData;

class Rbtree :public Simulator
{
public:
    explicit Rbtree();
    virtual ~Rbtree();
    // Simulator interface
public:
    virtual void setInputData(const std::vector<int> &) override;
    virtual void prepareReplay() override;
    virtual void produceModelData() override;
private:
    std::shared_ptr<RbtreeModel> rbtreeModel;
    std::shared_ptr<RbData> rbData;

    // Simulator interface
private:
    virtual void afterProduceModelData() override;
};


#endif // RBTREE_H
