#include <CoreLib/CoreLib.h>
#include <CoreLib/Reflection.h>
#include <cassert>
#include <functional>
#include <map>
#include <iostream>

using namespace JxCoreLib;

namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_TYPE(space::DynCreateClass, Object);
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
    CORELIB_DEF_TYPE(DataModel, Object);

private:
    //static inline struct __corelib_refl_id \
    //{ \
    //    template<typename T> using _Detected = decltype(std::declval<T&>().NAME); \
    //    __corelib_refl_id() \
    //{ \
    //    using FieldType = decltype(std::declval<__corelib_curclass&>().id); \
    //    using DecayType = fulldecay<FieldType>::type; \
    //    using ClType = get_cltype<remove_shared_ptr<std::remove_cv<FieldType>::type>::type>::type; \

    //    ReflectionBuilder::CreateFieldInfo<__corelib_curclass, FieldType>(\
    //        "id", false, JxCoreLib::is_detected<_Detected, __corelib_curclass>::value, \
    //        [](const sptr<Object>& p) -> sptr<Object> { \
    //        auto rawptr = (__corelib_curclass*)p.get(); \
    //        return get_object_pointer<DecayType>::get(rawptr->id); \
    //        }, \
    //        [](const sptr<Object>& p, const sptr<Object>& value) { \
    //        auto rawptr = (__corelib_curclass*)p.get();
    //        object_assign<DecayType>::assign(&rawptr->id, value);
    //        }); \
    //} \
    //} __corelib_refl_id;
    CORELIB_REFL_DECL_FIELD(id);
    int id = 0;
public:

    CORELIB_REFL_DECL_FIELD(is_human);

    bool is_human = true;

    //CORELIB_REFL_DECL_FIELD(name);
    //static inline struct __corelib_refl_name \
    //{ \
    //    template<typename T> using _Detected = decltype(std::declval<T&>().NAME); \
    //    __corelib_refl_name() \
    //{ \
    //    using _Ty = decltype(std::declval<__corelib_curclass&>().name); \
    //    using _Fuldecay = remove_shared_ptr< fulldecay<_Ty>::type>::type; \
    //    using _CTy = get_cltype<std::remove_cv<_Ty>::type>::type; \
    //    using _TyOncePtr = get_cltype<_Fuldecay>::type*; \
    //    ReflectionBuilder::CreateFieldInfo<__corelib_curclass, _Ty>(\
    //        "name", false, JxCoreLib::is_detected<_Detected, __corelib_curclass>::value, \
    //        [](Object* p) -> Object* { \
    //        return get_object_pointer<_Fuldecay>::get(((__corelib_curclass*)p)->name); \
    //        }, \
    //        [](Object* p, Object* value) { \
    //        auto dp = const_cast<std::remove_cv<_Ty>::type*>(&((__corelib_curclass*)p)->name); \
    //        _TyOncePtr obj = static_cast<_TyOncePtr>(value); \
    //        * dp = *find_pointer_if<_CTy, !std::is_pointer<_Ty>::value >::get(&obj); \
    //        }); \
    //} \
    //} __corelib_refl_name;
    sptr<Object> name;
};


void TestReflection()
{
    sptr<Object> n;

    //dynamic create
    Type* dyn_type = Type::GetType("space::DynCreateClass");
    auto dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });

    assert(dyn->GetType() == cltypeof<space::DynCreateClass>());

    //field reflection
    DataModel* model = new DataModel;

    Type* model_type = cltypeof<DataModel>();

    //id : const int
    FieldInfo* id_field = model_type->get_fieldinfo("id");
    assert(id_field->is_public() == true);
    assert(id_field->is_static() == false);
    assert(id_field->is_const() == true);
    assert(id_field->is_pointer() == false);

    assert(id_field->get_name() == "id");

    //id_field->SetValue(model, new Integer32{ 3 });

    //Object* id_value = id_field->GetValue(model);
    //assert(id_value->GetType() == cltypeof<get_cltype<int>::type>());
    //assert(*(Integer32*)id_value == 3);

    ////name : Object*
    //FieldInfo* name_field = model_type->get_fieldinfo("name");

    //auto obj = new Object();
    //name_field->SetValue(nullptr, obj);

    //auto value = name_field->GetValue(nullptr);
    //assert(value == obj);
}