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
    using string = std::string;
    using StringStream = std::stringstream;

    struct Char
    {
        char value[8]{ 0 };
        inline static int CharLen(const char* c)
        {
            for (int i = 0; i < 8; i++)
            {
                if (c[i] == 0) {
                    return i;
                }
            }
            return 8;
        }
        inline static void Charcpy(char* dest, const char* src)
        {
            for (size_t i = 0; i < 8; i++) dest[i] = src[i];
        }
        inline static bool Charcmp(const char* l, const char* r)
        {
            int len = CharLen(l);
            if (len != CharLen(r))
            {
                return false;
            }
            for (size_t i = 0; i < len; i++)
            {
                if (l[i] != r[i]) return false;
            }

            return true;
        }
        Char() {}
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
        string ToString()
        {
            return string(value);
        }

    };

    bool operator==(const Char& left, const string& right);
    bool operator==(const string& left, const Char& right);
    bool operator!=(const Char& left, const string& right);
    bool operator!=(const string& left, const Char& right);
    string operator+(const Char& left, const string& right);
    string operator+(const string& left, const Char& right);

    struct StringIndexMapping
    {
    private:
        size_t block_size_;
    public:
        StringIndexMapping(const string& str, size_t block_size) noexcept(false);
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
        static size_t CharLen(const string& str, const size_t& pos) noexcept(false);
        static string Replace(const string& src, const string& find, const string& target);
        static Char PosAt(const string& src, const size_t& bytepos);
        static Char CharAt(const string& src, const size_t& charpos);
        static Char CharAt(const string& src, const size_t& charpos, const StringIndexMapping& mapping);
        static size_t Length(const string& src);
        static size_t Length(const string& src, const StringIndexMapping& mapping);
        static std::vector<uint8_t> GetBytes(const string& str);

        static std::u16string Utf8ToUtf16(const string& str);
        static string Utf16ToUtf8(const std::u16string& str);
    };

}


#endif // !CORELIB_STRING_H
