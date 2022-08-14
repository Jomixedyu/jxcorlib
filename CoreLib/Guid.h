#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include "BasicTypes.h"

namespace JxCoreLib
{
    struct guid_t
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;
        uint32_t w = 0;
    public:

    public:
        static guid_t create_new();
        static guid_t parse(std::string_view str);
    public:
        std::string to_string() const;
        bool is_empty() const;
        bool operator==(guid_t right) const;
        operator bool() const;
        static guid_t empty() { return guid_t(); }
    };

    class Guid : public ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Guid, ValueTypeObject);
    public:
        using unboxing_type = guid_t;
        guid_t get_unboxing_value() const { return this->value_; }

        Guid() {}
        Guid(guid_t guid) : value_(guid) {}
        virtual string ToString() const override { return this->value_.to_string(); }
        virtual void Parse(const string& value) override { this->value_ = guid_t::parse(value); };
    private:
        guid_t value_;
    };
    CORELIB_DECL_SHORTSPTR(Guid);

    template<> struct get_boxing_type<guid_t> { using type = Guid; };

}