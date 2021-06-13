#include "../CoreLib/CoreLib.h"
#include <cassert>
#include <functional>
#include <map>
#include <iostream>

using namespace JxCoreLib;

namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_META(space::DynCreateClass, Object);
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
    CORELIB_REFL_DECL_FIELD(true , id);
    const int id = 0;
public:

    CORELIB_REFL_DECL_FIELD(true, is_human);
    bool is_human = true;

    COERLIB_REFL_DECL_FIELD_STATIC(true, name);
    static inline Object* name;
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

    //id : const int
    FieldInfo* id_field = model_type->get_fieldinfo("id");
    assert(id_field->is_public() == true);
    assert(id_field->is_static() == false);
    assert(id_field->is_const() == true);
    assert(id_field->is_pointer() == false);
    assert(id_field->is_reference() == false);
    assert(id_field->is_volatile() == false);
    assert(id_field->get_name() == "id");

    id_field->SetValue(model, 3);

    Object* id_value = id_field->GetValue(model);
    assert(id_value->get_type() == typeof<int>());
    assert(*(Integer32*)id_value == 3);

    //name : Object*
    FieldInfo* name_field = model_type->get_fieldinfo("name");

    auto obj = new Object();
    name_field->SetValue(nullptr, obj);

    auto value = name_field->GetValue(nullptr);
    assert(value == obj);
}