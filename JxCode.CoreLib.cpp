#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#include <filesystem>

#include "CoreLib/OOPCore.h"
#include "CoreLib/Attribute.h"
#include "CoreLib/Property.h"

using namespace std;


class TestClass : public Object
{
    DEF_OBJECT_META(TestClass, Object);
private:
    $ATTRIBUTE(TestClass::id, MemberType::Field, SerializableAttrubite);
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

    virtual bool Equals(Object* target) const override {
        if (target->get_type() != typeof<TestClass>()) {
            return false;
        }
        return this->id == (static_cast<TestClass*>(target))->id;
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
        //e -= static_method; 或 e.RemoveListener(static_method);

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

int main()
{
    using namespace std;

    //Type* type = Type::GetType(_T("ExampleClass"));
    //if (type != nullptr) {
    //    cout << "yes" << endl;
    //}
    //auto types = Type::GetTypes();
    
    //EventTest e;

    String s(_T("我啊a,235  ASda是苏打水打算的请问人"));

    auto c = StringUtil::At(s, 20);
    auto st = Encoding::UTF8ToANSI(c.ToString());
    cout << StringUtil::Length(s);

    return 0;

}

/*
class A {
public: virtual const unsigned long  int& f() const {};
};
class B : public A
{
public: static inline constexpr const unsigned long long get() noexcept {};
public: virtual const unsigned long int& f() const throw() override {};
};
*/