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

#define NulPtr$(ptr) _AssertNull(ptr, DEBUG_INFO(#ptr))
#define Nulable$(ptr) if(ptr != nullptr) ptr

#endif // !_CORELIB_DEBUGTOOL
