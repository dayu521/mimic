#include "quick_sort_data.h"

QuickSortData::QuickSortData()
{
    registerMethod(QuickSortData::SortA,[this](){sortA();});
    registerMethod(QuickSortData::SortA,[this](){sortB();});
}

void QuickSortData::prepareWorks()
{

}

void QuickSortData::insertSort(std::vector<int> &array_)
{
    for(decltype (array_.size()) i=0;i<array_.size();i++){
        auto temp=array_[i];
        auto j=i;
        for(;j>0&&temp<array_[j-1];j--){
            array_[j]=array_[j-1];
        }
        array_[j]=temp;
    }
}

std::vector<int>::value_type QuickSortData::getMid(std::vector<int> &array_, int left_, int right_)
{
    int mid=(left_+right_)/2;
    if(array_[mid]<array_[left_])
        std::swap(array_[mid],array_[left_]);
    if(array_[mid]>array_[right_])
        std::swap(array_[mid],array_[right_]);
    if(array_[left_]>array_[right_])
        std::swap(array_[left_],array_[right_]);
    std::swap(array_[mid],array_[right_-1]);
    return array_[right_-1];
}

void QuickSortData::sortA(std::vector<int> &array_, int left_, int right_)
{
    if(right_-left_>8){
        auto midValue=getMid(array_,left_,right_);
        auto il=left_,jr=right_;
        while (true) {
            while (array_[++il]<midValue)
                ;
            while (array_[--jr]>midValue)
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
        insertSort(array_);
}

void QuickSortData::sortB(const std::vector<int> & array_, int left_, int right_)
{

}
