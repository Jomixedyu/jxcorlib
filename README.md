# JxCode.CoreLib

![](https://img.shields.io/github/license/Jayshonyves/JxCode.CoreLib?style=for-the-badge)
![](https://img.shields.io/github/v/release/Jayshonyves/JxCode.CoreLib?style=for-the-badge)

C++对象框架与常用函数库，实现部分运行期反射功能，在运行时判断类与对象继承关系，动态字符串创建类对象，拥有统一的类型基类、事件模板等实用工具。

## Feature
- 拥有统一的基类型Object
- 拥有可以在运行时判断继承关系的Type
- 当类型实现了重写了指定的方法后，可以从运行时反射创建改类型的对象。
- UTF8字符串工具类，查找，替换，编码转换等实用功能
- 事件委托类，接受静态函数，静态lambda，成员函数，闭包lambda监听器。
- 属性模板，Get与Set访问器
- 基础异常类
- 调试信息工具

## Catalog
- [JxCode.CoreLib](#jxcodecorelib)
  - [Feature](#feature)
  - [Catalog](#catalog)
  - [使用本基本库与工具的需求](#使用本基本库与工具的需求)
  - [命名规范](#命名规范)
  - [String字符串](#string字符串)
    - [String与Char](#string与char)
    - [索引与访问](#索引与访问)
  - [Object类型](#object类型)
  - [声明类型](#声明类型)
  - [Type类型](#type类型)
  - [属性模板](#属性模板)
  - [事件发送器与委托](#事件发送器与委托)
    - [事件类](#事件类)
    - [添加与移除](#添加与移除)
    - [静态函数](#静态函数)
    - [Lambda](#lambda)
    - [成员函数](#成员函数)
    - [执行](#执行)
  - [调试工具](#调试工具)


## 使用本基本库与工具的需求
* 所有类型只能使用单继承，但是可以继承纯虚类（接口）
* 继承总应该是public继承
* 项目应采用Unicode字符集，使用UTF8编码来编译字符串

## 命名规范
类google命名规范：
* 成员方法使用下划线命名法，并以下划线结尾，如 list_
* 局部变量使用下划线命名法
* 属性方法使用get_field()与set_field()命名
* 类名与方法名使用Pascal规则

## String字符串
### String与Char
```c++
String s("a word 一个字");
```
字符串使用了别名引用，它的原型为
```c++
using String = std::string;
```
由此可见String并不继承Object，这也是为了能和其他使用标准库的类库与工具可以同时使用。  
项目应采用的所有字符串都应该是UTF8的，可以使用StringUtil来查询UTF8字符串长度，索引字符，编码转换。  
因为UTF8是不定长的字符编码，所以在处理字符时采用的Char是一个八字节大小的类型。  
```c++
struct Char
{
    char value[8]{ 0 };
    //...
};
```

### 索引与访问
正常使用工具类去索引一个UTF8字符
```c++
Char c = StringUtil::CharAt(s, 9);
```
但是当字符串特别大时，并对这个字符串的随机访问次数多时，直接使用这个方法会特别的慢，
为解决UTF8的索引和随机访问慢的问题，采用字符串分块位置映射的空间换时间方式来提升速度。  
```c++
StringIndexMapping mapping(s, 2); // use cache
Char c2 = StringUtil::CharAt(s, 9, mapping);
```
构造函数原型
```
StringIndexMapping(const String& str, size_t block_size);
```
第一个参数是字符串引用，第二个参数是块的大小：  
- 块越大，映射数据少，空间开销小，索引速度慢。
- 块越小，映射数据多，空间开销大，索引速度快。


## Object类型
Object类型有两个虚函数：
```c++
virtual Type* get_type() const;
virtual String ToString() const;
```
其中get_type不应该被用户所重写，重写由提供的定义宏来重写。  
而ToString是常用的格式化方法，类型选择性重写。  

## 声明类型
首先需要引入头文件`CoreLib/OOPCore.h`，然后进行类型声明：
```c++
class ExampleClass : public Object
{
    DEF_OBJECT_TYPE(ExampleClass, Object);
public:

};
```
或者
```c++
namespace space
{
    class DynCreateClass : public Object
    {
        DEF_OBJECT_META(space::DynCreateClass, Object);
        DECL_OBJECT_DYNCREATEINSTANCE() {
            return new DynCreateClass;
        }
    public:

    };
}
```
声明类型需要遵循以下几点：
- 继承于Object需要显式继承，并且总是public继承
- 使用宏声明本类与基类，本类需要使用完全限定名，即从根空间开始带有命名空间的完整路径。
- 使用DEF_OBJECT_TYPE进行声明时无法使用反射创建对象。
- 使用DEF_OBJECT_META进行声明时则需要额外使用DECL_OBJECT_DYNCREATEINSTANCE来声明反射用的工厂函数。


## Type类型
三个属性：运行时获取类型的大小，获取类名，获取类型的基类Type
```c++
virtual int get_structure_size() const;
const String& get_name() const;
Type* get_base() const;
```
三个方法：判断实例是否为该类型，指定Type是否为本类的子类，按字符串获取指定类的Type。
```c++
bool IsInstanceOfType(Object* object);
bool IsSubclassOf(Type* type);
static Type* GetType(const String& str);
```
全局函数：获取类型的Type，指定Object是否为指定Type的实例（包含派生关系）
```c++
template<typename T>
inline Type* typeof();
inline bool istype(Object* obj, Type* type);
```
样例：（类型声明在了[声明类型](#声明类型)中）
```c++
ExampleClass* exm = new ExampleClass;

cout << exm->get_type()->get_name() << endl;

Type* dyn_type = Type::GetType("space::DynCreateClass");
Object* dyn = dyn_type->CreateInstance();

cout << (dyn->get_type() == typeof<space::DynCreateClass>()) << endl;
```

## 属性模板
属性是一种以类访问字段的方式来执行方法，主要使用括号重载operator()和类型转换operator T来实现。  
类型声明：
```c++
#include "../CoreLib/Property.h"
class PropertyClass
{
private:
    int i_;
public:
    Property<int> i{
        PROP_GET(int) {
            return this->i_;
        },
        PROP_SET(int) {
            this->i_ = value;
        }
    };
};
```
直接使用
```c++
void TestProperty()
{
    PropertyClass c;
    
    c.i = 3;
    int num = c.i;
}
```

## 事件发送器与委托
### 事件类
- Events作为模板基类，提供回调函数的添加移除。  
  - Delegate是Events的派生类，提供更多的控制权，可以移除全部事件或者执行全部事件。  
- ActionEvents是Events的一个无返回值偏特化版本。  
  - Action是Delegate的一个无返回值的偏特化版本。  
- FunctionEvents是Events的一个别名。
  - Function是Delegate的派生类，除了Delegate的权限和执行能力之外还拥有返回所有回调执行返回的结果集的功能。 
  - Function<bool>是一个特化版本，还增加了返回结果集中是否存在false的功能，主要用于关闭询问等功能。
### 添加与移除
支持添加：
- 静态函数
- lambda静态函数
- 实例成员函数
- lambda捕获函数
拿Action来举例：
```c++
Action<> e;
```
### 静态函数
普通静态函数支持两种方法的添加：
```c++
e += static_func;
e.AddListener(static_func);
```
与之对应：
```c++
e -= static_func;
e.RemoveListener(static_func);
```
### Lambda
lambda也可以使用+=与AddListene进行添加，但由于lambda没有名字，没办法移除，所以需要使用返回的索引来进行移除。
```c++
int index = e += ([](){});
e.RemoveListenerByIndex(index);
```
另外，带捕获的lambda可以选择传入一个实例，这样就可以通过实例去移除。
```c++
c.AddListener(this, [this](){});
c.RemoveListenerByInstance(this);
```
### 成员函数
成员函数需要使用实例和成员函数地址。  
成员函数也可以使用按实例移除的方式来移除：
```c++
e.AddListener(this, &TestClass::MemFunc);
e.RemoveListener(this, &TestClass::MemFunc);
e.RemoveListenerByInstance(this);
```

### 执行
```c++
e.Invoke();
```

## 调试工具
引入DebugTool.h即可使用 (c++20)
```c++
#define DEBUG_INFO(info) std::format("info: {}; line: {}, file: {};", info, __LINE__, __FILE__);
```
