#ifndef JXCORELIB_EXCEPTIONBASE_H
#define JXCORELIB_EXCEPTIONBASE_H

#include <string>
#include "Object.h"

#define DEF_EXCEPTION_CTOR(class, parent_) \
class() {} \
class(const String& message) : parent_(message) {} \
class(const RefString& message) : parent_(message) {}

class ExceptionBase : public Object
{
    DEF_OBJECT_META(ExceptionBase, Object)
protected:
    String message_;
public:
    ExceptionBase() {}
    ExceptionBase(const String& message) : message_(message) {}
    ExceptionBase(const RefString& message) : message_(message) {}
public:
    virtual String what() const {
        return this->get_type()->get_name() + _T(" : ") + this->message_;
    }
    virtual String to_string() const override {
        return this->what();
    }
};

class RangeOutException : public ExceptionBase
{
    DEF_OBJECT_META(RangeOutException, RangeOutException)
public:
    DEF_EXCEPTION_CTOR(RangeOutException, ExceptionBase)
};

class ArgumentException : public ExceptionBase
{
    DEF_OBJECT_META(ArgumentException, ExceptionBase)
public:
    DEF_EXCEPTION_CTOR(ArgumentException, ExceptionBase)
};

class ArgumentNullException : public ArgumentException
{
    DEF_OBJECT_META(ArgumentNullException, ArgumentException)
public:
    DEF_EXCEPTION_CTOR(ArgumentNullException, ArgumentException)
};

class NotImplementException : public ArgumentException
{
    DEF_OBJECT_META(NotImplementException, ArgumentException)
public:
    DEF_EXCEPTION_CTOR(NotImplementException, ArgumentException)
};

#endif