#include <CoreLib/CoreLib.h>
#include <CoreLib/Enum.h>
#include <cassert>
using namespace JxCoreLib;

CORELIB_DEF_ENUM(AssemblyObject_JxCoreLib,
    , Mode,
    A,
    B = 3,
    C
);



void TestEnum()
{
    BoxingMode m = Mode::B;
    assert(m.GetName() == "B");
    auto defs = cltypeof<BoxingMode>()->GetEnumDefinitions();
    assert((*defs)[0].second == 0);
    assert((*defs)[1].second == 3);
    assert((*defs)[2].second == 4);

}