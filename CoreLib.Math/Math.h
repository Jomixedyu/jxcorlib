#pragma once

#include "Jmath.h"
#include "Assembly.h"


namespace JxCoreLib::Math
{
    using namespace jmath;

    class BoxingVector3f : public JxCoreLib::Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxMath, JxCoreLib::Math::BoxingVector3f, JxCoreLib::Object);

    private:
        Vector3f value_;
    };
}