#include "../CoreLib/String.h"

using namespace JxCoreLib;

void TestString()
{
    String s("a word Ò»¸ö×Ö");
    Char c = StringUtil::CharAt(s, 9);

    StringIndexMapping mapping(s, 2); // use cache
    Char c2 = StringUtil::CharAt(s, 9, mapping);

    Char nc("¸ö");
    nc.ToString();

}