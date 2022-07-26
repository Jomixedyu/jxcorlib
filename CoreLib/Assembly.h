#pragma once

#include "Object.h"
#include "Type.h"
#include "Events.hpp"

namespace JxCoreLib
{
    class Assembly final : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::Assembly, Object);

    public:
        Type* FindType(string_view name) const;
        sptr<List<Type*>> GetAllTypes() const;

        string get_name() const;

    public:
        
        
        static inline FunctionEvents<Action<>> OnReset;

    };
}