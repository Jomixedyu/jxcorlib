#ifndef CORELIB_REFLECTION_H
#define CORELIB_REFLECTION_H

#include "Object.h"
#include "Type.h"
#include "Exception.h"

#include <functional>
#include <map>

#define CORELIB_REFL_PUBLIC true
#define CORELIB_REFL_NONPUBLIC false


#define CORELIB_REFL_DECL_FIELD(IS_PUBLIC, TYPE, NAME) \
    static inline struct __corelib_refl_##NAME \
    { \
        __corelib_refl_##NAME() \
        { \
            ReflectionBuilder::CreateFieldInfo<__corelib_curclass, TYPE>( \
                #NAME, false, IS_PUBLIC, \
                [](void* p) -> std::any { \
                    return ((__corelib_curclass*)p)->id; \
                }, \
                [](void* p, const std::any& value) { \
                    auto dp = const_cast<std::remove_const<const int>::type*>(&((__corelib_curclass*)p)->id); \
                    *dp = std::any_cast<std::remove_reference<const int>::type>(value); \
                }); \
        } \
    } __corelib_refl_##NAME##_;

#define CORELIB_REFL_DEF_FIELD(IS_PUBLIC, TYPE, NAME) \
    CORELIB_REFL_DECL_FIELD(IS_PUBLIC, TYPE, NAME) \
    TYPE NAME

#define COERLIB_REFL_DECL_FIELD_STATIC(IS_PUBLIC, TYPE, NAME) \
    static inline struct __corelib_refl_##NAME \
    { \
        __corelib_refl_##NAME() \
        { \
            ReflectionBuilder::CreateFieldInfo<__corelib_curclass, TYPE>( \
                #NAME, true, IS_PUBLIC, \
                [](void* p) -> std::any { \
                    return __corelib_curclass::NAME; \
                }, \
                [](void*, const std::any& value) { \
                    auto p = const_cast<std::remove_const<TYPE>::type*>(&__corelib_curclass::NAME); \
                    *p = std::any_cast<std::remove_reference<TYPE>::type>(value); \
                }); \
        } \
    } __corelib_refl_##NAME##_;

#define CORELIB_REFL_DEF_FIELD_STATIC(IS_PUBLIC, TYPE, NAME) \
    COERLIB_REFL_DECL_FIELD_STATIC(IS_PUBLIC, TYPE, NAME) \
    TYPE NAME

namespace JxCoreLib
{
    class TypeInfo : public Object
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::TypeInfo, Object);
    public:
        TypeInfo(const TypeInfo&) = delete;
        TypeInfo(TypeInfo&&) = delete;
    };
    class MemberInfo : public Object
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::MemberInfo, TypeInfo);
    protected:
        string name_;
        bool is_static_;
        bool is_public_;
    public:
        const string& get_name() const { return this->name_; }
        bool is_static() const { return this->is_static_; }
        bool is_public() const { return this->is_public_; }
    public:
        MemberInfo(const string& name, bool is_static, bool is_public);
        MemberInfo(const MemberInfo& right) = delete;
        MemberInfo(MemberInfo&& right) = delete;
    };

    class FieldInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::FieldInfo, MemberInfo);
    public:
        struct FieldTypeInfo
        {
            bool is_pointer;
            bool is_const;
            bool is_reference;
            bool is_volatile;
        };
    protected:
        FieldTypeInfo info_;
        Type* field_type_;
        std::function<std::any(void* p)> getter_;
        std::function<void(void* p, const std::any& value)> setter_;
    public:
        Type* get_field_type() const { return this->field_type_; }
        bool is_pointer() const { return this->info_.is_pointer; }
        bool is_const() const { return this->info_.is_const; }
        bool is_reference() const { return this->info_.is_reference; }
        bool is_volatile() const { return this->info_.is_volatile; }
    public:
        FieldInfo(
            const string& name, bool is_static, bool is_public,
            FieldTypeInfo info, Type* field_type,
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
    class ParameterInfo : public TypeInfo
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::ParameterInfo, TypeInfo);
    protected:
        Type* param_type_;
        bool is_pointer_;
        bool is_const_;
        bool is_reference_;
        bool is_rreference_;
    public:
        Type* get_param_type() const { return this->param_type_; }
        bool is_pointer() const { return this->is_pointer_; }
        bool is_const() const { return this->is_const_; }
        bool is_reference() const { return this->is_reference_; }
        bool is_rreference() const { return this->is_rreference_; }
    public:
        ParameterInfo(const ParameterInfo&) = delete;
        ParameterInfo(ParameterInfo&&) = delete;
        ParameterInfo(
            Type* param_type,
            bool is_pointer,
            bool is_const,
            bool is_reference,
            bool is_rreference);
    };

    //TODO
    class MethodInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(JxCoreLib::MethodInfo, MemberInfo);
    protected:
        std::vector<ParameterInfo*> param_types_;
        ParameterInfo* ret_type_;
        bool is_abstract_;
    public:
        const std::vector<ParameterInfo*>& get_parameter_infos() const noexcept { return this->param_types_; }
        ParameterInfo* get_return_type() const noexcept { return this->ret_type_; }
        bool is_abstract() const { return this->is_abstract_; }
    public:
        MethodInfo(
            const string& name, bool is_static, bool is_public,
            ParameterInfo* ret_type, std::vector<ParameterInfo*>& params_infos, bool is_abstract);
        MethodInfo(const MethodInfo& right) = delete;
        MethodInfo(MethodInfo&& right) = delete;
    public:
        std::any Invoke(void* instance, const ParameterPackage& params) const;
    };


    class ReflectionBuilder
    {
    private:

    public:
        template<typename T, typename TField>
        static inline void CreateFieldInfo(
            const string& name,
            bool is_static, bool is_public,
            const std::function<std::any(void* p)>& getter,
            const std::function<void(void* p, const std::any& value)>& setter
        )
        {
            FieldInfo::FieldTypeInfo info;
            info.is_pointer = std::is_pointer<int>::value;
            info.is_const = std::is_const<const int>::value;
            info.is_reference = std::is_reference<int>::value;
            info.is_volatile = std::is_volatile<int>::value;

            Type* field_type = typeof<fulldecay<TField>::type>();

            typeof<T>()->_AddMemberInfo(new FieldInfo{ name, is_static, is_public, info, field_type, getter, setter });
        }
    };
}

#endif // !CORELIB_REFLECTION_H
