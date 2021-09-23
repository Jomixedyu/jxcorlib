#include <CoreLib/UString.h>
#include <cassert>

using namespace JxCoreLib;

void TestString()
{
    string s("a word 一个字");
    assert(StringUtil::Length(s) == 10);

    u8char c = StringUtil::CharAt(s, 9);
    assert(c == u8char{ "字" });

    StringIndexMapping mapping(s, 2); // use cache
    u8char c2 = StringUtil::CharAt(s, 9, mapping);
    assert(c2 == u8char{ "字" });

    u8char nc("个");
    nc.ToString();
    assert(nc.ToString() == string("个"));

    string str_concat = StringUtil::Concat("123", "ab", string("ba"));
    assert(str_concat == "123abba");

    string newstr = StringUtil::Replace(str_concat, "ab", "a");
    assert(newstr == "123aba");
}