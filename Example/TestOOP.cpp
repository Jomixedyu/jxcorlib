#include "../CoreLib/CoreLib.h"
#include <iostream>
#include <cassert>

using namespace JxCoreLib;

class ExampleClass : public Object
{
    CORELIB_DEF_TYPE(ExampleClass, Object);
public:

};

template<typename T>
class TemplateClass : public Object
{
    CORELIB_DEF_TEMPLATE_TYPE(TemplateClass, Object, T);
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

    TemplateClass<int>* templc = new TemplateClass<int>;
    Type* templc_type = templc->get_type();
    assert(templc_type->get_name() == "TemplateClass<int>");

    assert(templc->get_type()->get_typeinfo() == typeid(TemplateClass<int>));

}