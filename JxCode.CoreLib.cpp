
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

    return 0;
}
