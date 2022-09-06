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




    static int GetNum()
    {
        return 3;
    }

    static int AddOne(int32_t a, int64_t b)
    {
        return a + b;
    }

    static inline struct __corelib_refl_AddOne
    {
        __corelib_refl_AddOne()
        {
            array_list<ParameterInfo*> infos;
            ReflectionBuilder::CreateMethodParameterInfos(AddOne, &infos);
            ReflectionBuilder::CreateMethodInfo(StaticType(), "AddOne", true, true, std::move(infos));
            //auto lambd = []() {
            //    AddOne()
            //};
        }
    } __corelib_refl_AddOne_;
};



void TestReflection()
{
    sptr<Object> n;

    //dynamic create
    Type* dyn_type = Assembly::StaticFindAssembly(AssemblyObject_Test)->FindType("space::DynCreateClass");
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


    MethodInfo* minfo = cltypeof< DataModel>()->get_methodinfo("AddOne");
    assert(minfo->get_parameter_infos().at(0)->get_param_type() == cltypeof<Integer32>());
    assert(minfo->get_parameter_infos().at(1)->get_param_type() == cltypeof<Integer64>());
}