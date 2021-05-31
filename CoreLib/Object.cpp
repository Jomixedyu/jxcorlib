#include <vector>
#include <map>
#include "Object.h"
#include "Type.h"

namespace JxCoreLib
{

    static Object* CreateInstance(const ParameterPackage&)
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

}