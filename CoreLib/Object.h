#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include "String.h"
#include "CoreLibConfig.h"

#define DEF_TYPE_INIT(NAME) static inline struct NAME##_Init{NAME##_Init(){NAME::__meta_type();}} __##NAME##_Init_;

namespace JxCoreLib
{
    class Type;

    class Object
    {
    private:
        static Type* __meta_type();
#ifdef CORELIB_AUTOINIT
        DEF_TYPE_INIT(Object);
#endif
    public:
        virtual Type* get_type() const;
        friend class Type;
    public:
        Object();
    public:
        virtual string ToString() const;
    };

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
#endif // !CORELIB_OBJECT_H
