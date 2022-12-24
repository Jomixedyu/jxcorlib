#include <CoreLib/CoreLib.h>
#include <iostream>
#include <cassert>

using namespace JxCoreLib;



class ExampleClass : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, ExampleClass, Object);
public:
    CORELIB_REFL_DECL_FIELD(ilist);
    List_sp<int> ilist;
    CORELIB_REFL_DECL_FIELD(slist);
    List_sp<sptr<ExampleClass>> slist;
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

    ExampleClass_sp exm1 = mksptr(new ExampleClass);

    exm->ilist = mksptr(new List<int>{ 2,3 });
    exm1->ilist = mksptr(new List<int>{ 5,35 });

    exm->slist = mksptr(new List<ExampleClass_sp> { exm1 });

    auto copy = sptr_cast<ExampleClass>(ObjectUtil::DeepCopyObject(exm));

    assert(exm->ilist->Equals(copy->ilist.get()));

    assert(exm->GetType()->get_base() == cltypeof<Object>());
    assert(exm->GetType()->get_name() == string("ExampleClass"));

    assert(cltypeof<Object>()->IsInstanceOfType(exm.get()));

    assert(ExampleClass::StaticType() == cltypeof<ExampleClass>());

    assert(cltypeof<ExampleClass>()->IsSubclassOf(cltypeof<Object>()));

    auto* templc = new TemplateClass<int, double>;
    Type* templc_type = templc->GetType();

}