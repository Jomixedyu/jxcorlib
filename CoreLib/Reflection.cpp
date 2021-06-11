#include "Reflection.h"


namespace JxCoreLib
{
    FieldInfo::FieldInfo(
        const string& name, bool is_public, bool is_static,
        Type* field_type, 
        const std::function<std::any(void* p)> getter, 
        std::function<void(void* p, const std::any& value)> setter
    )
        : base(name, is_public, is_static),
        field_type_(field_type), getter_(getter), setter_(setter)
    {

    }


    void FieldInfo::SetValue(void* instance, const std::any& value)
    {
        this->setter_(instance, value);
    }

    std::any FieldInfo::GetValue(void* instance) const
    {
        return this->getter_(instance);
    }



    MemberInfo::MemberInfo(const string& name, bool is_public, bool is_static)
        : name_(name), is_public_(is_public), is_static_(is_static)
    {
    }

    std::any MethodInfo::Invoke(void* instance, const ParameterPackage& params) const
    {
        return std::any();
    }

}