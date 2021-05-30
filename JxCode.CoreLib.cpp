#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#include <filesystem>
#include <time.h>
#include <bitset>

#include "CoreLib/OOPCore.h"
#include "CoreLib/Property.h"

using namespace std;
using namespace JxCoreLib;

class TestClass : public Object
{
    DEF_OBJECT_META(TestClass, Object);
private:
    int id;

public:
    Property<int> ii{
        PROP_GET(int) {
            return this->id;
        },
        PROP_SET(int) {
            this->id = value;
        }
    };

public:
    TestClass(int _id) : id(_id) {
    }

    virtual String ToString() const override {
        Type* type = this->get_type();
        return _T("Str: ") + type->get_name();
    }

    DECL_OBJECT_DYNCREATEINSTANCE() {
        return new TestClass(0);
    }
};

class EventTest1
{
public:
    void Get(int c)
    {
        std::cout << "EventTest1: get" << std::endl;
    }
};


class ExampleClass{};


int main()
{
    using namespace std;

    /*
    Type* type = Type::GetType(_T("ExampleClass"));
    Object* obj = type->CreateInstance();
    obj->get_type() == typeof<ExampleClass>();
    */
    
    int a = 1176256512;
    float* f = (float*)&a;
    cout << *f;

    //EventTest e;


    return 0;

}
