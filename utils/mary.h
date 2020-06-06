#ifndef MARY_H
#define MARY_H
#include<iterator>

#ifdef __cpp_concepts
#include<concepts>
#endif

namespace Util
{
    //这是个数据单向流动的,只需要最后的所有者删除内存,
    struct ModelInput
    {
        struct Element
        {
            int model_fd{-1};
            int abstract_data{-1};
        };

        int simulationType{-1};
        int dataLength{0};
        Element * data{nullptr};
        ModelInput(int method_,int datal_=0):simulationType(method_),dataLength(datal_),data(new Element[datal_]){}
    };

#ifdef __cpp_concepts
    template<typename T>
    concept MyIteratorType=requires (T x)
    {
        std::derived_from<typename std::iterator_traits<T>::iterator_category,std::random_access_iterator_tag>;
        {x->data};
        {delete [] x->data};
    };
#endif

#ifdef __cpp_concepts
    template <MyIteratorType InputIterator>
    inline void
#else
    template <typename InputIterator>
    inline std::enable_if_t<std::is_base_of_v<typename std::iterator_traits<InputIterator>::iterator_category,std::random_access_iterator_tag>>
#endif
    clearAllInput(InputIterator begin_,InputIterator end_)
    {
        for(;begin_!=end_;begin_++)
            delete [] (*begin_).data;
    }

    struct SourceOutput
    {

    };
}
#endif // MARY_H
