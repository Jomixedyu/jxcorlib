#include "Object.h"
#include <vector>
#include <map>
#include "Type.h"
#include "Assembly.h"

namespace JxCoreLib
{

    static Object* CreateInstance(const ParameterPackage&)
    {
        return new Object;
    }

    Type* Object::StaticType()
    {
        static Type* type = nullptr;
        if (type == nullptr)
        {
            Assembly* assm = Assembly::StaticBuildAssembly(AssemblyObject_JxCoreLib);
            Type* type = new Type(CreateInstance, assm, nullptr, "JxCoreLib::Object", typeid(Object), sizeof(Object));
            assm->RegisterType(type);
        }
        return type;
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

    bool Object::Equals(const sptr<Object>& object) const
    {
        return this == object.get();
    }


}

std::string std::to_string(JxCoreLib::Object* obj)
{
    return obj->ToString();
}
