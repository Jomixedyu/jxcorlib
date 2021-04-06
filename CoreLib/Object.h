#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include "String.h"
#include <functional>
#define DEF_OBJECT_TYPE(name, base) \
private: \
    class _Type : public Type \
    { \
        virtual Object* CreateInstance(void* v) { \
            return new name(v); \
        } \
    }; \
private: \
    inline static Type* __meta_type() { \
        static int id = -1; \
        if (id == -1) { \
            Type* t = new _Type; \
            id = _Type_Register(t, Type::GetType(_T(#base)), NewString(_T(#name)) );\
        } \
        return Type::GetType(id); \
    } \
public: \
    inline virtual Type* get_type() const { \
        return __meta_type(); \
    } \
private: \
    friend Type* typeof<name>(); \

#define DEF_OBJECT_CINSTCTOR(name) name(void* v) {}

#define DECL_TOSTRING() virtual RefString to_string() const override
#define DECL_EQUALS() virtual bool Equals(Object* target) const override

class Type;
class Object;

int _Type_Register(Type* type, Type* base, RefString name);

class Type
{
protected:
    RefString name_;
    Type* base_;
protected:
    Type() : name_(nullptr), base_(nullptr) { }
public:
    inline static Type* __meta_type() {
        static RefString _name = NewString(_T("Type"));
        static int id = -1;
        if (id == -1) {
            Type* t = new Type;
            id = _Type_Register(t, nullptr, _name);
        }
        return Type::GetType(id);
    }
    inline virtual Type* get_type() const {
        return __meta_type();
    }
public:
    RefString get_name() const {
        return this->name_;
    }
    Type* get_base() const {
        return this->base_;
    }
public:
    bool IsInstanceOfType(Object* object);
    /*
    * 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。
    */
    bool IsSubclassOf(Type* type);
    virtual Object* CreateInstance() {
        return nullptr;
    }
public:
    static Type* GetType(const RefString& str);
    static Type* GetType(const String& str);
    static Type* GetType(int id);
public:
    friend int _Type_Register(Type* type, Type* base, RefString name);
};



class Object
{
private:
    class _Type : public Type
    {
        virtual Object* CreateInstance() {
            return new Object;
        }
    };
public:
    inline static Type* __meta_type() {
        static int id = -1;
        if (id == -1) {
            Type* t = new _Type;
            id = _Type_Register(t, nullptr, NewString(_T("Object")));
        }
        return Type::GetType(id);
    }
    inline virtual Type* get_type() const {

        return __meta_type();
    }
public:
    Object() {}
public:
    virtual RefString to_string() const {
        return this->get_type()->get_name();
    }
    static bool Equals(const Object* x, const Object* y) {
        return x == y;
    }
    virtual bool Equals(Object* target) const {
        return Equals(this, target);
    }
};



template<typename T>
inline Type* typeof() {
    return T::__meta_type();
}

#define TypeOf(class) class::__meta_type()

#define NameOf(class) _T(#class)


inline bool istype(Object* obj, Type* type)
{
    return type->IsInstanceOfType(obj);
}

template<typename T>
void RegisterClass() {
    typeof<T>();
}


#endif // !CORELIB_OBJECT_H
