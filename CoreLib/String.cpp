#include "String.h"
#include <cmath>

static inline uint16_t _ByteSwapInt16(uint16_t number)
{
    return (number >> 8) | (number << 8);
}

static std::string _Utf16LEToUtf8(const std::u16string& u16str)
{
    if (u16str.empty()) { return std::string(); }
    const char16_t* p = u16str.data();
    std::u16string::size_type len = u16str.length();

    if (p[0] == 0xFEFF) {
        p += 1;	//带有bom标记，后移
        len -= 1;
    }

    std::string u8str;
    u8str.reserve(len * 3);

    char16_t u16char;
    for (std::u16string::size_type i = 0; i < len; ++i)
    {
        u16char = p[i];
        // 1字节表示部分
        if (u16char < 0x0080) {
            // u16char <= 0x007f
            // U- 0000 0000 ~ 0000 07ff : 0xxx xxxx
            u8str.push_back((char)(u16char & 0x00FF));	// 取低8bit
            continue;
        }
        // 2 字节能表示部分
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            u8str.push_back((char)(((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char)((u16char & 0x3F) | 0x80));
            continue;
        }
        // 代理项对部分(4字节表示)
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            // * U-00010000 - U-001FFFFF: 1111 0xxx 10xxxxxx 10xxxxxx 10xxxxxx
            uint32_t highSur = u16char;
            uint32_t lowSur = p[++i];
            // 从代理项对到UNICODE代码点转换
            // 1、从高代理项减去0xD800，获取有效10bit
            // 2、从低代理项减去0xDC00，获取有效10bit
            // 3、加上0x10000，获取UNICODE代码点值
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            // 转为4字节UTF8编码表示
            u8str.push_back((char)((codePoint >> 18) | 0xF0));
            u8str.push_back((char)(((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char)(((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char)((codePoint & 0x3F) | 0x80));
            continue;
        }
        // 3 字节表示部分
        {
            // * U-0000E000 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
            u8str.push_back((char)(((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char)(((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char)((u16char & 0x3F) | 0x80));
            continue;
        }
    }

    return u8str;
}

static std::string _Utf16BEToUtf8(const std::u16string& u16str)
{
    if (u16str.empty()) { return std::string(); }
    const char16_t* p = u16str.data();
    std::u16string::size_type len = u16str.length();
    if (p[0] == 0xFEFF) {
        p += 1;	//带有bom标记，后移
        len -= 1;
    }

    // 开始转换
    std::string u8str;
    u8str.reserve(len * 2);
    char16_t u16char;	//u16le 低字节存低位，高字节存高位
    for (std::u16string::size_type i = 0; i < len; ++i) {
        // 这里假设是在小端序下(大端序不适用)
        u16char = p[i];
        // 将大端序转为小端序
        u16char = _ByteSwapInt16(u16char);

        // 1字节表示部分
        if (u16char < 0x0080) {
            // u16char <= 0x007f
            // U- 0000 0000 ~ 0000 07ff : 0xxx xxxx
            u8str.push_back((char)(u16char & 0x00FF));
            continue;
        }
        // 2 字节能表示部分
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            u8str.push_back((char)(((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char)((u16char & 0x3F) | 0x80));
            continue;
        }
        // 代理项对部分(4字节表示)
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            // * U-00010000 - U-001FFFFF: 1111 0xxx 10xxxxxx 10xxxxxx 10xxxxxx
            uint32_t highSur = u16char;
            uint32_t lowSur = _ByteSwapInt16(p[++i]);
            // 从代理项对到UNICODE代码点转换
            // 1、从高代理项减去0xD800，获取有效10bit
            // 2、从低代理项减去0xDC00，获取有效10bit
            // 3、加上0x10000，获取UNICODE代码点值
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            // 转为4字节UTF8编码表示
            u8str.push_back((char)((codePoint >> 18) | 0xF0));
            u8str.push_back((char)(((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char)(((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char)((codePoint & 0x3F) | 0x80));
            continue;
        }
        // 3 字节表示部分
        {
            // * U-0000E000 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
            u8str.push_back((char)(((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char)(((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char)((u16char & 0x3F) | 0x80));
            continue;
        }
    }
    return u8str;
}

static std::string _Utf16ToUtf8(const std::u16string& u16str)
{
    if (::JxCoreLib::StringUtil::IsLittleEndian()) {
        return _Utf16LEToUtf8(u16str);
    }
    else {
        return _Utf16BEToUtf8(u16str);
    }
}

static std::u16string _Utf8ToUtf16LE(const std::string& u8str, bool addbom, bool* ok)
{
    std::u16string u16str;
    u16str.reserve(u8str.size());
    if (addbom) {
        u16str.push_back(0xFEFF);	//bom (字节表示为 FF FE)
    }
    std::string::size_type len = u8str.length();

    const unsigned char* p = (unsigned char*)(u8str.data());
    // 判断是否具有BOM(判断长度小于3字节的情况)
    if (len > 3 && p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) {
        p += 3;
        len -= 3;
    }

    bool is_ok = true;
    // 开始转换
    for (std::string::size_type i = 0; i < len; ++i) {
        uint32_t ch = p[i];	// 取出UTF8序列首字节
        if ((ch & 0x80) == 0) {
            // 最高位为0，只有1字节表示UNICODE代码点
            u16str.push_back((char16_t)ch);
            continue;
        }
        switch (ch & 0xF0)
        {
            case 0xF0: // 4 字节字符, 0x10000 到 0x10FFFF
            {
                uint32_t c2 = p[++i];
                uint32_t c3 = p[++i];
                uint32_t c4 = p[++i];
                // 计算UNICODE代码点值(第一个字节取低3bit，其余取6bit)
                uint32_t codePoint = ((ch & 0x07U) << 18) | ((c2 & 0x3FU) << 12) | ((c3 & 0x3FU) << 6) | (c4 & 0x3FU);
                if (codePoint >= 0x10000)
                {
                    // 在UTF-16中 U+10000 到 U+10FFFF 用两个16bit单元表示, 代理项对.
                    // 1、将代码点减去0x10000(得到长度为20bit的值)
                    // 2、high 代理项 是将那20bit中的高10bit加上0xD800(110110 00 00000000)
                    // 3、low  代理项 是将那20bit中的低10bit加上0xDC00(110111 00 00000000)
                    codePoint -= 0x10000;
                    u16str.push_back((char16_t)((codePoint >> 10) | 0xD800U));
                    u16str.push_back((char16_t)((codePoint & 0x03FFU) | 0xDC00U));
                }
                else
                {
                    // 在UTF-16中 U+0000 到 U+D7FF 以及 U+E000 到 U+FFFF 与Unicode代码点值相同.
                    // U+D800 到 U+DFFF 是无效字符, 为了简单起见，这里假设它不存在(如果有则不编码)
                    u16str.push_back((char16_t)codePoint);
                }
            }
            break;
            case 0xE0: // 3 字节字符, 0x800 到 0xFFFF
            {
                uint32_t c2 = p[++i];
                uint32_t c3 = p[++i];
                // 计算UNICODE代码点值(第一个字节取低4bit，其余取6bit)
                uint32_t codePoint = ((ch & 0x0FU) << 12) | ((c2 & 0x3FU) << 6) | (c3 & 0x3FU);
                u16str.push_back((char16_t)codePoint);
            }
            break;
            case 0xD0: // 2 字节字符, 0x80 到 0x7FF
            case 0xC0:
            {
                uint32_t c2 = p[++i];
                // 计算UNICODE代码点值(第一个字节取低5bit，其余取6bit)
                uint32_t codePoint = ((ch & 0x1FU) << 12) | ((c2 & 0x3FU) << 6);
                u16str.push_back((char16_t)codePoint);
            }
            break;
            default:	// 单字节部分(前面已经处理，所以不应该进来)
                is_ok = false;
                break;
        }
    }
    if (ok != NULL) { *ok = is_ok; }

    return u16str;
}

static std::u16string _Utf8ToUtf16BE(const std::string& u8str, bool addbom, bool* ok)
{
    std::u16string u16str = _Utf8ToUtf16LE(u8str, addbom, ok);
    // reverse
    for (size_t i = 0; i < u16str.size(); ++i) {
        u16str[i] = _ByteSwapInt16(u16str[i]);
    }
    return u16str;
}

static std::u16string _Utf8ToUtf16(const std::string& u8str)
{
    if (::JxCoreLib::StringUtil::IsLittleEndian()) {
        return _Utf8ToUtf16LE(u8str, false, nullptr);
    }
    else {
        return _Utf8ToUtf16BE(u8str, false, nullptr);
    }
}

namespace JxCoreLib
{

    inline static bool _StringEqualsChar(const Char& c, const string& str)
    {
        if (str.size() > 6 || str.empty()) {
            return false;
        }
        return Char::Charcmp(c.value, str.c_str());
    }
    bool operator==(const Char& left, const string& right)
    {
        return _StringEqualsChar(left, right);
    }

    bool operator==(const string& left, const Char& right)
    {
        return _StringEqualsChar(right, left);
    }

    bool operator!=(const Char& left, const string& right)
    {
        return _StringEqualsChar(left, right);
    }

    bool operator!=(const string& left, const Char& right)
    {
        return _StringEqualsChar(right, left);
    }

    string operator+(const Char& left, const string& right)
    {
        return left.value + right;
    }

    string operator+(const string& left, const Char& right)
    {
        return left + right.value;
    }

    StringIndexMapping::StringIndexMapping(const string& str, size_t block_size) : block_size_(block_size)
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
                this->mapping.push_back(static_cast<int>(offset));
            }

            offset += StringUtil::U8Length(str, offset);
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

    bool StringUtil::IsLittleEndian() noexcept
    {
        static int i = 1;
        static bool b = *(char*)&i;
        return b;
    }

    size_t StringUtil::U8Length(const string& str, const size_t& pos)
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

    string StringUtil::_Replace(
        const char* src, size_t src_len,
        const char* oldstr, size_t oldstr_len,
        const char* newstr, size_t newstr_len)
    {
        if (src_len == 0) { return string{}; }
        if (oldstr_len == 0 || newstr_len == 0) { return string{ src }; }

        string nstr;
        if (newstr_len <= oldstr_len)
        {
            nstr.reserve(src_len + 1);
        }
        else
        {
            //最多替换次数
            size_t count = src_len / oldstr_len;
            //长度比例，最大2
            double m = std::min(2.0, (double)newstr_len / (double)oldstr_len);
            //可替换字符串放大后的长度
            size_t base = (size_t)((double)(count * oldstr_len) * m) + 1;
            //非可替换字符串长度
            size_t app = src_len - count * oldstr_len + 1;

            size_t len = base + app;

            nstr.reserve(len);
        }
        nstr = src;
        size_t pos = 0;
        while ((pos <= nstr.size()) && ((pos = nstr.find(oldstr, pos)) != nstr.npos))
        {
            nstr.replace(pos, oldstr_len, newstr);
            pos += newstr_len;
        }
        return nstr;
    }

    inline static Char _StringUtil_At(
        const string& src, const size_t& pos,
        const size_t& start_offset = 0, const size_t& start_char_count = 0)
    {
        size_t size = StringUtil::Size(src);
        size_t offset = start_offset;
        for (size_t i = start_char_count; i < pos; i++)
        {
            offset += StringUtil::U8Length(src, offset);
        }
        Char ch;
        size_t len = StringUtil::U8Length(src, offset);
        for (size_t i = 0; i < len; i++)
        {
            ch.value[i] = src[offset + i];
        }
        return ch;
    }
    Char StringUtil::PosAt(const string& src, const size_t& bytepos)
    {
        Char ch;
        size_t len = StringUtil::U8Length(src, bytepos);
        for (size_t i = 0; i < len; i++)
        {
            ch.value[i] = src[bytepos + i];
        }
        return ch;
    }
    Char StringUtil::CharAt(const string& src, const size_t& charpos)
    {
        return _StringUtil_At(src, charpos);
    }

    Char StringUtil::CharAt(const string& src, const size_t& charpos, const StringIndexMapping& mapping)
    {
        size_t block_size = mapping.get_block_size();
        if (block_size == 0) {
            return _StringUtil_At(src, charpos);
        }
        size_t block_pos = mapping.GetOffset(charpos);
        //位置前面的已有块*块大小=开始字符数
        size_t start_char_count = mapping.GetBlockPos(charpos) * mapping.get_block_size();
        return _StringUtil_At(src, charpos, mapping.GetOffset(charpos), start_char_count);
    }

    inline static size_t _StringUtil_U8Length_Internal(const string& src, const size_t& start = 0)
    {
        size_t size = StringUtil::Size(src);
        size_t index = start;
        size_t len = 0;

        while (index < size) {
            index += StringUtil::U8Length(src, index);
            len++;
        }
        return len;
    }

    size_t StringUtil::Length(const string& src)
    {
        return _StringUtil_U8Length_Internal(src);
    }

    size_t StringUtil::Length(const string& src, const StringIndexMapping& mapping)
    {
        return (mapping.get_block_count() - 1) * mapping.get_block_size()
            + _StringUtil_U8Length_Internal(src, mapping.mapping[mapping.get_block_count() - 1]);
    }

    std::vector<uint8_t> StringUtil::GetBytes(const string& str)
    {
        std::vector<uint8_t> c;
        c.reserve(str.size());
        c.assign(str.begin(), str.end());
        return c;
    }

    std::u16string StringUtil::Utf8ToUtf16(const string& str)
    {
        return _Utf8ToUtf16(str);
    }

    string StringUtil::Utf16ToUtf8(const std::u16string& str)
    {
        return _Utf16ToUtf8(str);
    }

    string StringUtil::StringCast(const std::u8string& str)
    {
        const size_t size = str.size();
        string ostr(size, 0);
        if constexpr (sizeof(char) == sizeof(char8_t))
        {
            memcpy(const_cast<char*>(ostr.c_str()), str.c_str(), size);
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                ostr.push_back((char)str.at(i));
            }
        }
        return ostr;
    }

}