#pragma once

#include "Jmath.h"
#include <CoreLib/Type.h>
#include <CoreLib/Reflection.h>
#include <format>
#include "Assembly.h"


namespace JxCoreLib::Math
{
    using namespace jmath;

    class BoxingVector2f : public JxCoreLib::ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxMath, JxCoreLib::Math::BoxingVector2f, JxCoreLib::ValueTypeObject);
    public:

        CORELIB_REFL_DECL_FIELD(x);
        float x;
        CORELIB_REFL_DECL_FIELD(y);
        float y;

        using unboxing_type = Vector2f;
        Vector2f get_unboxing_value() { return Vector2f(x, y); }

        BoxingVector2f() {}
        BoxingVector2f(Vector2f value) : x(value.x), y(value.y) {}

        virtual string ToString() const override { return to_string(unboxing_type(x, y)); }
    };
    template<> struct get_boxing_type<Vector2f> { using type = BoxingVector2f; };


    class BoxingVector3f : public JxCoreLib::ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxMath, JxCoreLib::Math::BoxingVector3f, JxCoreLib::ValueTypeObject);
    public:

        CORELIB_REFL_DECL_FIELD(x);
        float x;
        CORELIB_REFL_DECL_FIELD(y);
        float y;
        CORELIB_REFL_DECL_FIELD(z);
        float z;

        using unboxing_type = Vector3f;
        Vector3f get_unboxing_value() { return Vector3f(x, y, z); }

        BoxingVector3f() {}
        BoxingVector3f(Vector3f value) : x(value.x), y(value.y), z(value.z) {}

        virtual string ToString() const override { return to_string(unboxing_type(x, y, z)); }
    };
    template<> struct get_boxing_type<Vector3f> { using type = BoxingVector3f; };

    class BoxingVector4f : public JxCoreLib::ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxMath, JxCoreLib::Math::BoxingVector4f, JxCoreLib::ValueTypeObject);
    public:

        CORELIB_REFL_DECL_FIELD(x);
        float x;
        CORELIB_REFL_DECL_FIELD(y);
        float y;
        CORELIB_REFL_DECL_FIELD(z);
        float z;
        CORELIB_REFL_DECL_FIELD(w);
        float w;

        using unboxing_type = Vector4f;
        Vector4f get_unboxing_value() { return Vector4f(x, y, z, w); }

        BoxingVector4f() {}
        BoxingVector4f(Vector4f value) : x(value.x), y(value.y), z(value.z) {}

        virtual string ToString() const override { return to_string(unboxing_type(x, y, z, w)); }
    };
    template<> struct get_boxing_type<Vector4f> { using type = BoxingVector4f; };
}