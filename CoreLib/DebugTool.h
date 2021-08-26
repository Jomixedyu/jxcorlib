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

#define CHECK_NULLPOINTER(ptr) if(ptr == nullptr) throw JxCoreLib::NullPointerException(DEBUG_INFO(#ptr));

template<typename T>
T* AssertNull(T* v, const std::string& str)
{
    if (v == nullptr)
    {
        throw JxCoreLib::NullPointerException(str);
    }
    return v;
}

#define ANUL(ptr) AssertNull(ptr, DEBUG_INFO(#ptr))

#endif // !_CORELIB_DEBUGTOOL
