#include "../CoreLib/CoreLib.h"
#include <cassert>
#include <functional>
#include <map>


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
#define CORELIB_REFL_DECL_FIELD(IsPublic, IsStatic, Class, Type, Name) \
    static inline struct __corelib_refl_##Name \
    { \
        __corelib_refl_##Name() \
        { \
            ReflectionFieldBuilder::AddFieldInfo(typeof<Class>(), \
                ReflectionFieldBuilder::CreateFieldInfo( \
                #Name, IsPublic, IsStatic, typeof<std::remove_pointer<Type>::type>(), \
                [](void* p) -> std::any { \
                    return ((Class*)p)->Name; \
                }, \
                [](void* p, const std::any& value) { \
                    ((Class*)p)->Name = std::any_cast<Type>(value); \
                })); \
        } \
    } __corelib_refl_##Name##_;


class DataModel : public Object
{
    CORELIB_DEF_TYPE(DataModel, Object);
public:

    CORELIB_REFL_DECL_FIELD(CORE_REFL_PUBLIC, CORE_REFL_NONSTATIC, DataModel, int, id);
    int id;

    CORELIB_REFL_DECL_FIELD(CORE_REFL_PUBLIC, CORE_REFL_NONSTATIC, DataModel, Object*, name);
    Object* name;
};


void TestReflection()
{
    //dynamic create
    Type* dyn_type = Type::GetType("space::DynCreateClass");
    Object* dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });

    assert(dyn->get_type() == typeof<space::DynCreateClass>());
    
    //field reflection
    DataModel* model = new DataModel;

    Type* model_type = typeof<DataModel>();

    //id : int
    FieldInfo* id_field = model_type->get_fieldinfo("id");
    assert(id_field->get_is_public() == true);
    assert(id_field->get_is_static() == false);
    assert(id_field->get_name() == "id");

    id_field->SetValue(model, 3);

    assert(id_field->GetValue(model).type() == typeid(int));
    int id_value = id_field->GetValue<int>(model);
    assert(id_value == 3);

    //name : Object*
    FieldInfo* name_field = model_type->get_fieldinfo("name");

    auto obj = new Object;
    name_field->SetValue(model, obj);

    Object* value = name_field->GetValue<Object*>(model);
    assert(value == obj);
    

}