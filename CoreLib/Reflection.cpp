#include "Reflection.h"


namespace JxCoreLib
{



    Type* JxCoreLib::FieldInfo::GetFieldType() const
    {
        return nullptr;
    }

    void FieldInfo::SetValue(const string& name, const std::any& value)
    {
    }

    std::any FieldInfo::GetValue(const string& name) const
    {
        return std::any();
    }


    std::vector<Type*> MethodInfo::GetParametersType() const
    {
        return std::vector<Type*>();
    }

    Type* MethodInfo::GetReturnType() const
    {
        return nullptr;
    }

    std::any MethodInfo::Invoke(const ParameterPackage& params) const
    {
        return std::any();
    }

}