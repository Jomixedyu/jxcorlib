
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.h"
#include <format>

using namespace std;
using namespace JxCoreLib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();
extern void TestReflection();
extern void TestJsonSerializer();
extern void TestManagedPtr();
extern void TestDynCreateInst();
extern void TestManagedVector();

int main()
{
    using namespace std;

    TestOOP();
    TestProperty();
    TestString();
    TestEvents();
    TestReflection();
    TestJsonSerializer();
    TestManagedPtr();
    TestDynCreateInst();
    TestManagedVector();

    return 0;
}
