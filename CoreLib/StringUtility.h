#ifndef CORELIB_STRINGUTILITY_H
#define CORELIB_STRINGUTILITY_H

#include <string>

#pragma warning(disable : 4996)

class StringUtility
{
public:
    template<typename... Args>
    static std::wstring Format(const std::wstring& str, Args... args) {
        size_t size = 1 + swprintf(nullptr, 0, str.c_str(), args ...);  // Extra space for \0
        wchar_t* bytes = new wchar_t[size];
        swprintf(bytes, size, str.c_str(), args ...);
        std::wstring ret(bytes);
        delete[] bytes;
        return ret;
    }

    static std::string WstringToString(const std::wstring& wstr);
    static std::wstring StringToWstring(const std::string& str);
};

#endif // !CORELIB_STRINGUTILITY_H
