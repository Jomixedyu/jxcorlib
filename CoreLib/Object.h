#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include "String.h"

#define DEF_OBJECT_TYPE(NAME, BASE) \
private: \
    class _Type : public Type { \
    public: \
        _Type(__c_inst_ptr_t __c_inst_ptr) : Type(__c_inst_ptr) \
        { \
        } \
        virtual int get_structure_size() const override { \
            return sizeof(NAME); \
        } \
    }; \
public: \
    inline static Type* __meta_type() { \
        static int id = -1; \
        if (id == -1) { \
            Type* t = new _Type(DynCreateInstance); \
            id = _Type_Register(t, Type::GetType(RefString(_T(#BASE))), _T(#NAME) ); \
        } \
        return Type::GetType(id); \
    } \
public: \
    inline virtual Type* get_type() const { \
        return __meta_type(); \
    } \
private: \
    using base = BASE; \

#define DEF_OBJECT_DYNCREATEINSTANCE() \
        static Object* DynCreateInstance(CreateInstParamData* params) \
        { throw std::exception("the creation method is not implemented"); }

#define DEF_OBJECT_META(name, base) \
        DEF_OBJECT_TYPE(name, base) \
        DEF_OBJECT_DYNCREATEINSTANCE()

#define DECL_TOSTRING()   virtual String ToString() const override
#define DECL_EQUALS()     virtual bool Equals(Object* target) const override
#define DECL_OBJECT_DYNCREATEINSTANCE() \
    static Object* DynCreateInstance(CreateInstParamData* params)

class Type;
class Object;

struct CreateInstParamData
{
    void** data;
    int len;
};

using __c_inst_ptr_t = Object * (*)(CreateInstParamData*);

int _Type_Register(Type* type, Type* base, const String& name);


class Object
{
private:

public:
    static Type* __meta_type();
    inline virtual Type* get_type() const {
        return __meta_type();
    }
public:
    Object() {}
public:
    virtual String ToString() const;
    static bool Equals(const Object* x, const Object* y) {
        return x == y;
    }
    virtual bool Equals(Object* target) const {
        return Object::Equals(this, target);
    }
};


class Type : public Object
{
protected:
    String name_;
    Type* base_;
    __c_inst_ptr_t __c_inst_ptr_;

protected:
    Type() : base_(nullptr), __c_inst_ptr_(nullptr) { }
    Type(__c_inst_ptr_t __c_inst_ptr) : base_(nullptr), __c_inst_ptr_(__c_inst_ptr) { }

public:
    inline static Type* __meta_type() {
        static String _name = _T("Type");
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
    virtual int get_structure_size() const {
        return sizeof(Type);
    }
    const String& get_name() const {
        return this->name_;
    }
    Type* get_base() const {
        return this->base_;
    }
public:
    virtual String ToString() const override {
        return this->name_;
    }
public:
    bool IsInstanceOfType(Object* object);
    /*
    * 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。
    */
    bool IsSubclassOf(Type* type);
    virtual Object* CreateInstance(CreateInstParamData* v = nullptr) {
        if (this->__c_inst_ptr_ == nullptr) {
            throw std::exception("the creation method is not implemented");
        }
        return this->__c_inst_ptr_(v);
    }
public:
    static Type* GetType(const RefString& str);
    static Type* GetType(const String& str);
    static Type* GetType(const StringPointer& str);
    static Type* GetType(int id);
public:
    friend int _Type_Register(Type* type, Type* base, const String& name);
};



template<typename X, typename Y>
struct is_same_type
{
    constexpr operator bool() {
        return false;
    }
};
template<typename X>
struct is_same_type<X, X>
{
    constexpr operator bool() {
        return true;
    }
};

template<typename T>
inline Type* typeof()
{
    return T::__meta_type();
}


inline bool istype(Object* obj, Type* type)
{
    return type->IsInstanceOfType(obj);
}

template<typename T>
void RegisterClass() {
    typeof<T>();
}

class _TestExample : public Object {
    DEF_OBJECT_META(_TestExample, Object);
public:

};

#endif // !CORELIB_OBJECT_H
