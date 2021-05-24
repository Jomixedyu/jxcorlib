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
