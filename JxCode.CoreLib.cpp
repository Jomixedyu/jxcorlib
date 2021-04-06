#include <string>
#include <iostream>
#include "CoreLib/Object.h"
#include <sstream>

using namespace std;

class TestClass : public Object
{
    DEF_OBJECT_TYPE(TestClass, Object);
    DEF_OBJECT_CINSTCTOR(TestClass);
private:
    int id;
public:
    TestClass(int _id) : id(_id) { }
    DECL_TOSTRING() {
        return _T("Str: ") + this->get_type()->get_name();
    }
    DECL_EQUALS() {
        return this->id == ((TestClass)target).id;
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
    wcout << *type->get_name() << endl;

    //类型生成后，可以使用GetType查找
    //如果没有生成（没调用过get_type），需要用typeof<TestClass>()或者RegisterClass<TestClass>()来前向注册。
    Object* tc = Type::GetType(_T("TestClass"))->CreateInstance();
    wcout << *tc->to_string() << endl;
    


}

