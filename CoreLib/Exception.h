#ifndef CORELIB_EXCEPTION_H
#define CORELIB_EXCEPTION_H

#include <exception>
#include "Object.h"
#include "Type.h"

#define DEF_EXCEPTION_CTOR(class) \
class() {} \
class(const String& message) : base(message) {}

namespace JxCoreLib
{
    class ExceptionBase : public std::exception, public Object
    {
        DEF_OBJECT_META(ExceptionBase, Object);
        DECL_OBJECT_DYNCREATEINSTANCE();
    protected:
        String message_;
        mutable String whatcache_;
    public:
        ExceptionBase() {}
        ExceptionBase(const String& message) : message_(message) {}
    public:
        virtual String get_message() const {
            return this->message_;
        }
        virtual const char* what() const override {
            this->whatcache_ = String(this->get_type()->get_name() + _T(": ") + this->message_);
            return this->whatcache_.c_str();
        }
        virtual String ToString() const override {
            return this->what();
        }
    };

    class RangeOutException : public ExceptionBase
    {
        DEF_OBJECT_META(RangeOutException, ExceptionBase);
        DECL_OBJECT_DYNCREATEINSTANCE();
    public:
        DEF_EXCEPTION_CTOR(RangeOutException)
    };

    class ArgumentException : public ExceptionBase
    {
        DEF_OBJECT_META(ArgumentException, ExceptionBase);
        DECL_OBJECT_DYNCREATEINSTANCE();
    public:
        DEF_EXCEPTION_CTOR(ArgumentException)
    };

    class ArgumentNullException : public ArgumentException
    {
        DEF_OBJECT_META(ArgumentNullException, ArgumentException);
        DECL_OBJECT_DYNCREATEINSTANCE();
    public:
        DEF_EXCEPTION_CTOR(ArgumentNullException);
    };

    class NotImplementException : public ArgumentException
    {
        DEF_OBJECT_META(NotImplementException, ArgumentException);
        DECL_OBJECT_DYNCREATEINSTANCE();
    public:
        DEF_EXCEPTION_CTOR(NotImplementException);
    };

    class NullPointerException : public ExceptionBase
    {
        DEF_OBJECT_META(NullPointerException, ExceptionBase);
        DECL_OBJECT_DYNCREATEINSTANCE();
    public:
        DEF_EXCEPTION_CTOR(NullPointerException);
    };
}
#endif // !CORELIB_EXCEPTION_H
