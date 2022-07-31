
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>


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

    sptr<List<int>> intlist = mksptr(new List<int>);
    //interface
    auto list = sptr_cast<IList>(intlist);

    list->Add(BoxUtil<int>::Box(3));

    for (auto item : *intlist)
    {
        cout << item << endl;
    }

    cout << UnboxUtil<int>::Unbox(list->At(0)) << endl;

    return 0;
}
