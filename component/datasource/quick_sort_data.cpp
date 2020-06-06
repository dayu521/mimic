#include "quick_sort_data.h"

QuickSortData::QuickSortData()
{
    registerMethod(QuickSortData::SortA,[this](){sortA();});
    registerMethod(QuickSortData::SortA,[this](){sortB();});
}

void QuickSortData::prepareWorks()
{

}

void QuickSortData::insertSort(std::vector<int> &array_, int left_, int right_)
{
    for(int i=left_;i<=right_;i++){
        auto temp=array_[i];
        auto j=i;
        for(;j>left_&&temp<array_[j-1];j--){    //temp会错误写为array_[j]
            array_[j]=array_[j-1];
        }
        array_[j]=temp;
    }
}

std::vector<int>::value_type QuickSortData::getMid(std::vector<int> &array_, int left_, int right_)
{
    //进入三中值选择
    int mid=(left_+right_)/2;
    if(array_[left_]>array_[mid])
        std::swap(array_[left_],array_[mid]);
    if(array_[left_]>array_[right_])
        std::swap(array_[left_],array_[right_]);
    if(array_[mid]>array_[right_])
        std::swap(array_[mid],array_[right_]);
    std::swap(array_[mid],array_[right_-1]);
    return array_[right_-1];
}

void QuickSortData::sortA(std::vector<int> &array_, int left_, int right_)
{
    if(right_-left_>8){
        //多于8个才进入快排
        auto midValue=getMid(array_,left_,right_);
        auto il=left_,jr=right_-1;  //auto il=left_+1,jr=right_-2
        while (true) {
            while (array_[++il]<midValue)//当采用上述注释时,需要后缀形式,但这样会在array_[il]=array_[jr]=midValue时造成无限循环
                ;                       //因此,此处循环和下面循环必须用前缀形式才可保持这样的结构
            while (array_[--jr]>midValue)       //这里都没有使用>=或<=,是为了在遇到大量相同元素时停下来进行交换,使之后的分割减少不均匀情况
                ;
            if(il<jr)
                std::swap(array_[il],array_[jr]);
            else
                break;
        }
        std::swap(array_[il],array_[right_-1]);
        sortA(array_,left_,il-1);
        sortA(array_,il+1,right_);

    }else
        insertSort(array_,left_,right_);
}

void QuickSortData::sortB(const std::vector<int> & array_, int left_, int right_)
{

}
