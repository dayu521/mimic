#ifndef CONVENIENCE_H
#define CONVENIENCE_H
#include<tuple>

#include<memory>
#include<functional>
#include<vector>
#include<unordered_map>
#include"loki_wrap.h"

namespace Util {

    /******************************************************/

    template<int n,typename T1,typename ...T>
    struct FuckAll :FuckAll<n-1,T1,T1,T...>
    {};

    template<typename T1, typename ...T>
    struct FuckAll<0,T1,T...>
    {
        typedef  std::tuple<T...> Fa;
    };

    /**********************************
     *简化函数返回值std::tuple声明.
     *当存储多个相同类型的tuple时需要写多个类型参数,例如存储3个int类型
     *   std::tuple<int,int,int> f();
     *于是可以替换写作为:
     *   TupleForArray<int,3> f();
     *有些情况下可以decltype(auto)
     *****************************************/
    template <typename T,int n>
    using TupleWrapArray=typename FuckAll<n,T>::Fa;

    /*****************************************************/

    template <typename ...T>
    std::tuple<T...> __putArray(T...arr)
    {
        return std::make_tuple(arr...);
    }

    template<typename T>
    struct DifferentType{};

    template <int,typename ArrType,typename ...T>
    constexpr decltype (auto) __putArray(DifferentType<char>,ArrType,const T &...arr)
    {
        return std::make_tuple(arr...);
    }

    template <int size,typename ArrType,typename ...T>
    constexpr decltype (auto) __putArray(DifferentType<int>,ArrType arr,const T &... t)
    {
        typedef typename std::conditional<size-1,DifferentType<int>,DifferentType<char>>::type Fk;
        return __putArray<size-1,ArrType>(Fk(),arr,arr[size-1],t...);
    }

    /****************************
     * 简化构造数组时需要分别传递每个数组元素.
     * 当传递 int a[]={1,2,3,4,5} 中的5个元素给std::tuple时:
     *      std::make_tuple(a[0],a[1],a[2],a[3],a[4]);
     * 于是可替换为
     *      make_TupleForArray(a);
     ************************************/
    template <typename T,std::size_t N>
    constexpr decltype (auto) make_TupleForArray(T (&arr)[N])
    {
        return __putArray<N,T *>(DifferentType<int>(),static_cast<T *>(arr));
    }

    /******************************
     * 注册派生类型,从而以基类指针访问各自不同虚函数实现
     * ******************************/
    template <typename Base,typename ...Derived>
    struct ObjManager
    {
        using Fck=typename Loki::TL::NoDuplicates<typename Util::MakeTypelist<Derived...>::Result>::Result;
        using Crt=std::function<std::shared_ptr<Base>()>;

        //you can ignore this
        static const int CustomDefaultValue;

        static constexpr int NumberOfType=Loki::TL::Length<Fck>::value;

        template <typename T=Loki::NullType>
        static constexpr int FdOfType=Loki::TL::IndexOf<Fck,T>::value;

        std::shared_ptr<Base> GetObj(int typeFd_);

        template<typename H>
        std::enable_if_t<Loki::TL::IndexOf<Fck,H>::value!=-1> createObj()
        {
            int type_=FdOfType<H>;
    //        if(objRegistersContainer.find(type_)==objRegistersContainer.end())
    //            throw std::runtime_error("Can not create Obj,you need to register this kind of type first");
    //        else if(objRegistersContainer[type_].first)
    //            ;
            if(objRegistersContainer[type_].first)
                ;
            else{
                vecSim.push_back(((objRegistersContainer[type_]).second)());
                objRegistersContainer[type_].first=true;
            }
        }
        void createAll()
        {
            for(int i=0;i<NumberOfType;i++)
                GetObj(i);
        }

        template<int typeFd>
        std::enable_if_t<!std::is_same_v<typename Loki::TL::TypeAtNonStrict<Fck,typeFd>::Result,Loki::NullType>>
        createObj()
        {
            createObj<typename Loki::TL::TypeAt<Fck,typeFd>::Result>();
        }

        template<typename UU=Fck>
        std::enable_if_t<std::is_base_of_v<Base,typename UU::Head>> registerCreateFunction()
        {
            using namespace Loki::TL;
            objRegistersContainer.insert({IndexOf<Fck,typename UU::Head>::value,std::make_pair<bool,Crt>(false,[](){return std::make_shared<typename UU::Head>();})});
            registerCreateFunction<typename UU::Tail>();
        }

private:
        template<typename UU>
        std::enable_if_t<std::is_same_v<Loki::NullType,UU>> registerCreateFunction()
        {
        }

        void createObj(int typeFd_)
        {
            vecSim.push_back(((objRegistersContainer[typeFd_]).second)());
            objRegistersContainer[typeFd_].first=true;
        }

        //key=type,value=isAlreadyCreated,createFn
        std::unordered_map<int,std::pair<bool,Crt>> objRegistersContainer{};

        //ptr to Base
        std::vector<std::shared_ptr<Base>> vecSim{};
    };

    template <typename T,typename ...TT>
    const int ObjManager<T,TT...>::CustomDefaultValue=0;

    template<typename Base, typename ...Derived>
    std::shared_ptr<Base>  ObjManager<Base, Derived...>::GetObj(int typeFd_)
    {
        if(objRegistersContainer.find(typeFd_)==objRegistersContainer.end())
            throw std::runtime_error("Can not create Obj,you need to register this kind of type first");
        else if(objRegistersContainer[typeFd_].first)
            return vecSim[typeFd_];
        else{
            createObj(typeFd_);
            return vecSim[typeFd_];
        }
    }

}

#endif // CONVENIENCE_H
