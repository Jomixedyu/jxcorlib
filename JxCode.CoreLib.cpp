
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>
#include <cassert>
#include <CoreLib/UString.h>

using namespace std;
using namespace jxcorlib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();
extern void TestReflection();
extern void TestJsonSerializer();
extern void TestDynCreateInst();
extern void TestFormater();
extern void TestFile();
extern void TestEnum();
extern void TestGuid();
extern void TestMath();
extern void TestPlatform();
extern void TestDelegate();

int main()
{
    using namespace std;

    TestOOP();
    TestEnum();
    TestProperty();
    TestString();
    TestDelegate();
    TestEvents();
    TestReflection();
    TestJsonSerializer();
    TestDynCreateInst();
    TestFormater();
    TestGuid();
    TestFile();
    TestMath();
    TestPlatform();


    

    return 0;
}
