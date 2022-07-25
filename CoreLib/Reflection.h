/*
* @Moudule     : Reflection
* @Date        : 2021/05/23
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++17
*/

#ifndef _CORELIB_REFLECTION_H
#define _CORELIB_REFLECTION_H

#include "Object.h"
#include "Type.h"

#include <functional>
#include <any>
#include <memory>
//支持 基础值类型 系统内指针（包括智能指针）类型
#define CORELIB_REFL_DECL_FIELD(NAME) \
    static inline struct __corelib_refl_##NAME \
    { \
        template<typename T> using _Detected = decltype(std::declval<T&>().NAME); \
        __corelib_refl_##NAME() \
        { \
            using _Ty = decltype(std::declval<__corelib_curclass&>().NAME); \
            using _Fuldecay = fulldecay<_Ty>::type; \
            using _CTy = get_cltype<std::remove_cv<_Ty>::type>::type; \
            using _TyOncePtr = get_cltype<_Fuldecay>::type*; \
            ReflectionBuilder::CreateFieldInfo<__corelib_curclass, _Ty>( \
                #NAME, false, JxCoreLib::is_detected<_Detected, __corelib_curclass>::value, \
                [](Object* p) -> Object* { \
                    return get_object_pointer<_Fuldecay>::get(((__corelib_curclass*)p)->NAME); \
                }, \
                [](Object* p, Object* value) { \
                    auto dp = const_cast<std::remove_cv<_Ty>::type*>(&((__corelib_curclass*)p)->NAME); \
                    _TyOncePtr obj = static_cast<_TyOncePtr>(value); \
                    *dp = *find_pointer_if<_CTy, !std::is_pointer<_Ty>::value>::get(&obj); \
                }); \
        } \
    } __corelib_refl_##NAME##_;


namespace JxCoreLib
{
    class TypeInfo : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::TypeInfo, Object);
    public:
        TypeInfo(const TypeInfo&) = delete;
        TypeInfo(TypeInfo&&) = delete;
    };

    class MemberInfo : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::MemberInfo, TypeInfo);
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
        CORELIB_DEF_TYPE(JxCoreLib::FieldInfo, MemberInfo);
    public:
        struct FieldTypeInfo
        {
            bool is_raw_pointer;
            bool is_shared_pointer;
            bool is_weak_pointer;
            bool is_const;
        };
        using GetterType = std::function<Object* (Object* p)>;
        using SetterType = std::function<void(Object* p, Object* value)>;
    protected:
        FieldTypeInfo info_;
        Type* field_type_;
        GetterType getter_;
        SetterType setter_;
    public:
        Type* get_field_type() const { return this->field_type_; }
        bool is_raw_pointer() const { return this->info_.is_raw_pointer; }
        bool is_shared_pointer() const { return this->info_.is_shared_pointer; }
        bool is_weak_pointer() const { return this->info_.is_weak_pointer; }
        bool is_pointer() const { return is_raw_pointer() || is_shared_pointer() || is_weak_pointer(); }
        bool is_const() const { return this->info_.is_const; }

    public:
        FieldInfo(
            const string& name, bool is_static, bool is_public,
            FieldTypeInfo info, Type* field_type,
            const GetterType& getter, const SetterType& setter);

        FieldInfo(const FieldInfo& right) = delete;
        FieldInfo(FieldInfo&& right) = delete;
    public:
        void SetValue(Object* instance, Object* value);
        [[nodiscard]] Object* GetValue(Object* instance) const;
        std::unique_ptr<Object> GetValueUnique(Object* instance) const;
    private:
        template<typename TValue, typename TType>
        static inline bool _Assign(TValue* t, Object* value)
        {
            if (value->GetType() == cltypeof<TType>())
            {
                *t = static_cast<TType*>(value)->value;
                return true;
            }
            return false;
        }
    public:
        template<typename T>
        static bool Assign(T* t, Object* value)
        {
            return
                _Assign<T, String>(t, value) ||
                _Assign<T, Integer32>(t, value) ||
                _Assign<T, UInteger32>(t, value) ||
                _Assign<T, Single32>(t, value) ||
                _Assign<T, Double64>(t, value) ||
                _Assign<T, Boolean>(t, value) ||
                _Assign<T, Integer8>(t, value) ||
                _Assign<T, UInteger8>(t, value) ||
                _Assign<T, Integer16>(t, value) ||
                _Assign<T, UInteger16>(t, value) ||
                _Assign<T, Integer64>(t, value) ||
                _Assign<T, UInteger64>(t, value);
        }
        template<typename T>
        static bool Assign(T* t, Object* inst, FieldInfo* info)
        {
            auto _info = info->GetValueUnique(inst);
            return Assign(t, _info.get());
        }
    };
    class ParameterInfo : public TypeInfo
    {
        CORELIB_DEF_TYPE(JxCoreLib::ParameterInfo, TypeInfo);
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
        CORELIB_DEF_TYPE(JxCoreLib::MethodInfo, MemberInfo);
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
            const FieldInfo::GetterType& getter,
            const FieldInfo::SetterType& setter
        )
        {
            FieldInfo::FieldTypeInfo info;
            info.is_pointer = std::is_pointer<TField>::value;
            info.is_const = std::is_const<TField>::value;
            info.is_reference = std::is_reference<TField>::value;
            info.is_volatile = std::is_volatile<TField>::value;
            
            using CleanType = fulldecay<TField>::type;
            using ClType = get_cltype<CleanType>::type;
            Type* field_type = cltypeof<ClType>();

            cltypeof<T>()->_AddMemberInfo(new FieldInfo{ name, is_static, is_public, info, field_type, getter, setter });
        }
    };
}


#endif // !_CORELIB_REFLECTION_H
