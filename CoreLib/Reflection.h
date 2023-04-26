/*
* @Moudule     : Reflection
* @Date        : 2021/05/23
* @Author      : JomiXedYu
* @Description : This file is part of jxcorlib : https://github.com/JomiXedYu/jxcorlib
* @StdRequired : c++20
*/

#pragma once

#include "UString.h"
#include "Object.h"
#include "Type.h"
#include "Assembly.h"
#include "BasicTypes.h"
#include "Enum.h"

#include <functional>
#include <any>
#include <memory>


//支持 基础值类型 系统内指针（包括智能指针）类型
#define CORELIB_REFL_DECL_FIELD(NAME, ...) \
    static inline struct __corelib_refl_##NAME \
    { \
        template<typename T> using _Detected = decltype(std::declval<T&>().NAME); \
        __corelib_refl_##NAME() \
        { \
            using FieldType = decltype(std::declval<ThisClass&>().NAME); \
            using DecayType = fulldecay<FieldType>::type; \
            using CleanType = remove_shared_ptr<DecayType>::type; \
            using ClType = get_boxing_type<remove_shared_ptr<std::remove_cv<FieldType>::type>::type>::type; \
            ReflectionBuilder::CreateFieldInfo<ThisClass, FieldType>( \
                #NAME, jxcorlib::is_detected<_Detected, ThisClass>::value, \
                [](Object* p) -> sptr<Object> { \
                    auto rawptr = (ThisClass*)p; \
                    return get_object_pointer<CleanType>::get(rawptr->NAME); \
                }, \
                [](Object* p, sptr<Object> value) { \
                    auto rawptr = (ThisClass*)p; \
                    object_assign<CleanType>::assign(&rawptr->NAME, value); \
                }, \
            {__VA_ARGS__} ); \
        } \
    } __corelib_refl_##NAME##_;

#define CORELIB_REFL_DECL_FUNC(NAME) \
    static inline struct __corelib_refl_##NAME \
    { \
        __corelib_refl_##NAME() \
        { \
            array_list<ParameterInfo*> infos; \
            ReflectionBuilder::CreateMethodParameterInfos(NAME, &infos); \
        } \
    } __corelib_refl_##NAME##_;

namespace jxcorlib
{
    class Attribute;

