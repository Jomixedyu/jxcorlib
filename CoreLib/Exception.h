#ifndef CORELIB_EXCEPTION_H
#define CORELIB_EXCEPTION_H

#include "OOPCore.h"

#define DEF_EXCEPTION_CTOR(class) \
class() {} \
class(const String& message) : base(message) {} \
class(const RefString& message) : base(message) {}

namespace JxCoreLib
{
    class ExceptionBase : public Object
    {
        DEF_OBJECT_TYPE(ExceptionBase, Object)
    protected:
        String message_;
    public:
        ExceptionBase() {}
        ExceptionBase(const String& message) : message_(message) {}
    public:
        virtual String What() const {
            return this->get_type()->get_name() + _T(" : ") + this->message_;
        }
        virtual String ToString() const override {
            return this->What();
        }
    };

    class RangeOutException : public ExceptionBase
    {
        DEF_OBJECT_TYPE(RangeOutException, ExceptionBase)
    public:
        DEF_EXCEPTION_CTOR(RangeOutException)
    };

    class ArgumentException : public ExceptionBase
    {
        DEF_OBJECT_TYPE(ArgumentException, ExceptionBase)
    public:
        DEF_EXCEPTION_CTOR(ArgumentException)
    };

    class ArgumentNullException : public ArgumentException
    {
        DEF_OBJECT_TYPE(ArgumentNullException, ArgumentException)
    public:
        DEF_EXCEPTION_CTOR(ArgumentNullException)
    };

    class NotImplementException : public ArgumentException
    {
        DEF_OBJECT_TYPE(NotImplementException, ArgumentException)
    public:
        DEF_EXCEPTION_CTOR(NotImplementException)
    };

}
#endif // !CORELIB_EXCEPTION_H
