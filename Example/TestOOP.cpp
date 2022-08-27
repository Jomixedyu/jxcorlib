#include <CoreLib/CoreLib.h>
#include <iostream>
#include <cassert>

using namespace JxCoreLib;



class ExampleClass : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, ExampleClass, Object);
public:


    CORELIB_REFL_DECL_FUNC(get_inst);
    sptr<ExampleClass> get_inst() { return self(); }
};
CORELIB_DECL_SHORTSPTR(ExampleClass);

template<typename T, typename K>
class TemplateClass : public Object
{
public:
    CORELIB_DEF_TEMPLATE_TYPE(AssemblyObject_JxCoreLib, TemplateClass, Object, T, K);

};



void TestOOP()
{
    using namespace std;

    ExampleClass_sp exm = mksptr(new ExampleClass);

    auto that = exm->get_inst();

    assert(exm->GetType()->get_base() == cltypeof<Object>());
    assert(exm->GetType()->get_name() == string("ExampleClass"));

    assert(cltypeof<Object>()->IsInstanceOfType(exm.get()));

    assert(ExampleClass::StaticType() == cltypeof<ExampleClass>());

    assert(cltypeof<ExampleClass>()->IsSubclassOf(cltypeof<Object>()));

    auto* templc = new TemplateClass<int, double>;
    Type* templc_type = templc->GetType();

}