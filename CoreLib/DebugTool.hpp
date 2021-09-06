/*
* @Moudule     : DebugTool
* @Date        : 2021/05/20
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef _CORELIB_DEBUGTOOL
#define _CORELIB_DEBUGTOOL

#include <string>

#define DEBUG_INFO(info) std::format("info: {}; line: {}, file: {};", info, __LINE__, __FILE__)

template<typename T>
T* _AssertNull(T* v, const std::string& str)
{
    if (v == nullptr)
    {
        throw JxCoreLib::NullPointerException(str);
    }
    return v;
}

#define nulptr$(ptr) _AssertNull(ptr, DEBUG_INFO(#ptr))
#define nulable$(ptr) if(ptr != nullptr) ptr

template<typename T>
T _AssertZero(T num, const std::string& str)
{
    static_assert(std::is_integral<T>::value);
    if (num == T(0))
    {
        throw JxCoreLib::DivisionByZeroException(str);
    }
    return num;
}

#define zero$(num) _AssertZero(num, DEBUG_INFO(#num));
#define assert_zero(num) if(num == 0) throw JxCoreLib::DivisionByZeroException(DEBUG_INFO(#num));

#endif // !_CORELIB_DEBUGTOOL
