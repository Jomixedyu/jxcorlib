#ifndef CORELIB_STRING_H
#define CORELIB_STRING_H

#include <string>
#include <memory>
#include <string_view>

using StringPointer = const char*;
using String = std::string;
using CRString = const std::string&;
using RefString = std::string_view;
using StringStream = std::stringstream;

#define _T(str) str
#define NAMEOF(NAME) #NAME

#endif // !CORELIB_STRING_H
