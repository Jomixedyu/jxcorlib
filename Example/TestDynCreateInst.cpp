#include <CoreLib/CoreLib.h>
#include <cassert>

using namespace JxCoreLib;


class CreateFactory : public Object
{
    CORELIB_DEF_TYPE(CreateFactory, Object);
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
    CORELIB_DEF_TYPE(NonCreateFactory, Object);
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
    CORELIB_DEF_TYPE(NotImplCreate, Object);
    //throw NotImplementException
    NotImplCreate(int)
    {
    }
};

void TestDynCreateInst()
{

    auto p1 = (CreateFactory*)Type::GetType("CreateFactory")->CreateInstance();

    assert(p1->i == 1);

    auto p2 = (NonCreateFactory*)Type::GetType("NonCreateFactory")->CreateInstance();
    
    assert(p2->i == 2);

    int r = 0;

    try
    {
        Type::GetType("NotImplCreate")->CreateInstance();
    }
    catch (const std::exception&)
    {
        r = 3;
    }

    assert(r == 3);

}