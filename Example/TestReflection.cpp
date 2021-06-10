#include "../CoreLib/CoreLib.h"
#include <cassert>
#include <functional>

using namespace JxCoreLib;

namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_META(space::DynCreateClass, Object);
        CORELIB_DECL_DYNCREATEINSTANCE() {
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



void TestReflection()
{
    Type* dyn_type = Type::GetType("space::DynCreateClass");
    Object* dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });

    assert(dyn->get_type() == typeof<space::DynCreateClass>());


}