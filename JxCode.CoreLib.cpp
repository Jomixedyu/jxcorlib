#include <string>
#include <iostream>
#include <sstream>
#include <functional>

#define CORELIB_AUTOINIT
#include "CoreLib/OOPCore.h"
#include "CoreLib/Attribute.h"
#include "CoreLib/Property.h"


using namespace std;



class TestClass : public Object
{
    $ATTRIBUTE(TestClass, MemberType::Class, TagAttribute("Test"));
    DEF_OBJECT_TYPE(TestClass, Object);

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

