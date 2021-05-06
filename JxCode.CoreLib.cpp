#include <string>
#include <iostream>
#include "CoreLib/Object.h"
#include "CoreLib/Events.hpp"

#include <sstream>

using namespace std;

#define $ATTRIBUTE(src, type, attribute) 

enum class MemberType
{
    Class,
    Field,
    Method
};

template<typename T>
class Property
{
private:
    std::function<T&()> get;
    std::function<void(const T& value)> set;
public:
    Property(const std::function<T& ()>& get, const std::function<void(const T& value)>& set)
        : get(get), set(set)
    {
    }
    T& operator()() {
        return this->get();
    }
    void operator=(const T& v) {
        this->set(v);
    }
    operator T() {
        return this->get();
    }
};
#define PROP_GET(T) [this]()->T& 
#define PROP_SET(T) [this](const T& value) 

class TagAttribute {

};
class SerializableAttrubite {

};


$ATTRIBUTE(TestClass, MemberType::Class, TagAttribute("Test"));
class TestClass : public Object
{
    DEF_OBJECT_TYPE(TestClass, Object);

    $ATTRIBUTE(TestClass::id, MemberType::Field, SerializableAttrubite);
    private: int id;

    public: Property<int> ii {
        PROP_GET(int) {
            return this->id;
        },
        PROP_SET(int) {
            this->id = value;
        }
    };

    public: TestClass(int _id) : id(_id) {
    }
    
    public: virtual String ToString() const override {
        Type* type = this->get_type();
        return _T("Str: ") + type->get_name();
    }
    public: virtual bool Equals(Object* target) const override {
        if (target->get_type() != typeof<TestClass>()) {
            return false;
        }
        return this->id == (static_cast<TestClass*>(target))->id;
    }
    DECL_OBJECT_DYNCREATEINSTANCE() {
        return new TestClass(0);
    }
};

class MemoryBlock
{
    int chunk_size;
    void* head;
    MemoryBlock* next;
};
class MemoryPool
{
    //8 16 24 32 48 64 72 80 88 96 104 112 120 128
    MemoryBlock blocks[14];
    MemoryPool()
    {
        for (int i = 0; i < 14; i++)
        {
            blocks[i] = MemoryBlock();
        }
    }
};




int main()
{

    TestClass* tc0 = new TestClass(3);

    Type* type = tc0->get_type();
    cout << type->get_name() << endl;

    //类型生成后，可以使用GetType查找
    //如果没有生成（没调用过get_type），需要用typeof<TestClass>()或者RegisterClass<TestClass>()来前向注册。
    Object* tc = Type::GetType(_T("TestClass"))->CreateInstance();
    cout << tc->ToString() << endl;

    Action<> act;
    act.AddListener([]() { cout << "called" << endl; });
    act.Invoke();

    Action<int, bool> act2;
    act2.AddListener([](int x, bool y) {cout << x << y << endl; });
    act2.Invoke(999, false);
    return;
    
}

