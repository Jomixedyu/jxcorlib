#include "Type.h"

#include <vector>
#include <iostream>

#include "CommonException.h"
#include "Reflection.h"
#include "Assembly.h"
#include "BasicTypes.h"

namespace JxCoreLib
{
    static std::vector<Type*>* g_types = nullptr;

    string Type::ToString() const
    {
        return this->name_;
    }

    bool Type::IsInstanceOfType(Object* object)
    {
        return object->GetType()->IsSubclassOf(this);
    }

    bool Type::IsSubclassOf(Type* type)
    {
        Type* base = this;
        while (base != nullptr)
        {
            if (base == type) {
                return true;
            }
            else {
                base = base->get_base();
            }
        }
        return false;
    }

    Object* Type::CreateInstance(const ParameterPackage& v)
    {
        if (this->c_inst_ptr_ == nullptr) {
            throw NotImplementException(this->get_name() + ": the creation method is not implemented");
        }

        return (*this->c_inst_ptr_)(v);
    }

    sptr<Object> Type::CreateSharedInstance(const ParameterPackage& v)
    {
        if (this->c_inst_ptr_ == nullptr) {
            throw NotImplementException(this->get_name() + ": the creation method is not implemented");
        }
        return mksptr((*this->c_inst_ptr_)(v));
    }


    Type* Type::StaticType()
    {
        static Type* type = nullptr;
        if (type == nullptr)
        {
            Assembly* assm = Assembly::StaticBuildAssembly(AssemblyObject_JxCoreLib);
            Type* type = new Type(nullptr, assm, cltypeof<Object>(), "JxCoreLib::Type", typeid(Object), sizeof(Object));
            assm->RegisterType(type);
        }
        return type;
    }

    bool Type::is_primitive_type() const
    {
        return
            this == cltypeof<String>() ||
            this == cltypeof<CharType>() ||
            this == cltypeof<Integer8>() ||
            this == cltypeof<UInteger8>() ||
            this == cltypeof<Integer16>() ||
            this == cltypeof<UInteger16>() ||
            this == cltypeof<Integer32>() ||
            this == cltypeof<UInteger32>() ||
            this == cltypeof<Integer64>() ||
            this == cltypeof<UInteger64>() ||
            this == cltypeof<Single32>() ||
            this == cltypeof<Double64>() ||
            this == cltypeof<Boolean>();
    }


    std::vector<MemberInfo*> Type::get_memberinfos(TypeBinding attr)
    {
        std::vector<MemberInfo*> v;
        v.reserve(this->member_infos_.size());

        for (auto& [name, info] : this->member_infos_)
        {
            if (!EnumHasFlag(attr, TypeBinding::NonPublic))
            {
                if (!info->is_public_) continue;
            }
            if (EnumHasFlag(attr, TypeBinding::Static))
            {
                if (!info->is_static_) continue;
            }

            v.push_back(info);
        }
        return v;
    }

    MemberInfo* Type::get_memberinfo(const string& name)
    {
        if (this->member_infos_.find(name) == this->member_infos_.end())
        {
            return nullptr;
        }
        return this->member_infos_.at(name);
    }

    std::vector<FieldInfo*> Type::get_fieldinfos(TypeBinding attr)
    {
        std::vector<FieldInfo*> v;
        v.reserve(this->member_infos_.size());

        Type* fieldinfo_type = cltypeof<FieldInfo>();

        for (auto& [name, info] : this->member_infos_)
        {
            if (!fieldinfo_type->IsInstanceOfType(info))
            {
                continue;
            }
            if (!EnumHasFlag(attr, TypeBinding::NonPublic))
            {
                if (!info->is_public_) continue;
            }
            if (EnumHasFlag(attr, TypeBinding::Static))
            {
                if (!info->is_static_) continue;
            }
            v.push_back(static_cast<FieldInfo*>(info));
        }
        return v;
    }

    FieldInfo* Type::get_fieldinfo(const string& name)
    {
        if (this->member_infos_.find(name) == this->member_infos_.end())
        {
            return nullptr;
        }
        MemberInfo* info = this->member_infos_.at(name);
        if (!info->GetType()->IsSubclassOf(cltypeof<FieldInfo>()))
        {
            return nullptr;
        }
        return static_cast<FieldInfo*>(info);
    }

    std::vector<MethodInfo*> Type::get_methodinfos(TypeBinding attr)
    {
        std::vector<MethodInfo*> v;
        v.reserve(this->member_infos_.size());

        Type* methodi_type = cltypeof<MethodInfo>();

        for (auto& [name, info] : this->member_infos_)
        {
            if (!methodi_type->IsInstanceOfType(info))
            {
                continue;
            }
            if (!EnumHasFlag(attr, TypeBinding::NonPublic))
            {
                if (!info->is_public_) continue;
            }
            if (EnumHasFlag(attr, TypeBinding::Static))
            {
                if (!info->is_static_) continue;
            }
        }
        return v;
    }

    MethodInfo* Type::get_methodinfo(const string& name)
    {
        if (this->member_infos_.find(name) == this->member_infos_.end())
        {
            return nullptr;
        }
        MemberInfo* info = this->member_infos_.at(name);
        if (!info->GetType()->IsSubclassOf(cltypeof<MethodInfo>()))
        {
            return nullptr;
        }
        return static_cast<MethodInfo*>(info);
    }
    void Type::_AddMemberInfo(MemberInfo* info)
    {
        this->member_infos_.insert({ info->get_name(), info });
    }

}
