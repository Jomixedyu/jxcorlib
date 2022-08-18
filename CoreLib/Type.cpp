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

    bool Type::IsImplementedInterface(Type* type)
    {
        for (auto& [item, fun, sfun]: this->interfaces_)
        {
            if (type->IsSubclassOf(item))
            {
                return true;
            }
        }
        return false;
    }

    //array_list<Type*> Type::GetInterfaces() const
    //{
    //    return this->interfaces_;
    //}

    string Type::ToString() const
    {
        return this->name_;
    }

    bool Type::IsInstanceOfType(const Object* object) const
    {
        return object->GetType()->IsSubclassOf(this);
    }
    bool Type::IsSharedInstanceOfType(const sptr<Object>& ptr) const
    {
        return this->IsInstanceOfType(ptr.get());
    }

    bool Type::IsSubclassOf(const Type* type) const
    {
        const Type* base = this;
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
            type = new Type(nullptr, assm, cltypeof<Object>(), "JxCoreLib::Type", typeid(Object), sizeof(Object));
            assm->RegisterType(type);
        }
        return type;
    }

    bool Type::is_primitive_type() const
    {
        return this->IsSubclassOf(cltypeof<PrimitiveObject>());
    }

    bool Type::is_boxing_type() const
    {
        return this->IsSubclassOf(cltypeof<BoxingObject>());
    }

    bool Type::is_custom_primitive_type() const
    {
        return this->IsSubclassOf(cltypeof<CustomPrimitiveObject>());
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
        if (this->get_base())
        {
            auto infos = this->get_base()->get_fieldinfos(attr);
            v.insert(v.end(), infos.begin(), infos.end());
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

    Type* IInterface::StaticType()
    {
        static Type* type = nullptr;
        if (type == nullptr)
        {
            Assembly* assm = ::JxCoreLib::Assembly::StaticBuildAssembly(AssemblyObject_JxCoreLib);
            type = new Type(nullptr, assm, cltypeof<Object>(), "JxCoreLib::IInterface", typeid(IInterface), sizeof(IInterface));
            type->is_interface_ = true;
            assm->RegisterType(type);
        }
        return type;
    }

}
