#include "../CoreLib/CoreLib.h"
#include <iostream>

using namespace JxCoreLib;

class Base : public Object
{

};

class Other : public Object
{
public:
    ~Other()
    {
        std::cout << "Other destructor" << std::endl;
    }
};

void TestManagedPtr()
{
    Base* b = new Base;

    Other* o = new Other;
    Other* o2 = new Other;

    o->SetManagedParent(b);
    o2->SetManagedParent(b);
    o2->SetManagedParent(nullptr);

    delete b;
    delete o2;
}