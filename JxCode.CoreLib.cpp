#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#include <filesystem>
#include <time.h>
#include <bitset>
#include <cassert>
#include <type_traits>
#include "CoreLib/CoreLib.h"
#include "CoreLib/Property.h"

using namespace std;
using namespace JxCoreLib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();
extern void TestReflection();
extern void TestJsonSerializer();
extern void TestManagedPtr();




class TestClass : public Object
{
    CORELIB_DEF_TYPE(TestClass, Object)
private:

    TestClass(int)
    {
        int i;
    }
};


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



    return 0;

}
