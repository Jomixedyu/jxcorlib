#ifndef CORELIB_EXCEPTIONBASE_H
#define CORELIB_EXCEPTIONBASE_H

#include <string>
#include "Object.h"

#define DEF_EXCEPTION_CTOR(class, parent_) \
class() {} \
class(const String& message) : parent_(message) {} \
class(const RefString& message) : parent_(*message) {}

class ExceptionBase : public Object
{
    DEF_OBJECT_TYPE(ExceptionBase, Object)
protected:
    String message_;
public:
    ExceptionBase() {}
    ExceptionBase(const String& message) : message_(message) {}
    ExceptionBase(const RefString& message) : message_(*message) {}
public:
    virtual RefString what() const {
        return NewString(*this->get_type()->get_name() + L" : " + this->message_);
    }
    virtual RefString to_string() const override {
        return this->what();
    }
};

class RangeOutException : public ExceptionBase
{
    DEF_OBJECT_TYPE(RangeOutException, RangeOutException)
public:
    DEF_EXCEPTION_CTOR(RangeOutException, ExceptionBase)
};

class ArgumentException : public ExceptionBase
{
    DEF_OBJECT_TYPE(ArgumentException, ExceptionBase)
public:
    DEF_EXCEPTION_CTOR(ArgumentException, ExceptionBase)
};

class ArgumentNullException : public ArgumentException
{
    DEF_OBJECT_TYPE(ArgumentNullException, ArgumentException)
public:
    DEF_EXCEPTION_CTOR(ArgumentNullException, ArgumentException)
};

#endif