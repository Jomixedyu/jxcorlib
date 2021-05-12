#include <string>
#include <iostream>

#define CORELIB_AUTOINIT
#include "CoreLib/OOPCore.h"

#include <sstream>


class ExampleClass : public Object
{
    DEF_OBJECT_TYPE(ExampleClass, Object);

};
/*
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
    std::function<T& ()> get;
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

public: Property<int> ii{
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

*/



template<typename T>
struct A {
    const char* str = T;
};

int main()
{
    using namespace std;

    //Type* type = Type::GetType(_T("ExampleClass"));
    //if (type != nullptr) {
    //    cout << "yes" << endl;
    //}
    auto types = Type::GetTypes();
    return 0;

}

