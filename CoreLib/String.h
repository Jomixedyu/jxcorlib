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

#define _T(str) str
#define NAMEOF(NAME) #NAME

namespace JxCoreLib
{
    using String = std::string;
    using StringStream = std::stringstream;

    struct Char
    {
        char value[8]{ 0 };

        static void Charcpy(char* dest, const char* src)
        {
            for (size_t i = 0; i < 8; i++) dest[0] = src[0];
        }
        static bool Charcmp(const char* l, const char* r)
        {
            for (size_t i = 0; i < 8; i++) 
                if(l[i] != r[i]) return false;
            return true;
        }
        Char(const char* c)
        {
            Charcpy(this->value, c);
        }
        bool operator==(const Char& r)
        {
            return Charcmp(this->value, r.value);
        }
        bool operator!=(const Char& r)
        {
            return !Charcmp(this->value, r.value);
        }
        String ToString()
        {
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
        static bool IsLittleEndian() noexcept;
        static size_t CharLen(const String& str, const size_t& pos) noexcept(false);
        static String Replace(const String& src, const String& find, const String& target);
        static Char PosAt(const String& src, const size_t& bytepos);
        static Char CharAt(const String& src, const size_t& charpos);
        static Char CharAt(const String& src, const size_t& charpos, const StringIndexMapping& mapping);
        static size_t Length(const String& src);
        static size_t Length(const String& src, const StringIndexMapping& mapping);
        static std::vector<uint8_t> GetBytes(const String& str);

        static std::u16string Utf8ToUtf16(const String& str);
        static String Utf16ToUtf8(const std::u16string& str);
    };

}


#endif // !CORELIB_STRING_H
