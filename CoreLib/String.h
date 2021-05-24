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

class StringUtil
{
private:
    inline static size_t _StringSize(const String& str)
    {
        return str.size();
    }
    inline static size_t _StringSize(const char*& str)
    {
        return ::strlen(str);
    }
    template<typename TStr>
    inline static size_t _CharLen(const TStr& str, const size_t& pos)
    {
        unsigned char c = static_cast<unsigned char>(str[pos]);
        if ((c & 0b10000000) == 0b00000000)  return 1;
        else if ((c & 0b11100000) == 0b11000000) return 2;
        else if ((c & 0b11110000) == 0b11100000) return 3;
        else if ((c & 0b11111000) == 0b11110000) return 4;
        else if ((c & 0b11111100) == 0b11111000) return 5;
        else if ((c & 0b11111110) == 0b11111100) return 6;
        throw std::invalid_argument("string is invalid");
    }
public:
    template<typename TSrc, typename TFind, typename TTarget>
    inline static String Replace(const TSrc& src, const TFind& find, const TTarget& target)
    {
        String nstr(src);
        nstr.replace(nstr.find(find), _StringSize(find), target);
        return nstr;
    }
    template<typename TSrc>
    inline static Char At(const TSrc& src, const size_t& pos)
    {
        size_t size = _StringSize(src);
        size_t offset = 0;
        size_t len = 0;
        for (size_t i = 0; i < pos; i++)
        {
            len = _CharLen(src, offset);
            offset += len;
        }
        Char ch;
        for (size_t i = 0; i < len; i++)
        {
            ch.value[i] = src[offset + i];
        }
        return ch;
    }
    template<typename TSrc>
    inline static size_t Length(const TSrc& src)
    {
        size_t size = _StringSize(src);
        int index = 0;
        size_t len = 0;

        while (index < size) {
            index += _CharLen(src, index);
            len++;
        }
        return len;
    }

    inline static std::vector<uint8_t> GetBytes(CRString str)
    {
        std::vector<uint8_t> c;
        c.reserve(str.size());
        c.assign(str.begin(), str.end());
        return c;
    }
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
