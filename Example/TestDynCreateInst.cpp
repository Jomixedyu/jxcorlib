
#include "Assembly.h"

#include <cassert>

using namespace JxCoreLib;


class CreateFactory : public Object
{
    CORELIB_CLASS(CreateFactory, Object);
    //create by factory
    CORELIB_DECL_DYNCINST() {
        auto p = new CreateFactory;
        p->i = 1;
        return p;
    }
public:
    int i;
};

class NonCreateFactory : public Object
{
    CORELIB_CLASS(NonCreateFactory, Object);
public:
    int i;
    //create by default constructor
    NonCreateFactory()
    {
        i = 2;
    }
};



class NotImplCreate : public Object
{
    CORELIB_CLASS(NotImplCreate, Object);
    //throw NotImplementException
    NotImplCreate(int)
    {
    }
};

void TestDynCreateInst()
{

    Assembly* assm = Assembly::StaticFindAssembly(CURRENT_ASSEMBLY);
    auto p1 = std::static_pointer_cast<CreateFactory>(assm->FindType("CreateFactory")->CreateSharedInstance({}));
    assert(p1->i == 1);

    int r = 0;

    try
    {
        assm->FindType("NotImplCreate")->CreateInstance({});
        
    }
    catch (const std::exception&)
    {
        r = 3;
    }

    assert(r == 3);

}