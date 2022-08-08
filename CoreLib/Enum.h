#pragma once
#include "Object.h"
#include "Type.h"
#include "BasicTypes.h"
#include "Assembly.h"

#define CORELIB_DEF_ENUM(ASSEMBLY, NAMESPACE, NAME, ...) \
enum class NAME : uint32_t \
{ \
 __VA_ARGS__ \
}; \
class Boxing##NAME final : public Enum \
{ \
    CORELIB_DEF_ENUMTYPE(ASSEMBLY, NAMESPACE##Boxing##NAME, ::JxCoreLib::Enum); \
    static inline DataMap* definitions = nullptr; \
    static inline const char* string_definitions = #__VA_ARGS__; \
    static void init_defs() { InitDefinitions(string_definitions, &definitions); } \
public: \
    using unboxing_type = NAME; \
    static const DataMap* StaticGetDefinitions() { \
        init_defs(); \
        return definitions; \
    } \
    static string StaticFindName(NAME value) { \
        init_defs(); \
        for (auto& [name, enum_value] : *definitions) { \
            if (enum_value == static_cast<uint32_t>(value)) return name; \
        } \
        return {}; \
    } \
    string GetName() const override { return StaticFindName(static_cast<NAME>(this->value_)); } \
    Boxing##NAME& operator=(NAME value) { this->value_ = static_cast<uint32_t>(value); } \
    Boxing##NAME(NAME value) : base(static_cast<uint32_t>(value)) { } \
    Boxing##NAME() : base() {  } \
    virtual string ToString() const override { return this->GetName(); } \
};

namespace JxCoreLib
{
    class Enum : public ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Enum, Object);
    public:
        using DataMap = std::map<string, uint32_t>;
    protected:
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