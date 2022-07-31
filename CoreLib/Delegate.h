#pragma once

#include "Object.h"
#include "Type.h"

namespace JxCoreLib
{

    class Delegate : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::Delegate, Object);
        
    public:


    private:
        Object_sp target_;
        //std::function
    };
    
}