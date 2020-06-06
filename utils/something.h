#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include<stdexcept>
#include<array>
#include<iostream>

namespace Util {

    template<typename ...T>
    void logExcept(const T & ...){};

    using __width_int=int;
    using __height_int=int;
    using __factor_int=int;

    inline const int MAX_WIDTH=8000;
    inline const int MAX_HEIGHT=5000;

//     __factor_int factor=5;

    //
    template <int N>
    struct InstructionTP;

    //method+datalength+data....
    struct ModelInput;

    template <typename InputIterator>
    inline void clearAllInput(InputIterator begin_,InputIterator end_);

    struct Psize
    {
        int w{0};
        int h{0};
    };

//    constexpr std::array<Psize,MAX_PIX_COUNTS> arr={Psize{1,2},{2,3},{5,3},{}};
}

#endif // MYEXCEPTION_H
