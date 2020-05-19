#ifndef RBTREE_H
#define RBTREE_H
#include"simulator.h"
#include"datasource/far_away.h"

class RbtreeModel;
class RbData;

class Rbtree :public Simulator
{
public:
    explicit Rbtree();
    virtual ~Rbtree();
    // Simulator interface
public:
    virtual void convertInput(const std::vector<int> &) override;
    virtual void prepareReplay() override;
    virtual void produceModelData() override;
private:
    std::shared_ptr<RbtreeModel> rbtreeModel;
    std::shared_ptr<RbData> rbData;
};

#endif // RBTREE_H
