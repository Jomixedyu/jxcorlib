# JxCode.CoreLib

![](https://img.shields.io/github/license/JomiXedYu/JxCode.CoreLib?style=for-the-badge)
![](https://img.shields.io/github/v/release/JomiXedYu/JxCode.CoreLib?style=for-the-badge)
![](https://img.shields.io/github/release-date/JomiXedYu/JxCode.CoreLib?style=for-the-badge)
![](https://img.shields.io/badge/StdVer-C++20-blueviolet.svg?style=for-the-badge&logo=c%2B%2B)

C++轻量对象系统，托管指针，反射，拥有统一的类型基类、常用异常类，以及事件模板等实用工具。 

## Feature
- 拥有统一的基类型Object。
- 拥有可以在运行时判断继承关系以及元数据的Type。
- 对象指针托管
- concept模板约束
- 类型全退化等模板工具
- 反射获取字段与方法信息，修改字段内容与执行方法。
- UTF8字符串工具类，查找，替换，编码转换等实用功能
- 事件委托类，接受所有类型函数的监听器。
- 属性模板，Get与Set访问器
- 基础异常类
- 调试信息工具

## Contents
- [JxCode.CoreLib](#jxcodecorelib)
  - [Feature](#feature)
  - [Contents](#contents)
  - [使用本基本库与工具的需求](#使用本基本库与工具的需求)
  - [命名规范](#命名规范)
  - [String字符串](#string字符串)
    - [String与Char](#string与char)
    - [索引与访问](#索引与访问)
    - [编码转换](#编码转换)
    - [字符串工具类](#字符串工具类)
  - [Object类型](#object类型)
  - [声明类型](#声明类型)
    - [普通类型声明](#普通类型声明)
    - [模板类型声明](#模板类型声明)
  - [托管指针](#托管指针)
  - [Type类型](#type类型)
    - [基本成员](#基本成员)
    - [cltypeof<>()模板函数](#cltypeof模板函数)
    - [内建类型的Type](#内建类型的type)
    - [类型封装](#类型封装)
    - [样例](#样例)
  - [Concept概念](#concept概念)
  - [类型工具](#类型工具)
    - [类型全退化](#类型全退化)
    - [std::any类型转换工具](#stdany类型转换工具)
    - [判断是否为corelib类型](#判断是否为corelib类型)
    - [获取对象指针](#获取对象指针)
  - [反射系统](#反射系统)
    - [反射工厂动态创建对象](#反射工厂动态创建对象)
    - [参数包与变长验证模板函数](#参数包与变长验证模板函数)
    - [基元类型](#基元类型)
    - [字段反射](#字段反射)
    - [方法反射](#方法反射)
  - [反射扩展](#反射扩展)
    - [Json序列化](#json序列化)
  - [属性模板](#属性模板)
  - [事件发送器与委托](#事件发送器与委托)
    - [事件类](#事件类)
    - [添加与移除](#添加与移除)
    - [静态函数](#静态函数)
    - [Lambda](#lambda)
    - [成员函数](#成员函数)
    - [执行](#执行)
  - [异常类](#异常类)
  - [调试工具](#调试工具)
  - [待实现功能](#待实现功能)


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
使用`#include <CoreLib/UString.h>`引入  
```c++
string s("a word 一个字");
```
字符串使用了别名引用，它的原型为
```c++
using string = std::string;
```
由此可见string并不继承Object，这也是为了能和其他使用标准库的类库与工具可以同时使用。  
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
StringIndexMapping(const string& str, size_t block_size);
```
第一个参数是字符串引用，第二个参数是块的大小：  
- 块越大，映射数据少，空间开销小，索引速度慢。
- 块越小，映射数据多，空间开销大，索引速度快。


### 编码转换
因为项目规范使用Unicode字符集，并且以UTF8以基础字符串，所以编码转换仅提供UTF8与UTF16的互相转换。
```c++
static std::u16string Utf8ToUtf16(const string& str);
static String Utf16ToUtf8(const std::u16string& str);
```

### 字符串工具类
StringUtil类中有常用的`Replace`，`Concat`等函数，具体查看`String.h`中的`StingUtil`类

## Object类型
Object类型有两个虚函数：
```c++
virtual Type* GetType() const;
virtual String ToString() const;
```
其中get_type不应该被用户所重写，重写由提供的定义宏来重写。  
而ToString是常用的格式化方法，类型选择性重写。  
如果你想说：`嘿！不应该还有个Equals方法吗？`，那你可以直接选择对它的指针比较，或者解指针对它的值使用operator==进行比较。  

另外，增加了一个std::to_string的新重载，函数为
```c++
std::string std::to_string(JxCoreLib::Object* obj)
{
    return obj->ToString();
}
```
该函数可以用标准库的字符串格式化函数对所有继承于Object的类型使用。

## 声明类型
### 普通类型声明
首先需要引入头文件`CoreLib/CoreLib.h`，然后进行类型声明：
```c++
namespace space
{
    class ExampleClass : public Object
    {
        CORELIB_DEF_TYPE(space::ExampleClass, Object);
    public:

    };
}
```
或者
```c++
namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_TYPE(space::DynCreateClass, Object);
    public:

    };
}
```
`CORELIB_DEF_TYPE`可以为定义该类型的元数据，应该注意的是第一个类型参数应该为完整路径。  


声明类型需要遵循以下几点：
- 继承于Object需要显式继承，并且总是public继承
- 使用宏定义本类与基类，本类需要使用完全限定名，即从根空间开始带有命名空间的完整路径。

### 模板类型声明
除了普通的类型定义外，模板类型使用的定义宏与一些细节是不一样的。  
一个模板类的声明：
```c++
template<typename T>
class TemplateClass : public Object
{
    CORELIB_DEF_TEMPLATE_META(TemplateClass, Object, T);
    CORELIB_DECL_DYNCINST() {
        return new TemplateClass<T>;
    }
public:

};
```
在普通的类型中使用`CORELIB_DEF_TYPE`去定义元数据，而模板类则使用`CORELIB_DEF_TEMPLATE_META`来定义。  
类型定义的后面是一个变长列表，依次按照模板顺序添加。  

关于模板类型获取Type名字：
当获取模板类型`Type*`的`get_name()`时，这个名字并不会像普通类型固定，而是会到编译器的影响。  
如`TemplateClass<int>`类型，在msvc下，它的名字是`TemplateClass<int>`，而在gcc下则是`TemplateClass<i>`。  
模板类中的名字取决于类型的`std::type_info`中的`name()`。  
综上所述，因为编译器实现的不同，模板类的反射工厂无法通用。

## 托管指针
为了更方便的使用对象，采取指针托管的方式。  
通过调用`SetManagedParent`来设置托管的父对象，当父对象销毁时，该对象跟随销毁。
```c++
void TestManagedPtr()
{
    Base* b = new Base;

    Other* o = new Other;
    Other* o2 = new Other;

    o->SetManagedParent(b); //add managed
    o2->SetManagedParent(b); //add managed
    o2->SetManagedParent(nullptr); //remove managed

    delete b;
    delete o2;
}
```

## Type类型
### 基本成员
三个属性：运行时获取类型的大小，获取类名，获取类型的基类Type
```c++
virtual int get_structure_size() const;
const string& get_name() const;
Type* get_base() const;
```
三个方法：判断实例是否为该类型，指定Type是否为本类的子类，按字符串获取指定类的Type。
```c++
bool IsInstanceOfType(Object* object);
bool IsSubclassOf(Type* type);
static Type* GetType(const string& str);
```
全局函数istype：指定Object是否为指定Type的实例（包含派生关系）
```c++
inline bool istype(Object* obj, Type* type);
```
### cltypeof<>()模板函数
typeof是对Type::Typeof的调用，可以获取一个唯一的Type实例
```c++
template<typename T>
inline Type* cltypeof()
{
    return Type::Typeof<T>();
}
```
### 内建类型的Type
一些内建类型拥有一个对应继承于Object的类型，同时也对应拥有一个Type。

| 原类型   | 对应类型   |
| -------- | ---------- |
| int8_t   | Integer8   |
| uint8_t  | UInteger8  |
| int16_t  | Integer16  |
| uint16_t | UInteger16 |
| int32_t  | Integer32  |
| uint32_t | UInteger32 |
| int64_t  | Integer64  |
| uint64_t | UInteger64 |
| float    | Single32   |
| double   | Double64   |
| bool     | Boolean    |

除了这些基础类型之外，还有一些其他的类型与之对应。
| 原类型   | 对应类型 |
| -------- | -------- |
| string   | String   |
| std::any | StdAny      |

以上类型都会有一个typeof的偏特化版本。  

另外，类型拥有性质：  
`cltypeof<int32_t>() == cltypeof<Integer32>()`  
`typeid(int32_t) != typeid(Integer32)`

### 类型封装
内建类型和一些标准库中的类型会有对应的封装类型，如`string`的封装类型是`String`，
我们可以使用`get_cltype`轻易的获取corelib类型。

```c++
using StrType = get_cltype<string>::type;
```

则有：  
```c++
typeid(StrType) == typeid(String);
```
另外还可以获取被封装的类型。
```c++
typeid(String::type) == typeid(string);
```
StrType就是String类型

### 样例
样例：（类型声明在了[声明类型](#声明类型)中）
```c++
ExampleClass* exm = new ExampleClass;

cout << exm->GetType()->get_name() << endl;

Type* dyn_type = Type::GetType("space::DynCreateClass");
Object* dyn = dyn_type->CreateInstance();

cout << (dyn->GetType() == cltypeof<space::DynCreateClass>()) << endl;
```
## Concept概念  
在`Object`中提供了及个concept用于模板约束，分别是：  
- baseof_object_concept : 模板类型必须继承于Object
- newable_concept : 模板类型必须有零个参数的公共构造函数


## 类型工具
### 类型全退化
在类型系统中很多时候都需要用到原始类型，使用`JxCoreLib::fulldecay<>`来做全退化。  
如`fulldecay<const int* const>::type`，`type`的类型为`int`。
### std::any类型转换工具
在JxCoreLib中提供了一个StdAny类，它是std::any类的一个包装，其中提供一个静态模板函数：
```c++
template<typename TValue, typename... TCastable>
static bool AnyCast(const std::any& any, TValue* t)
```
传入any和赋值的指针，这个被赋值的指针类型需要可以接受TCastable类型的对象。
```c++
std::any a = 3;
int uuu;
bool success = StdAny::AnyCast<int, long, short, int>(a, &uuu);
```

### 判断是否为corelib类型
使用`get_object_pointer<T>::value`来判断，样例：
```c++
get_object_pointer<String>::value
```

### 获取对象指针
如果使用了内建类型，则会将内建类型转换为框架类型，如`int`将会转换为`Integer32`。  
如果是继承`Object`的类型，`Object`指针会返回自己，`Object`对象则会取地址返回。  
如果除以上的类型，则会使用`std::any`的封装类`StdAny`
样例：
```c++
Integer32* i = (Integer32*)get_object_pointer<int>(3);
```

## 反射系统
### 反射工厂动态创建对象
首先声明一个带构造函数的类型，并用`CORELIB_DEF_TYPE`和`CORELIB_DECL_DYNCINST`宏声明元数据和反射的工厂函数。
```c++
namespace space
{
    class DynCreateClass : public Object
    {
        CORELIB_DEF_TYPE(space::DynCreateClass, Object);
        CORELIB_DECL_DYNCINST() {
            return new DynCreateClass(0);
        }
    private:
        int id;
    public:
        DynCreateClass(int id) : id(id) {}
    };
}
```
`CORELIB_DEF_TYPE`会根据以下顺序进行函数：  
- 自动查找反射工厂的函数，如果有则绑定
- 自动查找是否有无参构造函数，如果有则绑定
- 绑定失败，如果创建则会抛出。  

可以使用`CORELIB_DECL_DYNCINST`来自定义实现实现反射工厂函数体。  
或者直接使用`CORELIB_DECL_DYNCINST`原型
```c++
static Object* DynCreateInstance(const ParameterPackage& params)
```

可以使用类名来获取Type对象，使用`CreateInstance`创建
```c++
Type* dyn_type = Type::GetType("space::DynCreateClass");
Object* dyn = dyn_type->CreateInstance();
```

### 参数包与变长验证模板函数
`ParameterPackage`是用一个any数组的封装类，公共的成员函数为：
```c++
template<typename T> void Add(const T& v)；
template<typename T> T Get(const int& index) const；
size_t Count() const；
bool IsEmpty() const;
template<typename... TArgs> bool Check() const;
```
可以从外部向ParameterPackage对象添加参数，在传入工厂函数内。
```c++
Type* dyn_type = Type::GetType("space::DynCreateClass");
Object* dyn = dyn_type->CreateInstance(ParameterPackage{ 20 });
```
之后`CreateInstance`将会调用对应类型的工厂函数。  
这里需要注意的是，即使外部并没有传入参数包，这里依然会得到一个空参数包的引用。  
在使用外部传入的参数包时，可以使用`IsEmpty()`或者`Count()`进行简单的验证，
也可以使用可变长参数模板来对参数类型进行验证：
```c++
if(!params.Check<int>()) {
    return /*...*/;
}
if(!params.Check<int, float>()) {
    return /*...*/;
}
if(!params.Check<int, float, String>()) {
    return /*...*/;
}
```
使用Get按索引获取指定类型的值：
```c++
int p1 = params.Get<int>(0);
```
如果索引值不在正确的范围内，std::vector将会抛出错误，所以总应该在函数最开始的地方对传入的数据进行验证。

### 基元类型
基元类型包含了[内建类型的Type](#内建类型的Type)中的基础类型和`string`类型，可以通过`Type`实例的`is_primitive_type()`函数获得。
### 字段反射
字段反射定义宏：实例字段和静态字段的两种声明。
```c++
#define CORELIB_REFL_DECL_FIELD(NAME)
#define COERLIB_REFL_DECL_FIELD_STATIC(NAME)
```

样例类：
```c++
class DataModel : public Object
{
    CORELIB_DEF_TYPE(DataModel, Object);
public:

    CORELIB_REFL_DECL_FIELD(id);
    const int id = 0;

    CORELIB_REFL_DECL_FIELD(is_human);
    bool is_human = true;

    COERLIB_REFL_DECL_FIELD_STATIC(name);
    static inline Object* name;
};
```


字段的反射信息存在类型`Type`中，使用`get_fieldinfo(sting&)`来获取一个`FieldInfo*`。  
```c++
    //field reflection
    DataModel* model = new DataModel;

    Type* model_type = cltypeof<DataModel>();

    //id : const int
    FieldInfo* id_field = model_type->get_fieldinfo("id");
    assert(id_field->is_public() == true);
    assert(id_field->is_static() == false);
    assert(id_field->is_const() == true);
    assert(id_field->is_pointer() == false);
    assert(id_field->is_reference() == false);
    assert(id_field->is_volatile() == false);
    assert(id_field->get_name() == "id");

    id_field->SetValue(model, 3);

    Object* id_value = id_field->GetValue(model);
    assert(id_value->GetType() == cltypeof<int>());
    assert(*(Integer32*)id_value == 3);

    //name : Object*
    FieldInfo* name_field = model_type->get_fieldinfo("name");

    auto obj = new Object();
    name_field->SetValue(nullptr, obj);

    auto value = name_field->GetValue(nullptr);
    assert(value == obj);
```
使用GetValue和SetValue获取和设置值。如果字段为静态，实例指针传入nullptr即可。

### 方法反射
TODO

## 反射扩展
### Json序列化
json库来自于`nlohmann`，序列化使用`CoreLib.Extension`中的`JsonSerializer`。  
首先引入头文件`CoreLib.Extension`，在`JsonSerializer`中主要有两个静态方法：
```c++
static string Serialize(Object* obj);
static Object* Deserialize(const string& jstr, Type* type);
```
另外Deserialize还有一个模板版本
```c++
template<typename T>
static T* Deserialize(const string& str);
```
先声明两个可反射的类型
```c++
class PersonInfo : public Object
{
    CORELIB_DEF_TYPE(PersonInfo, Object);
public:
    CORELIB_REFL_DECL_FIELD(name);
    string name;
    CORELIB_REFL_DECL_FIELD(age);
    int age;
    virtual string ToString() const override
    {
        return std::format("name: {}, age: {}", name, age);
    }
};

class StudentInfo : public Object
{
    CORELIB_DEF_TYPE(StudentInfo, Object);
public:

    CORELIB_REFL_DECL_FIELD(id);
    int id;
    CORELIB_REFL_DECL_FIELD(president);
    bool president;
    CORELIB_REFL_DECL_FIELD(person_info);
    PersonInfo* person_info;

    virtual string ToString() const override
    {
        return std::format("id: {}, president: {}, person_info: {{{}}}", id, president, person_info->ToString());
    }
};
```
定义`StudentInfo`对象并赋值：
```c++
StudentInfo* student = new StudentInfo;
student->id = 33;
student->president = true;

student->person_info = new PersonInfo;
student->person_info->name = "jx";
student->person_info->age = 12;
```
随后调用序列化
```c++
string json_str = JsonSerializer::Serialize(student)
```
或者反序列化
```c++
StudentInfo* newstudent = JsonSerializer::Deserialize<StudentInfo>(json_str);
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

## 异常类
类库内内置了以下基本异常类，位置在`CommonException.h`
- ExceptionBase
  - RangeOutException
  - ArgumentException
    - ArgumentNullException
  - NotImplementException
  - NullPointerException

其中作为类库中异常类的基类`ExceptionBase`是一个多继承的类
```c++
class ExceptionBase : public std::exception, public Object
```
这是为了保证可以使用统一的`std::exception`来进行捕获，还可以使用Object的特性。

## 调试工具
引入DebugTool.h即可使用 (c++20)
```c++
#define DEBUG_INFO(info) std::format("info: {}; line: {}, file: {};", info, __LINE__, __FILE__);
```

## 待实现功能
- 反射的数组类型
- 函数的反射
