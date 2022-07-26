#pragma once

#include "Object.h"
#include "Type.h"

namespace JxCoreLib
{
    SPTR_DECL(Delegate);
    class Delegate : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::Delegate, Object);
        
    public:


    private:
        sObject target_;
        //std::function
    };
    
}