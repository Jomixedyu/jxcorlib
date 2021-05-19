#include "Type.h"
#include <vector>
#include <iostream>

static std::vector<Type*>* g_types;

String Type::ToString() const
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

Object* Type::CreateInstance(CreateInstParamData* v)
{
    if (this->c_inst_ptr_ == nullptr) {
        throw std::exception("the creation method is not implemented");
    }
    return this->c_inst_ptr_(v);
}

Type* Type::GetType(const String& str)
{
    for (auto& item : *g_types) {
        if (item->get_name() == str) {
            return item;
        }
    }
    return nullptr;
}

Type* Type::GetType(const StringPointer& str)
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

Type::Type(int id, CRString name, Type* base, c_inst_ptr_t c_inst_ptr, int structure_size)
    : id_(id), name_(name), base_(base), c_inst_ptr_(c_inst_ptr), structure_size_(structure_size)
{
}

Type* Type::__meta_type()
{
    static int id = -1;
    if (id == -1) {
        id = Type::Register(nullptr, typeof<Object>(), _T("Type"), sizeof(Type));
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

const String& Type::get_name() const
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

int Type::Register(Object* (*dynCreate)(CreateInstParamData*), Type* base, const String& name, int structure_size)
{
    int id = _Type_Get_Index();
    Type* type = new Type(id, name, base, dynCreate, structure_size);

    static String object = _T("Object");

    if (base == nullptr && name != object) {
        base = typeof<Object>();
    }

    type->base_ = base;

    //std::cout << name << std::endl;

    if (g_types == nullptr) {
        g_types = new std::vector<Type*>;
    }
    g_types->push_back(type);
    return id;
}

