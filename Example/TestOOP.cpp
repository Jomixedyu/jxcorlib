#include <CoreLib/CoreLib.h>
#include <iostream>
#include <cassert>
#include <CoreLib/Converter.hpp>

using namespace JxCoreLib;

class ExampleClass : public Object
{
    CORELIB_DEF_TYPE(ExampleClass, Object);
public:

};

template<typename T, typename K>
class TemplateClass : public Object
{
    CORELIB_DEF_TEMPLATE_TYPE(TemplateClass, Object, T, K);
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

    auto* templc = new TemplateClass<int, double>;
    Type* templc_type = templc->GetType();

    assert(templc_type->get_template_types()->at(0) == cltypeof<int>());
    assert(templc_type->get_template_types()->at(1) == cltypeof<double>());

}