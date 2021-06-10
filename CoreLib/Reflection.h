#ifndef CORELIB_REFLECTION_H
#define CORELIB_REFLECTION_H

#include "Object.h"
#include "Type.h"
#include "Exception.h"

namespace JxCoreLib
{
    class MemberInfo : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::MemberInfo, Object);
    private:
        string name_;
        bool is_static_;
        bool is_nonpublic_;
    public:

    };
    class FieldInfo : public MemberInfo
    {
        CORELIB_DEF_TYPE(JxCoreLib::FieldInfo, MemberInfo);
    private:

    public:
        Type* GetFieldType() const;
        void SetValue(const string& name, const std::any& value);
        std::any GetValue(const string& name) const;
    };
    class MethodInfo : public MemberInfo
    {
        CORELIB_DEF_TYPE(JxCoreLib::MethodInfo, MemberInfo);
    public:
        std::vector<Type*> GetParametersType() const;
        Type* GetReturnType() const;
        std::any Invoke(const ParameterPackage& params) const;
    };
}

#endif // !CORELIB_REFLECTION_H
