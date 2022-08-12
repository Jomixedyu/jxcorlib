#include "Assembly.h"
#include <cassert>
#include <functional>
#include <map>
#include <iostream>

using namespace JxCoreLib;

namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_Test, space::DynCreateClass, Object);
        CORELIB_DECL_DYNCINST() {
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


class DataModel : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_Test, DataModel, Object);

private:
    CORELIB_REFL_DECL_FIELD(id);
    int id = 0;
public:

    CORELIB_REFL_DECL_FIELD(is_human);
    bool is_human = true;

    CORELIB_REFL_DECL_FIELD(name);
    sptr<Object> name;

    //static inline struct __corelib_refl_ff \
    //{ \
    //    template<typename T> using _Detected = decltype(T::FF); \
    //    __corelib_refl_ff() \
    //{ \
    //    using MethodType = decltype(__corelib_curclass::FF);
    //    using RetType = decltype(std::declval<__corelib_curclass&>().FF());
    //    constexpr bool ispublic = JxCoreLib::is_detected<_Detected, __corelib_curclass>::value;

    //    ReflectionBuilder::CreateMethodInfo(nullptr, "FF", ispublic, )
    //    ReflectionBuilder::CreateFieldInfo<__corelib_curclass, FieldType>(\
    //        #NAME, false, JxCoreLib::is_detected<_Detected, __corelib_curclass>::value, \
    //        [](Object* p) -> sptr<Object> { \
    //        auto rawptr = (__corelib_curclass*)p; \
    //        return get_object_pointer<CleanType>::get(rawptr->NAME); \
    //        }, \
    //        [](Object* p, sptr<Object> value) { \
    //        auto rawptr = (__corelib_curclass*)p; \
    //        object_assign<CleanType>::assign(&rawptr->NAME, value); \
    //        }); \
    //} \
    //} __corelib_refl_ff_;

    //void FF(int);


};


void TestReflection()
{
    sptr<Object> n;

    //dynamic create
    Type* dyn_type =  Assembly::StaticFindAssembly(AssemblyObject_Test)->FindType("space::DynCreateClass");
    auto dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });

    assert(dyn->GetType() == cltypeof<space::DynCreateClass>());

    //field reflection
    DataModel* model = new DataModel;

    Type* model_type = cltypeof<DataModel>();

    //id : const int
    FieldInfo* id_field = model_type->get_fieldinfo("id");
    assert(id_field->is_public() == true);
    assert(id_field->is_static() == false);
    assert(id_field->is_const() == false);
    assert(id_field->is_pointer() == false);

    assert(id_field->get_name() == "id");

    id_field->SetValue(model, mksptr(new Integer32{ 3 }));

    Object_sp id_value = id_field->GetValue(model);
    assert(id_value->GetType() == cltypeof<get_boxing_type<int>::type>());
    assert(*(Integer32*)id_value.get() == 3);

    //name : Object*
    FieldInfo* name_field = model_type->get_fieldinfo("name");

    Object_sp obj = mksptr(new Object());
    name_field->SetValue(model, obj);

    auto value = name_field->GetValue(model);
    assert(value == obj);
}