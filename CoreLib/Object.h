#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include "String.h"

#define DEF_TYPE_INIT(NAME) static struct NAME##_Init{NAME##_Init(){NAME::__meta_type();}} NAME##_Init_;

namespace JxCoreLib
{
    class Type;

    class Object
    {
    public:
        static Type* __meta_type();
        virtual Type* get_type() const;
    public:
        Object();
    public:
        virtual String ToString() const;
        static bool Equals(const Object* x, const Object* y);
        virtual bool Equals(Object* target) const;
    };

#ifdef CORELIB_AUTOINIT
    DEF_TYPE_INIT(Object)
#endif

}
#endif // !CORELIB_OBJECT_H
