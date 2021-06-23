#include "../CoreLib/CoreLib.h"

using namespace JxCoreLib;


class NonCreateFactory : public Object
{
    CORELIB_DEF_TYPE(NonCreateFactory, Object);
    //create by default constructor
};

class CreateFactory : public Object
{
    CORELIB_DEF_TYPE(CreateFactory, Object);
    
};

void TestDynCreateInst()
{

}