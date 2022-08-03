
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace JxCoreLib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();
extern void TestReflection();
extern void TestJsonSerializer();
extern void TestDynCreateInst();
extern void TestFormater();
extern void TestFile();

int main()
{
    using namespace std;

    TestOOP();
    TestProperty();
    TestString();
    TestEvents();
    TestReflection();
    TestJsonSerializer();
    TestDynCreateInst();
    TestFormater();
    //TestFile();

    sptr<List<String_sp>> intlist = mksptr(new List<String_sp>);
    //interface
    auto list = sptr_cast<IList>(intlist);

    list->Add(BoxUtil<string>::Box("aaa"));

    for (auto item : *intlist)
    {
        cout << item << endl;
    }
    

    assert(intlist->GetType()->IsImplementedInterface(cltypeof<IList>()));

    return 0;
}
