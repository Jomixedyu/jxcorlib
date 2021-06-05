#include "../CoreLib/Property.h"
#include "../CoreLib/OOPCore.h"

using namespace JxCoreLib;

class PropertyClass
{
private:
    int i_;
public:
    Property<int> i{
        PROP_GET(int) {
            return this->i_;
        },
        PROP_SET(int) {
            this->i_ = value;
        }
    };
};

void TestProperty()
{
    PropertyClass c;
    
    c.i = 3;
    int num = c.i;

}