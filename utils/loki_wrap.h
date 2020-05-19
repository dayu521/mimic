#ifndef LOKI_WRAP_H
#define LOKI_WRAP_H

#ifdef USE_GIT_SUBMODULE_LOKI
#include"loki/Typelist.h"
#else
#include<loki/Typelist.h>
#endif
namespace Util {

    /********************
     * Modern C++ Design
     * Reading Chapter 3 "Typelist" first,
     * if you want to know about the details of some templates
     * ***************/
    template <typename ...T>
    struct MakeTypelist{
        typedef Loki::NullType Result;
    };

    template <typename T,typename ...TT>
    struct MakeTypelist<T,TT...>{
        typedef Loki::Typelist<T,typename MakeTypelist<TT...>::Result> Result;
    };

}

#endif // LOKI_WRAP_H
