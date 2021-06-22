/*
* @Moudule     : DebugTool
* @Date        : 2021/05/20
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JxmiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef CORELIB_DEBUGTOOL
#define CORELIB_DEBUGTOOL

#define DEBUG_INFO(info) std::format("info: {}; line: {}, file: {};", info, __LINE__, __FILE__)

#define CHECK_NULLPOINTER(ptr) if(ptr == nullptr) throw JxCoreLib::NullPointerException(DEBUG_INFO(#ptr));

#endif // !CORELIB_DEBUGTOOL
