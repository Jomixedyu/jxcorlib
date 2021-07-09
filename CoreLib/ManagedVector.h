#ifndef _CORELIB_MANAGEDVECTOR_H
#define _CORELIB_MANAGEDVECTOR_H

#include "Object.h"
#include "Type.h"
#include <vector>

namespace JxCoreLib
{
    template<baseof_object_pointer_concept T>
    class ManagedVector : public Object, public std::vector<T>
    {
    public:
        using type = std::vector<T>;
        using type::type;

        virtual ~ManagedVector()
        {
            for (auto& item : *this)
            {
                delete item;
            }
        }
    };

}

#endif