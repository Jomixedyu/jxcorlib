
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>
#include <cassert>
#include <CoreLib/UString.h>

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



CORELIB_DEF_ENUM(AssemblyObject_JxCoreLib,
    , Mode,
    A = 1,
    B = 2,
    C = 3
    );

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

    BoxingMode m = Mode::B;
    cout << m.GetName() << endl;;
    auto f = cltypeof<BoxingMode>()->GetEnumDefinitions();
    cout << cltypeof<BoxingMode>()->get_name() <<endl;

    sptr<List<String_sp>> intlist = mksptr(new List<String_sp>);
    //interface
    auto list = sptr_cast<IList>(intlist);

    list->Add(BoxUtil<string>::Box("aaa"));

    for (auto item : *intlist)
    {
        cout << item << endl;
    }

    cout << PathUtil::Combine("asd/", "/vasf") << endl;

    assert(intlist->GetType()->IsImplementedInterface(cltypeof<IList>()));

    return 0;
}
