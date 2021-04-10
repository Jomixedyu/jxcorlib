# JxCode.CoreLib
C++对象框架与常用函数库，实现部分运行期反射功能，在运行时判断类与对象继承关系，动态字符串创建类对象，拥有统一的类型基类、事件模板等实用工具。

使用前需要引入Object.h头文件
```c++
#include "Object.h"
```

创建类型
```c++
namespace space
{
class TestClass : public Object
{
    DEF_OBJECT_META(space::TestClass, Object);
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
}

```
类的继承应该总是为public的，并且规范只能单类型继承，多接口实现。
在类型顶部使用```DEF_OBJECT_META(name, base)```宏可以加入类型系统，第一个参数应为完全限定名，第二个参数为基类类型。

重写的两个函数原型与快捷宏定义
```c++
#define DECL_TOSTRING()  virtual String to_string() const override
#define DECL_EQUALS()    virtual bool Equals(Object* target) const override
```

如果你想让该类型可以通过反射创建实例，则需要使用```DEF_OBJECT_TYPE(name, base)```而不是```DEF_OBJECT_META(name, base)```，并且还需要实现函数```static Object* DynCreateInstance(CreateInstParamData* params)```，可以使用宏```DECL_OBJECT_DYNCREATEINSTANCE()```来快速定义。
```c++
namespace space
{
class TestClass : public Object
{
    DEF_OBJECT_TYPE(space::TestClass, Object);
private:
    int id;
public:
    TestClass(int _id) : id(_id) { }
    DECL_OBJECT_DYNCREATEINSTANCE() {
        return new TestClass(*(((int*)params)[0]));
    }
};
}
```

使用
```c++
TestClass* tc0 = new TestClass(3);

Type* type = tc0->get_type();

if(type == typeof<TestClass>()) {
	cout << type->get_name() << endl;
    cout << type->get_base()->get_name() << endl;
}

//反射创建对象
CreateInstParamData data;
void* p[1];
int param1 = 1;
p[0] = &param1;
data.len = 1;

Object* obj = Type::GetType(_T("TestClass"))->CreateInstance(&data);
```