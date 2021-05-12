#ifndef CORELIB_OOPCORE_H
#define CORELIB_OOPCORE_H

#include "String.h"
#include "Object.h"
#include "Type.h"

#define DEF_OBJECT_META(NAME, BASE) \
public: \
    inline static Type* __meta_type() { \
        static int id = -1; \
        if (id == -1) { \
            id = Type::Register(DynCreateInstance, typeof<BASE>(), _T(NAMEOF(NAME)), sizeof(NAME)); \
        } \
        return Type::GetType(id); \
    } \
public: \
    inline virtual Type* get_type() const override { \
        return __meta_type(); \
    } \
private: \
    inline static struct NAME##_TypeInit { NAME##_TypeInit() { NAME::__meta_type(); } } _type_init_; \
    using base = BASE; \

#define DEF_OBJECT_DYNCREATEINSTANCE() \
        static Object* DynCreateInstance(CreateInstParamData* params) \
        { throw std::exception("the creation method is not implemented"); }

#define DEF_OBJECT_TYPE(name, base) \
        DEF_OBJECT_META(name, base) \
        DEF_OBJECT_DYNCREATEINSTANCE()

#define DECL_TOSTRING()   virtual String ToString() const override
#define DECL_EQUALS()     virtual bool Equals(Object* target) const override
#define DECL_OBJECT_DYNCREATEINSTANCE() \
    static Object* DynCreateInstance(CreateInstParamData* params)

#endif // !CORELIB_OOPCORE_H