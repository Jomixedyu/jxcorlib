#ifndef CORELIB_TYPE_H
#define CORELIB_TYPE_H

#include <vector>
#include <map>
#include <any>

#include "String.h"
#include "Object.h"

#define CORELIB_DEF_BASETYPE_META(NAME, BASE) \
public: \
    friend class Type; \
    inline virtual Type* get_type() const override { \
        return __meta_type(); \
    } \
private: \
    using base = BASE; \
    using __corelib_curclass = NAME; \
    static inline struct _TypeInit{ \
        _TypeInit() \
        { \
            NAME::__meta_type(); \
        } \
    } __type_init_; \

//声明CoreLib元数据
#define CORELIB_DEF_META(NAME, BASE) \
    private: \
        static inline Type* __meta_type() { \
            static int id = -1; \
            if (id == -1) { \
                id = Type::Register(DynCreateInstance, typeof<BASE>(), NAMEOF(NAME), typeid(NAME), sizeof(NAME)); \
            } \
            return Type::GetType(id); \
        } \
    CORELIB_DEF_BASETYPE_META(NAME, BASE)
//声明CoreLib模板元数据
#define CORELIB_DEF_TEMPLATE_META(NAME, BASE, ...) \
    private: \
        static inline Type* __meta_type() { \
            static int id = -1; \
            if (id == -1) { \
                    id = Type::Register(DynCreateInstance, typeof<BASE>(), StringUtil::Concat(#NAME, "<", typeid(__VA_ARGS__).name(), ">"), typeid(NAME<__VA_ARGS__>), sizeof(NAME<__VA_ARGS__>)); \
            } \
            return Type::GetType(id); \
        } \
    CORELIB_DEF_BASETYPE_META(NAME, BASE)

//反射工厂创建函数声明
#define CORELIB_DECL_DYNCINST() \
    static Object* DynCreateInstance(const ParameterPackage& params)

//反射工厂没实现的函数体
#define CORELIB_IMPL_DYNCINST_NOTIMPL_FUNCBODY() \
        throw JxCoreLib::NotImplementException(__meta_type()->get_name() + ", the creation method is not implemented")

//反射工厂使用无参构造函数的默认函数体
#define CORELIB_IMPL_DYNCINST_DEFAULTIMPL_FUNBODY() \
        return new __corelib_curclass;

//反射工厂没实现定义
#define CORELIB_DEF_DYNCINST_NOTIMPL() \
        CORELIB_DECL_DYNCINST() \
        { CORELIB_IMPL_DYNCINST_NOTIMPL_FUNCBODY(); }

//反射工厂默认定义
#define CORELIB_DEF_DYNCINST() \
        CORELIB_DECL_DYNCINST() \
        { CORELIB_IMPL_DYNCINST_DEFAULTIMPL_FUNBODY(); }

//带有一个未实现（会抛出NotImplmentException异常）的反射工厂函数 的CoreLib类型
#define CORELIB_DEF_TYPE_NOTIMPL_DYNCINST(name, base) \
        CORELIB_DEF_META(name, base) \
        CORELIB_DEF_DYNCINST_NOTIMPL()

//定义使用默认无参构造函数的CoreLib类型
#define CORELIB_DEF_TYPE(name, base) \
        CORELIB_DEF_META(name, base) \
        CORELIB_DEF_DYNCINST()

//带有一个未实现（会抛出NotImplmentException异常）的反射工厂函数 的CoreLib模板类型
#define CORELIB_DEF_TEMPLATE_TYPE_NOTIMPL_DYNCINST(name, base, ...) \
        CORELIB_DEF_TEMPLATE_META(name, base, __VA_ARGS__) \
        CORELIB_DEF_DYNCINST_NOTIMPL()

//定义使用默认无参构造函数的CoreLib模板类型
#define CORELIB_DEF_TEMPLATE_TYPE(name, base, ...) \
        CORELIB_DEF_TEMPLATE_META(name, base, __VA_ARGS__) \
        CORELIB_DEF_DYNCINST()

namespace JxCoreLib
{
    struct ParameterPackage;

    class MemberInfo;
    class FieldInfo;
    class MethodInfo;
    class ReflectionFieldBuilder;

    class Type final : public Object
    {
    private:
        using c_inst_ptr_t = Object * (*)(const ParameterPackage&);
    private:
        int id_;
        string name_;
        int structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;
        const std::type_info& typeinfo_;

    private:
        Type(int id,
            const string& name,
            Type* base,
            c_inst_ptr_t c_inst_ptr,
            const std::type_info& typeinfo,
            int structure_size);

        Type(const Type& r) = delete;
        Type(Type&& r) = delete;
        static Type* __meta_type();
        static inline struct _TypeInit {
            _TypeInit() {
                Type::__meta_type();
            }
        } _type_init_;
    public:
        virtual Type* get_type() const;
    public:
        virtual int get_structure_size() const;
        const string& get_name() const;
        Type* get_base() const;
        const std::type_info& get_typeinfo() const;
    public:
        virtual string ToString() const override;
    public:
        bool IsInstanceOfType(Object* object);
        /*
        * 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。
        */
        bool IsSubclassOf(Type* type);
        [[nodiscard]] Object* CrearteInstance();
        [[nodiscard]] Object* CreateInstance(const ParameterPackage& v);
    public:
        static Type* GetType(const string& str);
        static Type* GetType(const char*& str);
        static Type* GetType(int id);
        static std::vector<Type*> GetTypes();
    public:
        static int Register(
            c_inst_ptr_t dyncreate,
            Type* base,
            const string& name,
            const std::type_info& info,
            int structure_size);

        template<typename T>
        static inline Type* Typeof()
        {
            return T::__meta_type();
        }
        /* Reflection */
    public:
        friend class MemberInfo;
        friend class FieldInfo;
        friend class MethodInfo;
        friend class ReflectionFieldBuilder;
    public:
        std::vector<MemberInfo*> get_memberinfos(bool is_public = true, bool is_static = false);
        MemberInfo* get_memberinfo(const string& name);
        std::vector<FieldInfo*> get_fieldinfos(bool is_public = true, bool is_static = false);
        FieldInfo* get_fieldinfo(const string& name);
        std::vector<MethodInfo*> get_methodinfos(bool is_public = true, bool is_static = false);
        MethodInfo* get_methodinfo(const string& name);
    private:
        std::map<string, MemberInfo*> member_infos_;
    private:
        void _AddMemberInfo(MemberInfo* info);
    };



    template<typename T>
    inline Type* typeof()
    {
        return Type::Typeof<T>();
    }

    inline bool istype(Object* obj, Type* type)
    {
        return type->IsInstanceOfType(obj);
    }

    template<typename T>
    inline void RegisterClass()
    {
        typeof<T>();
    }

    struct ParameterPackage : public Object
    {
        CORELIB_DEF_META(JxCoreLib::ParameterPackage, Object);
        CORELIB_DECL_DYNCINST();
    private:
        std::vector<std::any> data;
    public:
        ParameterPackage() {}
        ParameterPackage(std::initializer_list<std::any> lst) : data(lst.begin(), lst.end()) { }
        template<typename T>
        inline void Add(const T& v) {
            data.push_back(std::any(v));
        }
        template<typename T>
        inline T Get(const int& index) const {
            return std::any_cast<T>(data[index]);
        }
        inline size_t Count() const { return data.size(); }
        inline bool IsEmpty() const { return this->data.empty(); }
    private:
        template<int I>  bool _Check() const { return true; }

        template<int I, typename T, typename... TArgs>
        bool _Check() const {
            return (this->data[I].type() == typeid(T)) & _Check<I + 1, TArgs...>();
        }
    public:
        template<typename... TArgs>
        inline bool Check() const {
            if (this->data.size() != sizeof...(TArgs)) {
                return false;
            }
            return _Check<0, TArgs...>();
        }
    };

    class Integer32 : public Object
    {
        CORELIB_DEF_META(JxCoreLib::Integer32, Object);
        CORELIB_DECL_DYNCINST();
    public:
        int32_t value;
        Integer32(int32_t value) : value(value) { }
        operator int32_t() { return value; }
        virtual string ToString() const override { return std::to_string(value); }
    };
    class Single32 : public Object
    {
        CORELIB_DEF_META(JxCoreLib::Single32, Object);
        CORELIB_DECL_DYNCINST();
    public:
        float value;
        Single32(float value) : value(value) { }
        operator float() { return value; }
        virtual string ToString() const override { return std::to_string(value); }
    };
    class Double64 : public Object
    {
        CORELIB_DEF_META(JxCoreLib::Double64, Object);
        CORELIB_DECL_DYNCINST();
    public:
        double value;
        Double64(double value) : value(value) { }
        operator double() { return value; }
        virtual string ToString() const override { return std::to_string(value); }
    };
    class Boolean : public Object
    {
        CORELIB_DEF_META(JxCoreLib::Boolean, Object);
        CORELIB_DECL_DYNCINST();
    public:
        bool value;
        Boolean(bool value) : value(value) { }
        operator bool() { return value; }
        virtual string ToString() const override { return std::to_string(value); }
    };



    template<>
    inline Type* typeof<string>()
    {
        static int string_id = Type::Register(
            [](const ParameterPackage& str)->Object* {
                return nullptr;
            }, typeof<Object>(), "string", typeid(string), sizeof(string));

        return Type::GetType(string_id);
    }

    template<> inline Type* typeof<int32_t>() { return typeof<Integer32>(); }
    template<> inline Type* typeof<float>() { return typeof<Single32>(); }
    template<> inline Type* typeof<double>() { return typeof<Double64>(); }
    template<> inline Type* typeof<bool>() { return typeof<Boolean>(); }
}

#endif