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


struct gen_lambda
{
    template<typename R>
    static std::function<Object_sp(array_list<Object_sp>&&)> get(R(*ptr)())
    {
        return [ptr](array_list<Object_sp>&& objs) -> Object_sp {
            auto ret = ptr();
            if (cltype_concept<R>)
            {

            }
            return BoxUtil::Box(ret);
        };
    }

    template<typename R, typename P1>
    static std::function<Object_sp(array_list<Object_sp>&&)> get(R(*ptr)(P1))
    {
        return [ptr](array_list<Object_sp>&& objs) -> Object_sp {
            auto ret = ptr(UnboxUtil::Unbox<P1>(objs[0]));
            return BoxUtil::Box(ret);
        };
    }

    template<typename R, typename P1, typename P2>
    static std::function<Object_sp(array_list<Object_sp>&&)> get(R(*ptr)(P1, P2))
    {
        return [ptr](array_list<Object_sp>&& objs) -> Object_sp {
            auto ret = ptr(UnboxUtil::Unbox<P1>(objs[0]), UnboxUtil::Unbox<P2>(objs[1]));
            return BoxUtil::Box(ret);
        };
    }
};

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
        return a + int(b);
    }

    static inline struct __corelib_refl_AddOne
    {
        __corelib_refl_AddOne()
        {
            array_list<ParameterInfo*> infos;
            ReflectionBuilder::CreateMethodParameterInfos(AddOne, &infos);
            ReflectionBuilder::CreateMethodInfo(StaticType(), "AddOne", true, true, std::move(infos));
            auto lambd = [](array_list<Object_sp>&& objs) -> Object_sp {
                auto ret = AddOne(UnboxUtil::Unbox<int32_t>(objs[0]), UnboxUtil::Unbox<int64_t>(objs[1]));
                return BoxUtil::Box(ret);
            };
            gen_lambda::get(AddOne);
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


    MethodInfo* minfo = cltypeof<DataModel>()->get_methodinfo("AddOne");
    assert(minfo->get_parameter_infos().at(0)->get_param_type() == cltypeof<Integer32>());
    assert(minfo->get_parameter_infos().at(1)->get_param_type() == cltypeof<Integer64>());
    minfo->Invoke(model, {});
}