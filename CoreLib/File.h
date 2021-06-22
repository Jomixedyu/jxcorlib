/*
* @Moudule     : File
* @Date        : 2020/07/14
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JxmiXedYu/JxCode.CoreLib
* @StdRequired : c++11
*/

#ifndef CORELIB_FILE_H
#define CORELIB_FILE_H

#include <string>

namespace JxCoreLib
{
    class File final
    {
    public:
        static std::string ReadAllText(const std::string& path);
    };
}

#endif // !CORELIB_FILE_H


