#ifndef CORELIB_STRING_H
#define CORELIB_STRING_H

#include <string>
#include <memory>

using StringPointer = const wchar_t*;
using String = std::wstring;
using RefString = std::shared_ptr<std::wstring>;
using StringStream = std::wstringstream;

#define _T(str) L##str
#define STR(str) String(L##str)


RefString operator+(const RefString& left, const RefString& right);

RefString operator+(const StringPointer& left, const RefString& right);
RefString operator+(const RefString& left, const StringPointer& right);

RefString operator+(const RefString& left, const String& right);
RefString operator+(const String& left, const RefString& right);

template<typename... Param>
RefString NewString(Param&& ...str) {
    return std::make_shared<std::wstring>(str...);
}

#endif // !CORELIB_STRING_H
