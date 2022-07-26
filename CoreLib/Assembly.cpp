#include "Assembly.h"

namespace JxCoreLib
{

    Type* Assembly::FindType(string_view name) const
    {
        return nullptr;
    }
    sptr<List<Type*>> Assembly::GetAllTypes() const
    {
        return ArrayList<Type*>();
    }
}

