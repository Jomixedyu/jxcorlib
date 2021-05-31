#ifndef CORELIB_OOPCORE_H
#define CORELIB_OOPCORE_H

#include "String.h"
#include "Object.h"
#include "Type.h"
#include "Exception.h"

#define DEF_OBJECT_DYNCREATEINSTANCE_FUNCBODY \
        throw (__meta_type()->get_name() + ", the creation method is not implemented");

#define DEF_OBJECT_DYNCREATEINSTANCE() \
        static Object* DynCreateInstance(const ParameterPackage& params) \
        { DEF_OBJECT_DYNCREATEINSTANCE_FUNCBODY }

#define DEF_OBJECT_TYPE(name, base) \
        DEF_OBJECT_META(name, base) \
        DEF_OBJECT_DYNCREATEINSTANCE()



#endif // !CORELIB_OOPCORE_H