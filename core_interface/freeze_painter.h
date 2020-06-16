#ifndef ABSTRACTANIMATION_H
#define ABSTRACTANIMATION_H
#include<vector>
namespace Util
{
    using __width_int=int;
    using __height_int=int;
    using __factor_int=int;

    template <int N>
    struct InstructionTP;

    struct ModelInput;
    struct SourceOutput;
}

using Instruction=Util::InstructionTP<6>;
using ModelInput=Util::ModelInput;
using SourceOutput=Util::SourceOutput;

class QPixmap;

class FreezePainter
{
public:
    explicit FreezePainter(){}
    virtual ~FreezePainter(){}

    //动画的下N帧
    virtual void nextNFrame(int i=0)=0;

    //上N帧
    virtual void previousFrame(int i=0)=0;

    //获取当前动画的快照.例如,更改大小后需要重绘
    virtual void currentSnapshot()const=0;

    //是否播放完成
    virtual bool isBlow()=0;

    //清除模型数据
    virtual void clearAllModelDatas()=0;

    //初始化模型数据
    virtual void initModelData()=0;

    //设置输入模型数据,但某些模型要通过源产生,之后再次补全模型输入
    virtual void pullModelsFromUserInputFirst(std::vector<int>)=0;

    //返回模型输入作为源的输入
    virtual std::vector<ModelInput> generateModelInputForSource()=0;

    //如果源产生了输出,则补全之前的模型数据
    virtual void completeModelsFromSourceAfter(std::vector<SourceOutput>)=0;

    virtual std::tuple<Util::__width_int,Util::__height_int> getSize()=0;

//    //改变元素大小
//    virtual void setElementNewSize(Util::__width_int,Util::__height_int)=0;

    void setInstructions(std::vector<Instruction> v)
    {
        instructions=std::move(v);
        current=0;
        all=instructions.size();
    }

    void setPix(QPixmap * p)
    {
        pix=p;
    }

    bool isRunning()
    {
        return current>0;
    }

    bool canRunning()
    {
        return instructions.size()>0;
    }

protected:
    int current{0};
    int all{0};
    std::vector<Instruction> instructions{};
    QPixmap * pix{};
};

#endif // ABSTRACTANIMATION_H
