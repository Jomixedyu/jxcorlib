#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#include <filesystem>
#include <time.h>
#include <bitset>

#include "CoreLib/CoreLib.h"
#include "CoreLib/Property.h"

using namespace std;
using namespace JxCoreLib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();


int main()
{
    using namespace std;
    TestOOP();
    TestProperty();
    TestString();
    TestEvents();

    return 0;

}
