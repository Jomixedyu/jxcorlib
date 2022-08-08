#pragma once
#include "Object.h"
#include "Type.h"
#include "Assembly.h"

namespace JxCoreLib
{
    class Enum : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Enum, Object);
    protected:
        using DataMap = std::map<string, uint32_t>;
        static void InitDefinitions(const char* datas, DataMap** defs);
    public:
        Enum() : value_(0) {}
        Enum(uint32_t value) : value_(value) {}

        uint32_t GetValue() { return this->value_; }
        void SetValue(uint32_t value) { this->value_ = value; }

        virtual string GetName() const = 0;

        static uint32_t StaticParse(Type* type, string_view name);

    protected:
        uint32_t value_;
    };
}