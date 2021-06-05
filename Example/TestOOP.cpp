#include "../CoreLib/OOPCore.h"
#include <iostream>
#include <cassert>

using namespace JxCoreLib;

class ExampleClass : public Object
{
    DEF_OBJECT_TYPE(ExampleClass, Object);
public:

};

namespace space
{
    class DynCreateClass : public Object
    {
        DEF_OBJECT_META(space::DynCreateClass, Object);
        DECL_OBJECT_DYNCREATEINSTANCE() {
            if (!params.Check<int>()) {
                return nullptr;
            }
            int p1 = params.Get<int>(0);
            return new DynCreateClass(p1);
        }
    private:
        int id;
    public:
        DynCreateClass(int id) : id(id) {}
    };
}

void TestOOP()
{
    using namespace std;

    ExampleClass* exm = new ExampleClass;

    assert(exm->get_type()->get_name() == string("ExampleClass"));

    Type* dyn_type = Type::GetType("space::DynCreateClass");
    Object* dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });
    
    assert(dyn->get_type() == typeof<space::DynCreateClass>());

}