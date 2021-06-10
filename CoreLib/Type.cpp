#include "Type.h"
#include <vector>
#include <iostream>

#include "CoreLib.h"

namespace JxCoreLib
{
    static std::vector<Type*>* g_types = nullptr;

    string Type::ToString() const
    {
        return this->name_;
    }

    bool Type::IsInstanceOfType(Object* object)
    {
        Type* type = object->get_type();
        return type->IsSubclassOf(this);
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

    Object* Type::CrearteInstance()
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

    Type::Type(int id, const string& name, Type* base, c_inst_ptr_t c_inst_ptr, int structure_size)
        : id_(id), name_(name), base_(base), c_inst_ptr_(c_inst_ptr), structure_size_(structure_size)
    {
    }

    Type* Type::__meta_type()
    {
        static int id = -1;
        if (id == -1) {
            id = Type::Register(nullptr, typeof<Object>(), _T("JxCoreLib::Type"), sizeof(Type));
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


    static int _Type_Get_Index()
    {
        static int i = -1;
        ++i;
        return i;
    }

    static struct _ObjectTypeRegister
    {
        _ObjectTypeRegister()
        {
            g_types = new std::vector<Type*>;

        }
    };

    int Type::Register(c_inst_ptr_t dyncreate, Type* base, const string& name, int structure_size)
    {
        int id = _Type_Get_Index();
        Type* type = new Type(id, name, nullptr, dyncreate, structure_size);

        static bool is_init = false;
        static _ObjectTypeRegister obj_reg;

        if (is_init)
        {
            if (base == nullptr) {
                base = typeof<Object>();
            }
            type->base_ = base;
        }
        else [[unlikely]]
        {
            is_init = true;
        }

        g_types->push_back(type);
        return id;
    }

    Object* ParameterPackage::DynCreateInstance(const ParameterPackage& params)
    {
        CORELIB_IMPL_DYNCREATEINSTANCE_FUNCBODY();
    }
}
