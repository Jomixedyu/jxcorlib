#include "../CoreLib/CoreLib.h"
#include <iostream>
#include <cassert>

using namespace JxCoreLib;

class ExampleClass : public Object
{
    CORELIB_DEF_TYPE(ExampleClass, Object);
public:

};



void TestOOP()
{
    using namespace std;

    ExampleClass* exm = new ExampleClass;

    assert(exm->get_type()->get_base() == typeof<Object>());

    assert(exm->get_type()->get_name() == string("ExampleClass"));

    assert(typeof<Object>()->IsInstanceOfType(exm));

    assert(typeof<ExampleClass>()->IsSubclassOf(typeof<Object>()));
}