/*
* @Moudule     : File
* @Date        : 2020/07/14
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++11
*/

#ifndef _CORELIB_FILE_H
#define _CORELIB_FILE_H

#include <string>

namespace JxCoreLib
{
    namespace FileUtil
    {
        std::string ReadAllText(const std::string& path);
    }
    namespace PathUtil
    {
        std::string GetFilenameWithoutExt(const std::string& path);
    };
}

#endif // !_CORELIB_FILE_H


