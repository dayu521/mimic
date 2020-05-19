#ifndef QUICKSORTSIM_H
#define QUICKSORTSIM_H
#include"simulator.h"
#include<QColor>

#ifdef fuck
//数据结构与算法分析c++描述第三版
class QuickSortSimulation:public Simulator
{
public:
    explicit QuickSortSimulation();
    void quickSort(QVector<int> & v){quickSort(v,0,v.size()-1);}
    // Simulator interface
public:
    virtual void makeElementsBig(int factor) override;
    virtual void produceActionData() override;
    virtual void clearActionData() override;
    virtual void prepareReplay() override;
    virtual QString getName() const override;
    virtual void currentSnapshot() const override;
    virtual int actionNumber() const override;
    virtual void nextAction() override;
    virtual QSize calculationMinPixSize() override;
private:
    int width=15;  //元素宽度
    int dHeight=2;    //单位高度.元素高度=单位高度*数值大小
    int fontSize=16;
    int maxElement=0;
    int maxHeight;
    QVector<int> data;
    QVector<int> originData;
    QVector<int> elementContainer;
    struct State
    {
        int left;
        int right;
        int median;
        int toRight;
        int toLeft;
    };

    enum Action{
        Swap,
        ToRight,
        ToLeft,
        DrawLine,
        ChangeColor
    };
    struct Frame
    {
        Action act;
        int data[6];
        /* Swap
         *  0:源下标
         *  1:目的下标
         * ToRight
         *  0:源
         *  1:目的
         * ToLeft
         *  0:源
         *  1:目的
         * DrawLine
         *  0:下标
         *
         */
    };
    QVector<Frame> frames;
    QColor colors[3];
private:
    void quickSort(QVector<int>& vec_,int left_,int right_);
    int median3(QVector<int> & vec_,int left_,int right);

};
#endif
#endif // QUICKSORTSIM_H
