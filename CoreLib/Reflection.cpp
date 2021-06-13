#include "Reflection.h"


namespace JxCoreLib
{
    FieldInfo::FieldInfo(
        const string& name, bool is_static, bool is_public,
        FieldTypeInfo info, Type* field_type,
        const std::function<Object*(Object* p)> getter,
        std::function<void(Object* p, const std::any& value)> setter
    )
        : base(name, is_static, is_public),
        info_(info), field_type_(field_type), getter_(getter), setter_(setter)
    {

    }


    void FieldInfo::SetValue(Object* instance, const std::any& value)
    {
        this->setter_(instance, value);
    }

    Object* FieldInfo::GetValue(Object* instance) const
    {
        return this->getter_(instance);
    }


    MemberInfo::MemberInfo(const string& name, bool is_static, bool is_public)
        : name_(name), is_static_(is_static), is_public_(is_public)
    {
    }

    MethodInfo::MethodInfo(
        const string& name, bool is_static, bool is_public,
        ParameterInfo* ret_type, std::vector<ParameterInfo*>& params_infos, bool is_abstract
    ) : base(name, is_static, is_public),
        ret_type_(ret_type), param_types_(params_infos), is_abstract_(is_abstract)
    {
    }

    std::any MethodInfo::Invoke(void* instance, const ParameterPackage& params) const
    {
        return std::any();
    }


}