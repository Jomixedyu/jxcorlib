#include "StringUtility.h"
#include <iostream>
#include <memory>
#include <Windows.h>

//template<typename... Args>
//std::wstring StringUtility::Format(const std::wstring &str, Args... args) {
//    size_t size = 1 + swprintf(nullptr, 0, str.c_str(), args ...);  // Extra space for \0
//    wchar_t *bytes = new wchar_t[size];
//    swprintf(bytes, size, str.c_str(), args ...);
//    std::wstring ret(bytes);
//    delete[] bytes;
//    return ret;
//}

//wstring=>string
std::string StringUtility::WstringToString(const std::wstring& wstr)
{
    std::string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //宽字节编码转换成多字节编码  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}
// string => wstring
std::wstring StringUtility::StringToWstring(const std::string& str)
{
    std::wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}