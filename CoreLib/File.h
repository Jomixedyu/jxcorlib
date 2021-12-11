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
#include <string_view>

namespace JxCoreLib
{
    namespace FileUtil
    {
        std::string ReadAllText(std::string_view path);
        void WriteAllText(std::string_view path, std::string_view content);
    }
    namespace PathUtil
    {
        std::string GetFilenameWithoutExt(std::string_view path);
        std::string GetFilename(std::string_view path);
        std::string GetFilenameExt(std::string_view path);
        std::string GetDirectory(std::string_view path);
    };
}

#endif // !_CORELIB_FILE_H


