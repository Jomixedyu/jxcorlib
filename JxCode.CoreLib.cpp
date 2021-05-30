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

#include "CoreLib/Events.hpp"
class EventTest
{
public:
    int cb_index;

    Action<int> e;
    void lambda_inst(int c) {
        std::cout << "lambda_inst callback" << std::endl;
    }
    void bind_inst(int c) {
        std::cout << "bind_inst callback" << std::endl;
    }
    static void static_method(int c) {
        std::cout << "static_method callback" << std::endl;
    }

    EventTest()
    {
        //静态lambda两种方式
        e.AddListener([](int c) { std::cout << "static lambda1 callback" << std::endl; });
        e += [](int c) { std::cout << "static lambda2 callback" << std::endl; };

        //静态函数
        e += static_method; //或 e.AddListener(static_method);
        //e -= static_method; //或 e.RemoveListener(static_method);

        //添加与移除闭包lambda方法，可以把lambda托管给this，然后最后按实例移除
        this->cb_index = e.AddListener(this, [this](int c) { this->lambda_inst(c); });
        //e.RemoveListenerByIndex(this->cb_index);

        //添加与移除成员方法
        e.AddListener(this, &EventTest::bind_inst);
        //e.RemoveListener(this, &EventTest::bind_inst);

        //执行
        e.Invoke(3);

        //移除实例中的所有事件
        e.RemoveByInstance(this);

        //移除全部事件
        e.RemoveAllListener();
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
