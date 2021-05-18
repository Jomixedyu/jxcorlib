#include <string>
#include <iostream>
#include <sstream>
#include <functional>


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
        //lambda两种方式
        e.AddListener([](int c) { std::cout << "lambda callback" << std::endl; });

        auto de = [](int c) { std::cout << "lambda callback" << std::endl; };
        e += de; //或 e.AddListener(de);
        //e -= de; 或 e.RemoveListener(de);
        
        //添加与移除实例方法
        this->cb_index = e.AddListener(this, [this](int c) { this->lambda_inst(c); });
        //e.RemoveListenerByIndex(this->cb_index);
        e.AddListener(this, &EventTest::bind_inst);
        e.RemoveListener(this, &EventTest::bind_inst);

        e.Invoke(3);

        //移除实例中的所有事件
        e.RemoveByInstance(this);
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
    EventTest e;



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