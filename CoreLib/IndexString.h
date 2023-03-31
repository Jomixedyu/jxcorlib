#pragma once

#include "index_string.hpp"
#include "Object.h"
#include "Type.h"
#include "BasicTypes.h"

namespace jxcorlib
{

    class IndexString : public PrimitiveObject, public index_string
    {
        CORELIB_DEF_TYPE(AssemblyObject_jxcorlib, jxcorlib::IndexString, PrimitiveObject);
    public:
        using index_string::basic_index_string;
        IndexString(std::basic_string_view<char_t> view) : index_string(view) {}

        virtual string ToString() const override { return this->to_string(); }
        index_string get_unboxing_value() { return *this; }

    };
    CORELIB_DECL_SHORTSPTR(IndexString);
    template<> struct get_boxing_type<index_string> { using type = IndexString; };

}
