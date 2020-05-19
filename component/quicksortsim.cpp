#ifdef fuck
#include "quicksortsim.h"
#include<QWidget>
#include<QPainter>
#include<QDebug>

QuickSortSimulation::QuickSortSimulation()
{

}

void QuickSortSimulation::makeElementsBig(int factor)
{
    dHeight+=factor/5;
    width+=factor;
    fontSize+=factor/2;
    maxHeight=maxElement*dHeight+50;
}

void QuickSortSimulation::produceActionData()
{
    for(int i=0;i<80;i++){
        auto x=rand()%200;
        originData<<x;
        data<<x;
        if(maxElement<x)
            maxElement=x;
    }
    quickSort(data);
    elementContainer.append(originData);
    maxHeight=maxElement*dHeight+50;
}

void QuickSortSimulation::clearActionData()
{
    frames.clear();
    originData.clear();
    data.clear();
    elementContainer.clear();
}

void QuickSortSimulation::prepareReplay()
{
    pix->fill();
}

QString QuickSortSimulation::getName() const
{
    return QStringLiteral("快速排序");
}

void QuickSortSimulation::currentSnapshot() const
{
    pix->fill();
    QPainter p(pix.get());
//    auto f_=p.font();
//    f_.setPixelSize(fontSize);
//    p.setFont(f_);
    p.setBrush(Qt::red);
    for(int i=0;i<elementContainer.size();i++){
        p.drawRect(i*width+1,maxHeight-elementContainer[i]*dHeight,width-2,elementContainer[i]*dHeight);
        p.drawText(i*width,maxHeight,QString::number(elementContainer[i]));
    }

}

int QuickSortSimulation::actionNumber() const
{
    return 10;
}

void QuickSortSimulation::nextAction()
{

}

QSize QuickSortSimulation::calculationMinPixSize()
{
//    auto x=0;
//    std::for_each(elementContainer.begin(),elementContainer.end(),[&x](int e){x=e>x?e:x;});
    return {width*elementContainer.size(),maxHeight+50};
}

void QuickSortSimulation::quickSort(QVector<int> &vec_, int left_, int right_)
{
    //三个或三个以上进行快排
    if(left_<=right_-9){
        auto pivot_Value=median3(vec_,left_,right_);
        int i=left_+1,j=right_;
        while (true) {
            while (vec_[++i]<pivot_Value) {
//                i++;
            }
            while (vec_[--j]>pivot_Value) {
//                j--;
            }
            if(i<j)
                std::swap(vec_[i],vec_[j]);
            else
                break;
        }
        std::swap(vec_[left_+1],vec_[j]);
        quickSort(vec_,left_,j-1);
        quickSort(vec_,j+1,right_);
    }else{
        //插入排序
        for(int i=left_+1;i<=right_;i++){
            auto temp_=vec_[i];
            int j=i;
            for(;j>left_&&vec_[j-1]>temp_;j--){
                vec_[j]=vec_[j-1];
            }
            vec_[j]=temp_;
        }
    }
}

int QuickSortSimulation::median3(QVector<int> &vec_, int left_, int right_)
{
    auto mid_=(left_+right_)/2;
    //先找最大值
    if(vec_[right_]<vec_[mid_])
        std::swap(vec_[right_],vec_[mid_]);
    if(vec_[right_]<vec_[left_])
        std::swap(vec_[right_],vec_[left_]);
    //再判断最小值和中间值
    if(vec_[mid_]<vec_[left_])
        std::swap(vec_[mid_],vec_[left_]);
    std::swap(vec_[left_+1],vec_[mid_]);
    return vec_[left_+1];
}

#endif
