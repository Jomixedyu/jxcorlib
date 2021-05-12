#include <vector>
#include <map>
#include "Object.h"
#include "Type.h"

class _Object_Type : public Type
{
    virtual Object* CreateInstance() {
        return new Object;
    }
};

static Object* CreateInstance(CreateInstParamData*)
{
    return new Object;
}

Type* Object::__meta_type()
{
    static int id = -1;
    if (id == -1) {
        id = Type::Register(CreateInstance, nullptr, _T("Object"), sizeof(Object));
    }
    return Type::GetType(id);
}

Type* Object::get_type() const
{
    return __meta_type();
}

Object::Object()
{
}

String Object::ToString() const
{
    return this->get_type()->get_name();
}

bool Object::Equals(const Object* x, const Object* y)
{
    return x == y;
}

bool Object::Equals(Object* target) const
{
    return Object::Equals(this, target);
}