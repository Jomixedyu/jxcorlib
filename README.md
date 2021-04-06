# JxCode.CoreLib
C++对象框架与常用函数库，实现部分运行期反射功能，在运行时判断类与对象继承关系，动态字符串创建类对象，拥有统一的类型基类、事件模板等实用工具。

创建类型
```c++
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
```

使用
```c++
TestClass* tc0 = new TestClass(3);

Type* type = tc0->get_type();

if(type == Type::GetType(_T("TestClass"))) {
	wcout << *type->get_name() << endl;
}
```