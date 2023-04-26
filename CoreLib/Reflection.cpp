#include "Reflection.h"
#include "Attribute.h"

namespace jxcorlib
{
    sptr<Attribute> TypeInfo::GetAttribute(Type* type)
    {
        for (auto& attr : this->m_attributes)
        {
            if (type->IsInstanceOfType(attr.get()))
            {
                return attr;
            }
        }
        return nullptr;
    }
    array_list<sptr<Attribute>> TypeInfo::GetAttributes(Type* type)
    {
        array_list<sptr<Attribute>> ret;
        for (auto& attr : this->m_attributes)
        {
            if (type->IsInstanceOfType(attr.get()))
            {
                ret.push_back(attr);
            }
        }
        return ret;
    }

    bool TypeInfo::IsDefinedAttribute(Type* type)
    {
        return GetAttribute(type) != nullptr;
    }

    FieldInfo::FieldInfo(
        const string& name, bool is_public,
        FieldTypeInfo info, Type* field_type,
        const GetterFunction& getter,
        const SetterFunction& setter
    )
        : base(name, is_public),
        m_info(info), m_fieldType(field_type), m_getter(getter), m_setter(setter)
    {

    }



    void FieldInfo::SetValue(Object* instance, sptr<Object> value)
    {
        this->m_setter(instance, value);
    }

    sptr<Object> FieldInfo::GetValue(Object* instance) const
    {
        return this->m_getter(instance);
    }

    MemberInfo::MemberInfo(const string& name, bool is_public)
        : m_name(name), m_isPublic(is_public)
    {
    }

    MethodInfo::MethodInfo(
        const string& name, bool is_public,
        ParameterInfo* ret_type, array_list<ParameterInfo*>&& params_infos, bool is_abstract
    ) : base(name, is_public),
        ret_type_(ret_type), param_types_(params_infos), is_abstract_(is_abstract)
    {
    }

    Object_sp MethodInfo::Invoke(Object* instance, array_list<Object_sp>&& params) const
    {

        return nullptr;
    }


}