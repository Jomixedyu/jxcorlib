#include "String.h"


#ifdef _MSC_VER
#include <Windows.h>
static String _Convert(const char* strIn, int sourceCodepage, int targetCodepage)
{
    int  wLen = MultiByteToWideChar(sourceCodepage, 0, strIn, -1, NULL, 0);
    wchar_t* wStr = new wchar_t[wLen];
    MultiByteToWideChar(sourceCodepage, 0, strIn, -1, wStr, wLen);

    int targetLen = WideCharToMultiByte(targetCodepage, 0, wStr, -1, NULL, 0, NULL, NULL);

    String s((size_t)(targetLen), (char)0);

    WideCharToMultiByte(targetCodepage, 0, wStr, -1, &s[0], targetLen, NULL, NULL);

    delete[] wStr;
    return s;
}

inline static String _ANSIToUTF8(CRString str)
{
    return _Convert(str.c_str(), CP_ACP, CP_UTF8);
}
inline static String _UTF8ToANSI(CRString str)
{
    return _Convert(str.c_str(), CP_UTF8, CP_ACP);
}
inline static std::u16string _UTF8ToUTF16(CRString str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::u16string ustr((size_t)len, (char16_t)0);
    
    wchar_t* u16p = reinterpret_cast<wchar_t*>(const_cast<char16_t*>(ustr.c_str()));
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, u16p, 0);
    return ustr;
}
inline static String _UTF16ToUTF8(const std::u16string& str)
{
    wchar_t* u16p = reinterpret_cast<wchar_t*>(const_cast<char16_t*>(str.c_str()));
    int len = WideCharToMultiByte(CP_UTF8, 0, u16p, -1, NULL, 0, NULL, NULL);
    String u8str((size_t)len, (char)0);

    char* u8p = const_cast<char*>(u8str.c_str());
    WideCharToMultiByte(CP_UTF8, 0, u16p, -1, u8p, len, NULL, NULL);
    return u8str;
}
#endif // _MSC_VER

String Encoding::ANSIToUTF8(CRString str)
{
    return _ANSIToUTF8(str);
}

String Encoding::UTF8ToANSI(CRString str)
{
    return _UTF8ToANSI(str);
}

std::u16string Encoding::UTF8ToUTF16(CRString str)
{
    return _UTF8ToUTF16(str);
}

String Encoding::UTF16ToUTF8(const std::u16string& str)
{
    return _UTF16ToUTF8(str);
}

inline static bool _StringEqualsChar(const Char& c, const String& str)
{
    if (str.size() > 6) {
        return false;
    }
    for (size_t i = 0; i < 6; i++)
    {
        if (c.value[i] != str[i]) {
            return false;
        }
    }
    return true;
}
bool operator==(const Char& left, const String& right)
{
    return _StringEqualsChar(left, right);
}

bool operator==(const String& left, const Char& right)
{
    return _StringEqualsChar(right, left);
}

bool operator!=(const Char& left, const String& right)
{
    return _StringEqualsChar(left, right);
}

bool operator!=(const String& left, const Char& right)
{
    return _StringEqualsChar(right, left);
}

String operator+(const Char& left, const String& right)
{
    return left.value + right ;
}

String operator+(const String& left, const Char& right)
{
    return left + right.value;
}

StringIndexMapping::StringIndexMapping(const String& str, size_t block_size) : block_size_(block_size)
{
    if (block_size <= 0) {
        throw std::invalid_argument("block_size");
    }
    size_t len = str.size();
    if (len <= block_size) {
        this->mapping.push_back(0);
        return;
    }

    size_t offset = 0;
    size_t index = 0;

    while (offset < len)
    {
        if ((index % block_size) == 0 || index == 0) {
            this->mapping.push_back(offset);
        }

        offset += StringUtil::CharLen(str, offset);
        index++;
    }
}

size_t StringIndexMapping::get_block_size() const
{
    return this->block_size_;
}

size_t StringIndexMapping::get_block_count() const
{
    return this->mapping.size();
}

size_t StringIndexMapping::GetOffset(const size_t& pos) const
{
    return this->mapping[this->GetBlockPos(pos)];
}
size_t StringIndexMapping::GetBlockPos(const size_t& pos) const
{
    if (pos <= 0) {
        throw std::invalid_argument("the arg must be greater than zero.");
    }
    return pos / this->block_size_;
}

inline static size_t _StringSize(const String& str)
{
    return str.size();
}

size_t StringUtil::CharLen(const String& str, const size_t& pos)
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

inline String StringUtil::Replace(const String& src, const String& find, const String& target)
{
    String nstr(src);
    nstr.replace(nstr.find(find), _StringSize(find), target);
    return nstr;
}

inline static Char _StringUtil_At(
    const String& src, const size_t& pos,
    const size_t& start_offset = 0, const size_t& start_char_count = 0)
{
    size_t size = _StringSize(src);
    size_t offset = start_offset;
    for (size_t i = start_char_count; i < pos; i++)
    {
        offset += StringUtil::CharLen(src, offset);
    }
    Char ch;
    size_t len = StringUtil::CharLen(src, offset);
    for (size_t i = 0; i < len; i++)
    {
        ch.value[i] = src[offset + i];
    }
    return ch;
}

Char StringUtil::At(const String& src, const size_t& pos)
{
    return _StringUtil_At(src, pos);
}

Char StringUtil::At(const String& src, const size_t& pos, const StringIndexMapping& mapping)
{
    size_t block_size = mapping.get_block_size();
    if (block_size == 0) {
        return _StringUtil_At(src, pos);
    }
    size_t block_pos = mapping.GetOffset(pos);
    //位置前面的已有块*块大小=开始字符数
    size_t start_char_count = mapping.GetBlockPos(pos) * mapping.get_block_size();
    return _StringUtil_At(src, pos, mapping.GetOffset(pos), start_char_count);
}

inline static size_t _StringUtil_Length(const String& src, const size_t& start = 0)
{
    size_t size = _StringSize(src);
    int index = start;
    size_t len = 0;

    while (index < size) {
        index += StringUtil::CharLen(src, index);
        len++;
    }
    return len;
}

size_t StringUtil::Length(const String& src)
{
    return _StringUtil_Length(src);
}

size_t StringUtil::Length(const String& src, const StringIndexMapping& mapping)
{
    return (mapping.get_block_count()-1) * mapping.get_block_size()
        + _StringUtil_Length(src, mapping.mapping[mapping.get_block_count()-1]);
}

std::vector<uint8_t> StringUtil::GetBytes(CRString str)
{
    std::vector<uint8_t> c;
    c.reserve(str.size());
    c.assign(str.begin(), str.end());
    return c;
}