    class TypeInfo : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::TypeInfo, Object);
        friend class ReflectionBuilder;
    public:
        TypeInfo() {}
        TypeInfo(const TypeInfo&) = delete;
        TypeInfo(TypeInfo&&) = delete;
    public:
        sptr<Attribute>             GetAttribute(Type* type);
        array_list<sptr<Attribute>> GetAttributes(Type* type);
        bool                        IsDefinedAttribute(Type* type);
    private:
        array_list<sptr<Attribute>> m_attributes;
    };

    class MemberInfo : public TypeInfo
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::MemberInfo, TypeInfo);
        friend class ReflectionBuilder;
    protected:
        string   m_name;
        bool     m_isPublic;

    public:
        const string& GetName() const { return this->m_name; }
        bool IsPublic() const { return this->m_isPublic; }
        
    public:
        MemberInfo(const string& name, bool is_public);
        MemberInfo(const MemberInfo& right) = delete;
        MemberInfo(MemberInfo&& right) = delete;
    };

    class FieldInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::FieldInfo, MemberInfo);
        friend class ReflectionBuilder;
    public:
        struct FieldTypeInfo
        {
            bool IsRawPointer;
            bool IsSharedPointer;
            bool IsConst;
        };
        using GetterFunction = std::function<sptr<Object>(Object* instance)>;
        using SetterFunction = std::function<void(Object* instance, sptr<Object> value)>;
    protected:
        FieldTypeInfo    m_info;
        Type*            m_fieldType;
        GetterFunction   m_getter;
        SetterFunction   m_setter;
    public:
        Type* get_field_type() const { return this->m_fieldType; }
        bool IsRawPointer()    const { return this->m_info.IsRawPointer; }
        bool IsSharedPointer() const { return this->m_info.IsSharedPointer; }
        bool IsPointer()       const { return IsRawPointer() || IsSharedPointer(); }
        bool IsConst()         const { return this->m_info.IsConst; }
    public:
        FieldInfo(
            const string& name, bool is_public,
            FieldTypeInfo info, Type* field_type,
            const GetterFunction& getter, const SetterFunction& setter);

        FieldInfo(const FieldInfo& right) = delete;
        FieldInfo(FieldInfo&& right)      = delete;
    public:
        void         SetValue(Object* instance, sptr<Object> value);
        sptr<Object> GetValue(Object* instance) const;

    };
    class ParameterInfo : public TypeInfo
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::ParameterInfo, TypeInfo);
        friend class ReflectionBuilder;
    protected:
        Type* m_paramType;
        bool m_isPointer;
        bool m_isConst;
        bool m_isRef;
        bool m_isRref;
    public:
        Type* GetParamType() const { return this->m_paramType; }
        bool IsPointer() const { return this->m_isPointer; }
        bool IsConst() const { return this->m_isConst; }
        bool IsReference() const { return this->m_isRef; }
        bool IsRReference() const { return this->m_isRref; }
    public:
        ParameterInfo(const ParameterInfo&) = delete;
        ParameterInfo(ParameterInfo&&) = delete;
        ParameterInfo(
            Type* m_paramType,
            bool isPointer,
            bool isConst,
            bool isRef,
            bool isRref)
            : m_paramType(m_paramType),
            m_isPointer(isPointer),
            m_isConst(isConst),
            m_isRef(isRef),
            m_isRref(isRref) {}
    };

    //TODO
    class MethodInfo final : public MemberInfo
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::MethodInfo, MemberInfo);
        friend class ReflectionBuilder;
    protected:
        array_list<ParameterInfo*> param_types_;
        ParameterInfo* ret_type_;
        bool is_abstract_;
    public:
        const std::vector<ParameterInfo*>& get_parameter_infos() const noexcept { return this->param_types_; }
        ParameterInfo* get_return_type() const noexcept { return this->ret_type_; }
        bool is_abstract() const { return this->is_abstract_; }
    public:
        MethodInfo(
            const string& name, bool is_public,
            ParameterInfo* ret_type, array_list<ParameterInfo*>&& params_infos, bool is_abstract);
        MethodInfo(const MethodInfo& right) = delete;
        MethodInfo(MethodInfo&& right) = delete;
    public:
        Object_sp Invoke(Object* instance, array_list<Object_sp>&& params) const;
    };


    class ReflectionBuilder
    {
    private:

    public:
        template<typename T, typename TField>
        static inline void CreateFieldInfo(
            const string& name,
            bool is_public,
            const FieldInfo::GetterFunction& getter,
            const FieldInfo::SetterFunction& setter,
            std::initializer_list<class Attribute*> attributeList
        )
        {
            FieldInfo::FieldTypeInfo info;
            info.IsRawPointer = std::is_pointer<TField>::value;
            info.IsConst = std::is_const<TField>::value;
            info.IsSharedPointer = is_shared_ptr<TField>::value;

            using ClType = get_boxing_type<typename fulldecay<TField>::type>::type;
            Type* fieldType = cltypeof<ClType>();

            FieldInfo* fieldInfo = new FieldInfo{ name, is_public, info, fieldType, getter, setter };

            for (auto& attr : attributeList)
            {
                static_cast<TypeInfo*>(fieldInfo)->m_attributes.push_back(mksptr(attr));
            }

            cltypeof<T>()->_AddMemberInfo(fieldInfo);
        }

    private:

        static void _GetParameters(array_list<ParameterInfo*>* out) {}

        template<typename T, typename... P>
        static void _GetParameters(array_list<ParameterInfo*>* out, T* t, P*... params)
        {
            using Box = typename get_boxing_type<T>::type;
            out->push_back(new ParameterInfo(cltypeof<Box>(),
                std::is_pointer_v<Box>,
                std::is_const_v<Box>,
                std::is_reference_v<Box>,
                std::is_rvalue_reference_v<Box>));
            _GetParameters(out, params...);
        }

        template<typename T>
        static constexpr T* GenNullptr() { return (T*)nullptr; }
    public:

        template<typename R, typename... P>
        static void CreateMethodParameterInfos(R(*p)(P...), array_list<ParameterInfo*>* out)
        {
            _GetParameters(out, GenNullptr<P>()...);
        }


        static void CreateMethodInfo(Type* type, const string& name, bool is_public, array_list<ParameterInfo*>&& info)
        {
            //todo: return value
            type->_AddMemberInfo(new MethodInfo(name, is_public, nullptr, std::move(info), false));
        }
    };
}
