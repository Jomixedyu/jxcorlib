#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include "String.h"
#include "CoreLibConfig.h"

#define DEF_TYPE_INIT(NAME) static struct NAME##_Init{NAME##_Init(){NAME::__meta_type();}} __##NAME##_Init_;

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
        virtual string ToString() const;
    };

#ifdef CORELIB_AUTOINIT
    DEF_TYPE_INIT(Object);
#endif

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
#endif // !CORELIB_OBJECT_H
