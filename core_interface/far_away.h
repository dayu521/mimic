#ifndef ABSTRACTDATASOURCE_H
#define ABSTRACTDATASOURCE_H
#include<vector>
#include<atomic>
#include<functional>
#include<unordered_map>

namespace Util {
    template <int N>
    struct InstructionTP
    {
        int action{-1};
        int data[N]{};
        bool isPartOfOther{false};
    };

    struct ModelInput;
    struct SourceOutput;
}
enum struct FAStatus:unsigned int{
    Uncertain,
    Ready,
    Running,
    GodJob,
//    Error
};

using Instruction=Util::InstructionTP<6>;
using ModelInput=Util::ModelInput;
using SourceOutput=Util::SourceOutput;

class FarAway
{
public:
    explicit FarAway();

    FarAway(const FarAway &)=delete ;

    virtual ~FarAway()=0;

    virtual void doWork()=0;

    //method+datalength+data....
    //  1      1     N
    virtual bool pullInputFromModel(std::vector<ModelInput>)=0;

    virtual std::vector<Instruction> getInstructions()=0;

    virtual std::vector<SourceOutput> modelOutputLeft()=0;

    virtual void prepareWorks()=0;

    virtual FAStatus status()
    {
        return st;
    }
protected:
    using Method=std::function<void ()>;
    virtual void registerMethod(int methodKey_,Method m_) final;

protected:
    FAStatus st{FAStatus::Uncertain};
    std::vector<ModelInput> input{};
    int index{0};
    std::vector<Instruction> output{};
    std::unordered_map<int,Method> ms{};
};

#endif // ABSTRACTDATASOURCE_H
