#include <vector>
#include <map>
#include "Object.h"

static std::vector<Type*> g_types;

static int _Type_Get_Index()
{
    static int i = -1;
    ++i;
    return i;
}

int _Type_Register(Type* type, Type* base, const String& name)
{
    type->name_ = name;

    static String _object = _T("Object");

    if (base == nullptr && name != _object) {
        base = typeof<Object>();
    }

    type->base_ = base;

    g_types.push_back(type);
    return _Type_Get_Index();
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

Type* Type::GetType(const RefString& str)
{
    for (auto& item : g_types) {
        if (item->get_name() == str) {
            return item;
        }
    }
    return nullptr;
}

Type* Type::GetType(const String& str)
{
    for (auto& item : g_types) {
        if (item->get_name() == str) {
            return item;
        }
    }
    return nullptr;
}

Type* Type::GetType(const StringPointer& str)
{
    const RefString rs(str);
    for (auto& item : g_types) {
        if (item->get_name() == rs) {
            return item;
        }
    }
    return nullptr;
}

Type* Type::GetType(int id)
{
    return g_types[id];
}
