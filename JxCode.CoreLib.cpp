#include <string>
#include <iostream>
#include "CoreLib/Object.h"

using namespace std;

class TestClass : public Object
{
    DEF_OBJECT_TYPE(TestClass, Object)
public:
    virtual RefString to_string() const {
        return NewString( _T("Str: ") + *this->get_type()->get_name() );
    }
};

int main()
{
    TestClass* tc0 = new TestClass;

    Type* type = tc0->get_type();
    wcout << *type->get_name() << endl;

    //类型生成后，可以使用GetType查找
    //如果没有生成（没调用过get_type），需要用typeof<TestClass>()或者RegisterClass<TestClass>()来前向注册。
    Object* tc = Type::GetType(_T("TestClass"))->CreateInstance();
    wcout << *tc->to_string() << endl;

}

