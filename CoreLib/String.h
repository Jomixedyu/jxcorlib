/*
* @Moudule     : String
* @Date        : 2021/05/24
* @Author      : Jx
* @Description : This file is part of JxCode.CoreLib : https://github.com/Jayshonyves
*/

#ifndef CORELIB_STRING_H
#define CORELIB_STRING_H

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

//Encoding: UTF-8

#define _T(str) u8##str
#define NAMEOF(NAME) u8#NAME

using String = std::string;
using CRString = const std::string&;
using StringStream = std::stringstream;

struct Char
{
    char value[8]{ 0 };
    String ToString() {
        return String(value);
    }
};

bool operator==(const Char& left, const String& right);
bool operator==(const String& left, const Char& right);
bool operator!=(const Char& left, const String& right);
bool operator!=(const String& left, const Char& right);
String operator+(const Char& left, const String& right);
String operator+(const String& left, const Char& right);

struct StringIndexMapping
{
private:
    size_t block_size_;
public:
    StringIndexMapping(const String& str, size_t block_size) noexcept(false);
public:
    std::vector<int> mapping;
    size_t get_block_size() const;
    size_t get_block_count() const;
public:
    size_t GetOffset(const size_t& pos) const noexcept(false);
    size_t GetBlockPos(const size_t& pos) const noexcept(false);
};

class StringUtil
{
private:
    StringUtil() = delete;
public:
    static size_t CharLen(const String& str, const size_t& pos) noexcept(false);
    static String Replace(const String& src, const String& find, const String& target);
    static Char At(const String& src, const size_t& pos);
    static Char At(const String& src, const size_t& pos, const StringIndexMapping& mapping);
    static size_t Length(const String& src);
    static size_t Length(const String& src, const StringIndexMapping& mapping);
    static std::vector<uint8_t> GetBytes(CRString str);
};

class Encoding
{
public:
    static String ANSIToUTF8(CRString str);
    static String UTF8ToANSI(CRString str);
    static std::u16string UTF8ToUTF16(CRString str);
    static String UTF16ToUTF8(const std::u16string& str);
};




#endif // !CORELIB_STRING_H
