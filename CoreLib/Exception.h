#ifndef CORELIB_EXCEPTION_H
#define CORELIB_EXCEPTION_H

#include <exception>
#include "Object.h"
#include "Type.h"

#define DEF_EXCEPTION_CTOR(class) \
class() {} \
class(const string& message) : base(message) {}

namespace JxCoreLib
{
    class ExceptionBase : public std::exception, public Object
    {
        CORELIB_DEF_META(JxCoreLib::ExceptionBase, Object);
        CORELIB_DECL_DYNCINST();
    protected:
        string message_;
        mutable string whatcache_;
    public:
        ExceptionBase() {}
        ExceptionBase(const string& message) : message_(message) {}
    public:
        virtual string get_message() const {
            return this->message_;
        }
        virtual const char* what() const override {
            this->whatcache_ = string(this->get_type()->get_name() + _T(": ") + this->message_);
            return this->whatcache_.c_str();
        }
        virtual string ToString() const override {
            return this->what();
        }
    };

    class RangeOutException : public ExceptionBase
    {
        CORELIB_DEF_META(JxCoreLib::RangeOutException, ExceptionBase);
        CORELIB_DECL_DYNCINST();
    public:
        DEF_EXCEPTION_CTOR(RangeOutException)
    };

    class ArgumentException : public ExceptionBase
    {
        CORELIB_DEF_META(JxCoreLib::ArgumentException, ExceptionBase);
        CORELIB_DECL_DYNCINST();
    public:
        DEF_EXCEPTION_CTOR(ArgumentException)
    };

    class ArgumentNullException : public ArgumentException
    {
        CORELIB_DEF_META(JxCoreLib::ArgumentNullException, ArgumentException);
        CORELIB_DECL_DYNCINST();
    public:
        DEF_EXCEPTION_CTOR(ArgumentNullException);
    };

    class NotImplementException : public ArgumentException
    {
        CORELIB_DEF_META(JxCoreLib::NotImplementException, ArgumentException);
        CORELIB_DECL_DYNCINST();
    public:
        DEF_EXCEPTION_CTOR(NotImplementException);
    };

    class NullPointerException : public ExceptionBase
    {
        CORELIB_DEF_META(JxCoreLib::NullPointerException, ExceptionBase);
        CORELIB_DECL_DYNCINST();
    public:
        DEF_EXCEPTION_CTOR(NullPointerException);
    };
}
#endif // !CORELIB_EXCEPTION_H
