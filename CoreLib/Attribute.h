#ifndef CORELIB_ATTRIBUTE_H
#define CORELIB_ATTRIBUTE_H

#include "OOPCore.h"
namespace JxCoreLib
{
    enum class MemberType
    {
        Class,
        Field,
        Method
    };

    template<typename T, MemberType memberType, typename Tag>
    struct $Attribute
    {

    };

    class Attribute : public Object
    {
        DEF_OBJECT_TYPE(Attribute, Object);

    };

#define $ATTRIBUTE(src, type, attribute) 
}

#endif // !CORELIB_ATTRIBUTE_H
