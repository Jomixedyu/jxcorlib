#ifndef CORELIB_REFLECTION_H
#define CORELIB_REFLECTION_H

#include "Object.h"
#include "Type.h"
#include "Exception.h"

#include <functional>
#include <map>

#define CORE_REFL_PUBLIC true
#define CORE_REFL_NONPUBLIC false
#define CORE_REFL_STATIC true
#define CORE_REFL_NONSTATIC false

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

namespace JxCoreLib
{
    class MemberInfo : public Object
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::MemberInfo, Object);
    protected:
        string name_;
        bool is_static_;
        bool is_public_;
    public:
        const string& get_name() const { return this->name_; }
        bool get_is_static() const { return this->is_static_; }
        bool get_is_public() const { return this->is_public_; }
    public:
        MemberInfo(const string& name, bool is_public, bool is_static);
        MemberInfo(const MemberInfo& right) = delete;
        MemberInfo(MemberInfo&& right) = delete;
    };

    class FieldInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::FieldInfo, MemberInfo);
    protected:
        Type* field_type_;

        std::function<std::any(void* p)> getter_;
        std::function<void(void* p, const std::any& value)> setter_;
    public:
        Type* get_field_type() const { return this->field_type_; }
    public:
        FieldInfo(
            const string& name, bool is_public, bool is_static,
            Type* field_type,
            const std::function<std::any(void* p)> getter,
            std::function<void(void* p, const std::any& value)> setter);
        FieldInfo(const FieldInfo& right) = delete;
        FieldInfo(FieldInfo&& right) = delete;
    public:
        void SetValue(void* instance, const std::any& value);
        std::any GetValue(void* instance) const;
        template<typename T> T GetValue(void* instance) const
        {
            return std::any_cast<T>(this->GetValue(instance));
        }
    };
    //TODO
    class MethodInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::MethodInfo, MemberInfo);
    protected:
        std::vector<Type*> param_types_;
        Type* ret_type_;
    public:
        const std::vector<Type*>& get_parameter_types() const noexcept { return this->param_types_; }
        Type* get_return_type() const noexcept { return this->ret_type_; }
    public:
        MethodInfo();
        MethodInfo(const MethodInfo& right) = delete;
        MethodInfo(MethodInfo&& right) = delete;
    public:
        std::any Invoke(void* instance, const ParameterPackage& params) const;
    };


    class ReflectionFieldBuilder
    {
    private:

    public:
        static inline FieldInfo* CreateFieldInfo(
            const string& name,
            bool is_public,
            bool is_static,
            Type* type,
            const std::function<std::any(void* p)>& getter,
            const std::function<void(void* p, const std::any& value)>& setter
        )
        {
            return new FieldInfo(name, is_public, is_static, type, getter, setter);
        }
        static inline void AddFieldInfo(Type* type, FieldInfo* info)
        {
            type->_AddMemberInfo(info);
        }
    };
}

#endif // !CORELIB_REFLECTION_H
