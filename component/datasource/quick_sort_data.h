#ifndef QUICKSORTDATA_H
#define QUICKSORTDATA_H

#include"wrap_far_away.h"

class QuickSortData : public WrapFarAway
{
public:
    QuickSortData();

    enum Method:int{
        SortA=0,    //数据结构与算法分析c++描述
        SortB       //算法导论
    };
    // FarAway interface
public:
    virtual void prepareWorks() override;
private:
    void insertSort(std::vector<int> &array_,int left_,int right_);
    std::vector<int>::value_type getMid(std::vector<int> & array_,int left_,int right_);
    void sortA(std::vector<int> &array_, int left_, int right_);
    void sortB(const std::vector<int> &array_, int left_, int right_);

    void sortA()
    {
        auto d=input.at(index);
        array.assign(d.data,d.data+d.dataLength);
        sortA(array,0,array.size()-1);
    }
    void sortB()
    {
        auto d=input.at(index);
        array.assign(d.data,d.data+d.dataLength);
        sortB(array,0,array.size()-1);
    }
private:
    std::vector<int> array{};
};

#endif // QUICKSORTDATA_H
