#include <CoreLib/CoreLib.h>
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

    assert(exm->GetType()->get_base() == cltypeof<Object>());

    assert(exm->GetType()->get_name() == string("ExampleClass"));

    assert(cltypeof<Object>()->IsInstanceOfType(exm));

    assert(cltypeof<ExampleClass>()->IsSubclassOf(cltypeof<Object>()));

    TemplateClass<int>* templc = new TemplateClass<int>;
    Type* templc_type = templc->GetType();
    assert(templc_type->get_name() == "TemplateClass<int>");

    assert(templc->GetType()->get_typeinfo() == typeid(TemplateClass<int>));

}