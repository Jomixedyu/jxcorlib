#include <CoreLib/ManagedVector.h>
#include <iostream>

using namespace JxCoreLib;
using namespace std;

class Child : public Object
{
    CORELIB_DEF_TYPE(Child, Object);
public:
    Child()
    {
        cout << "TestManagedVector: ctor" << endl;
    }
    virtual ~Child()
    {
        cout << "TestManagedVector: dtor" << endl;
    }
};

void TestManagedVector()
{
    {
        ManagedVector<Object*> v;
        v.push_back(new Child);
    }

}
