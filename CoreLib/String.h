#ifndef CORELIB_STRING_H
#define CORELIB_STRING_H

#include <string>
#include <memory>

#define _T(str) L##str

using String = std::wstring;
using RefString = std::shared_ptr<std::wstring>;
using StringStream = std::wstringstream;

template<typename... Param>
RefString NewString(Param&& ...str) {
    return std::make_shared<std::wstring>(str...);
}

#endif // !CORELIB_STRING_H
