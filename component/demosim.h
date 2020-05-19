#ifndef RBTREESIM_H
#define RBTREESIM_H
#include"simulator.h"

class DemoSim //: public Simulator
{
public:
    explicit DemoSim();
    ~DemoSim();
    // Simulator interface
public:
private:
    int _diameter =40;   //节点直径
    int _radius = _diameter / 2;    //半径
    int _nodeLineHeight = 3 * _diameter / 2;     //垂直距离
    int _fontSize=_radius;      //字体大小
    int k;
    int mm=100;
    int n_=100;
};
#endif // RBTREESIM_H
