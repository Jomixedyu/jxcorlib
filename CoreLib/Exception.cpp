#include "Exception.h"
#include "OOPCore.h"

#define DECL_FUNC_NAME() DynCreateInstance(const ParameterPackage& params)

#define DEF_EXCEPTION_DYN(T) Object* T::DECL_FUNC_NAME(){ return new T(""); }

namespace JxCoreLib
{
    DEF_EXCEPTION_DYN(ExceptionBase);
    DEF_EXCEPTION_DYN(RangeOutException);
    DEF_EXCEPTION_DYN(ArgumentException);
    DEF_EXCEPTION_DYN(ArgumentNullException);
    DEF_EXCEPTION_DYN(NotImplementException);
    DEF_EXCEPTION_DYN(NullPointerException);


}


