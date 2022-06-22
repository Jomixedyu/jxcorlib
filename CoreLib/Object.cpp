#include "Object.h"
#include <vector>
#include <map>
#include "Type.h"

namespace JxCoreLib
{

    static Object* CreateInstance(const ParameterPackage&)
    {
        return new Object;
    }

    Type* Object::StaticType()
    {
        static int id = -1;
        if (id == -1) {
            id = Type::Register(CreateInstance, nullptr, "JxCoreLib::Object", typeid(Object), sizeof(Object));
        }
        return Type::GetType(id);
    }

    Type* Object::GetType() const
    {
        return StaticType();
    }

    Object::~Object()
    {
    }

    string Object::ToString() const
    {
        return this->GetType()->get_name();
    }

}

std::string std::to_string(JxCoreLib::Object* obj)
{
    return obj->ToString();
}
