#include "Type.h"

#include <vector>
#include <iostream>

#include "CommonException.h"
#include "Reflection.h"

namespace JxCoreLib
{
    static std::vector<Type*>* g_types = nullptr;

    string Type::ToString() const
    {
        return this->name_;
    }

    bool Type::IsInstanceOfType(Object* object)
    {
        return object->get_type()->IsSubclassOf(this);
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

    Object* Type::CreateInstance()
    {
        if (this->c_inst_ptr_ == nullptr) {
            throw NotImplementException(this->get_name() + ": the creation method is not implemented");
        }

        return (*this->c_inst_ptr_)({});
    }

    Object* Type::CreateInstance(const ParameterPackage& v)
    {
        if (this->c_inst_ptr_ == nullptr) {
            throw NotImplementException(this->get_name() + ": the creation method is not implemented");
        }
        return (*this->c_inst_ptr_)(v);
    }

    Type* Type::GetType(const string& str)
    {
        for (auto& item : *g_types) {
            if (item->get_name() == str) {
                return item;
            }
        }
        return nullptr;
    }

    Type* Type::GetType(const char*& str)
    {
        for (auto& item : *g_types) {
            if (item->get_name() == str) {
                return item;
            }
        }
        return nullptr;
    }

    Type* Type::GetType(int id)
    {
        return g_types->at(id);
    }

    std::vector<Type*> Type::GetTypes()
    {
        return *g_types;
    }

    Type::Type(
        int id,
        const string& name,
        Type* base,
        c_inst_ptr_t c_inst_ptr,
        const std::type_info& typeinfo,
        int structure_size
    ) : id_(id), name_(name), base_(base),
        c_inst_ptr_(c_inst_ptr), typeinfo_(typeinfo), structure_size_(structure_size)
    {
    }

    Type* Type::__meta_type()
    {
        static int id = -1;
        if (id == -1) {
            id = Type::Register(nullptr, cltypeof<Object>(), _T("JxCoreLib::Type"), typeid(Type), sizeof(Type));
        }
        return Type::GetType(id);
    }

    Type* Type::get_type() const
    {
        return __meta_type();
    }

    int Type::get_structure_size() const
    {
        return sizeof(Type);
    }

    const string& Type::get_name() const
    {
        return this->name_;
    }

    Type* Type::get_base() const
    {
        return this->base_;
    }

    const std::type_info& Type::get_typeinfo() const
    {
        return this->typeinfo_;
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


    static int _Type_Get_Index()
    {
        static int i = -1;
        ++i;
        return i;
    }

    struct _ObjectTypeRegister
    {
        _ObjectTypeRegister()
        {
            g_types = new std::vector<Type*>;

        }
    };

    int Type::Register(c_inst_ptr_t dyncreate, Type* base, const string& name, const std::type_info& info, int structure_size)
    {
        int id = _Type_Get_Index();

        Type* type = new Type(id, name, nullptr, dyncreate, info, structure_size);

        static bool is_init = false;
        static _ObjectTypeRegister obj_reg;

        if (is_init)
        {
            if (base == nullptr) {
                base = cltypeof<Object>();
            }
            type->base_ = base;
        }
        else
        {
            is_init = true;
        }

        g_types->push_back(type);
        return id;
    }


    std::vector<MemberInfo*> Type::get_memberinfos(bool is_public, bool is_static)
    {
        std::vector<MemberInfo*> v;
        v.reserve(this->member_infos_.size());
        for (auto& item : this->member_infos_)
        {
            if ((item.second->is_public() == is_public)
                && (item.second->is_static() == is_static))
            {
                v.push_back(item.second);
            }
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

    std::vector<FieldInfo*> Type::get_fieldinfos(bool is_public, bool is_static)
    {
        std::vector<FieldInfo*> v;
        v.reserve(this->member_infos_.size());
        for (auto& item : this->member_infos_)
        {
            if ((item.second->get_type()->IsSubclassOf(cltypeof<FieldInfo>()))
                && (item.second->is_public() == is_public)
                && (item.second->is_static() == is_static)
                )
            {
                v.push_back(static_cast<FieldInfo*>(item.second));
            }
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
        if (!info->get_type()->IsSubclassOf(cltypeof<FieldInfo>()))
        {
            return nullptr;
        }
        return static_cast<FieldInfo*>(info);
    }

    std::vector<MethodInfo*> Type::get_methodinfos(bool is_public, bool is_static)
    {
        std::vector<MethodInfo*> v;
        v.reserve(this->member_infos_.size());
        for (auto& item : this->member_infos_)
        {
            if ((item.second->get_type()->IsSubclassOf(cltypeof<MethodInfo>()))
                && (item.second->is_public() == is_public)
                && (item.second->is_static() == is_static)
                )
            {
                v.push_back(static_cast<MethodInfo*>(item.second));
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
        if (!info->get_type()->IsSubclassOf(cltypeof<MethodInfo>()))
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
