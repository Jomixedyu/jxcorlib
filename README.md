# JxCode.CoreLib
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
- [](#)


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
