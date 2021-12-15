/*
* @Moudule     : Type
* @Date        : 2021/05/15
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef _CORELIB_TYPE_H
#define _CORELIB_TYPE_H


#define __CORELIB_DEF_BASETYPE_META(NAME, BASE) \
private: \
    using base = BASE; \
    using __corelib_curclass = NAME; \
    friend class Type; \
    friend class TypeTraits; \
    public: \
        inline virtual Type* GetType() const override { \
            return __meta_type(); \
        } \
    private: \
        static inline struct _TypeInit { \
            _TypeInit() { NAME::__meta_type(); } \
        } __corelib_type_init_;

//声明CoreLib元数据
#define CORELIB_DEF_TYPE(NAME, BASE) \
    static inline Type* __meta_type() \
    { \
        static int id = -1; \
        if (id == -1) \
        { \
            auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value(); \
            if (dynptr == nullptr) \
            { \
                dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get(); \
            } \
            id = Type::Register(dynptr, cltypeof<BASE>(), #NAME, typeid(NAME), sizeof(NAME)); \
        } \
        return Type::GetType(id); \
    } \
    __CORELIB_DEF_BASETYPE_META(NAME, BASE)

//声明CoreLib模板元数据
#define CORELIB_DEF_TEMPLATE_TYPE(NAME, BASE, ...) \
    static inline Type* __meta_type() \
    { \
        static int id = -1; \
        if (id == -1) \
        { \
            auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value(); \
            if (dynptr == nullptr) \
            { \
                dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get(); \
            } \
            id = Type::Register(dynptr, cltypeof<BASE>(), StringUtil::Concat(#NAME, "<", typeid(__VA_ARGS__).name(), ">"), typeid(NAME<__VA_ARGS__>), sizeof(NAME<__VA_ARGS__>)); \
        } \
        return Type::GetType(id); \
    } \
    __CORELIB_DEF_BASETYPE_META(NAME, BASE)

//反射工厂创建函数声明
#define CORELIB_DECL_DYNCINST() \
    static Object* DynCreateInstance(const ParameterPackage& params)

#include <vector>
#include <map>
#include <any>
#include <type_traits>
#include <memory>

#include "UString.h"
#include "Object.h"

namespace JxCoreLib
{
    struct ParameterPackage;

    class MemberInfo;
    class FieldInfo;
    class MethodInfo;
    class ReflectionBuilder;
    
    struct TypeBinding
    {
        enum Enum : int32_t
        {
            NonPublic = 1,
            Static = 1 << 1,
        };
    };

    //inline constexpr TypeBindingAttr operator|(TypeBindingAttr l, TypeBindingAttr r)
    //{
    //    return static_cast<TypeBindingAttr>(static_cast<int32_t>(l) | static_cast<int32_t>(r));
    //}
    //inline constexpr TypeBindingAttr operator&(TypeBindingAttr l, TypeBindingAttr r)
    //{
    //    return static_cast<TypeBindingAttr>(static_cast<int32_t>(l) & static_cast<int32_t>(r));
    //}


    class Type final : public Object
    {
    private:
        using c_inst_ptr_t = Object * (*)(const ParameterPackage&);
    private:
        int32_t id_;
        string name_;
        int32_t structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;
        const std::type_info& typeinfo_;

    private:
        Type(int32_t id,
            const string& name,
            Type* base,
            c_inst_ptr_t c_inst_ptr,
            const std::type_info& typeinfo,
            int32_t structure_size);

        Type(const Type& r) = delete;
        Type(Type&& r) = delete;
        static Type* __meta_type();
        static inline struct _TypeInit {
            _TypeInit() {
                Type::__meta_type();
            }
        } _type_init_;
    public:
        virtual Type* GetType() const;
    public:
        virtual int get_structure_size() const;
        const string& get_name() const;
        Type* get_base() const;
        const std::type_info& get_typeinfo() const;
        bool is_primitive_type() const;
    public:
        virtual string ToString() const override;
    public:
        bool IsInstanceOfType(Object* object);
        /* 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。*/
        bool IsSubclassOf(Type* type);
    public:
        [[nodiscard]] Object* CreateInstance();
        [[nodiscard]] Object* CreateInstance(const ParameterPackage& v);
        std::unique_ptr<Object> CreateInstanceUnique();
        std::unique_ptr<Object> CreateInstanceUnique(const ParameterPackage& v);
    public:
        static Type* GetType(const string& str);
        static Type* GetType(const char*& str);
        static Type* GetType(int32_t id);
        static std::vector<Type*> GetTypes();
    public:
        static int Register(
            c_inst_ptr_t dyncreate,
            Type* base,
            const string& name,
            const std::type_info& info,
            int32_t structure_size);

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
        friend class ReflectionBuilder;
    public:
        std::vector<MemberInfo*> get_memberinfos(TypeBinding::Enum attr);
        MemberInfo* get_memberinfo(const string& name);
        std::vector<FieldInfo*> get_fieldinfos(TypeBinding::Enum attr);
        FieldInfo* get_fieldinfo(const string& name);
        std::vector<MethodInfo*> get_methodinfos(TypeBinding::Enum attr);
        MethodInfo* get_methodinfo(const string& name);
    private:
        std::map<string, MemberInfo*> member_infos_;
    private:
        void _AddMemberInfo(MemberInfo* info);

    };

    template<typename T> struct fulldecay { using type = T; };
    template<typename T> struct fulldecay<const T> : fulldecay<T> { };
    template<typename T> struct fulldecay<T&> : fulldecay<T> { };
    template<typename T> struct fulldecay<T*> : fulldecay<T> { };
    template<typename T> struct fulldecay<volatile T> : fulldecay<T> { };
    template<typename T> struct fulldecay<T[]> : fulldecay<T> { };
    template<typename T, int I>  struct fulldecay<T[I]> : fulldecay<T> { };

    template<typename T>
    inline Type* cltypeof()
    {
        return Type::Typeof<T>();
    }

    template<typename T>
    struct is_corelib_type
    {
        static constexpr inline bool value = std::is_base_of<JxCoreLib::Object, T>::value;
    };

    inline bool istype(Object* obj, Type* type)
    {
        return type->IsInstanceOfType(obj);
    }

    template<typename T>
    inline void RegisterClass()
    {
        cltypeof<T>();
    }

    struct ParameterPackage
    {
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
    class TypeTraits final
    {
    public:
        template<typename T, typename = void>
        struct is_zeroparam_ctor : std::false_type {};

        template<typename T>
        struct is_zeroparam_ctor<T, std::void_t<decltype(T())>> : std::true_type {};

        template<typename T, bool HasZeroParamCtor = is_zeroparam_ctor<T>::value>
        struct get_zeroparam_object {};

        template<typename T>
        struct get_zeroparam_object<T, true>
        {
            using value_type = Object * (*)(const ParameterPackage&);
            static value_type get()
            {
                return [](const ParameterPackage&) -> Object* {
                    return new T;
                };
            }
        };
        template<typename T>
        struct get_zeroparam_object<T, false>
        {
            using value_type = Object * (*)(const ParameterPackage&);
            static value_type get()
            {
                return nullptr;
            }
        };

        template<typename T>
        struct has_dyninstc
        {
        private:
            template<typename U, Object* (*)(const ParameterPackage&) = &U::DynCreateInstance>
            static constexpr bool check(U*) { return true; }
            static constexpr bool check(...) { return false; }
        public:
            static constexpr bool value = check(static_cast<T*>(0));
        };

        template<typename T, bool Test = has_dyninstc<T>::value>
        struct get_dyninstpointer {};

        template<typename T>
        struct get_dyninstpointer<T, true>
        {
            using value_type = Object * (*)(const ParameterPackage&);
            //member method
            static value_type get_value()
            {
                return &T::DynCreateInstance;
            }
        };

        template<typename T>
        struct get_dyninstpointer<T, false>
        {
            using value_type = Object * (*)(const ParameterPackage&);
            //nullptr
            static value_type get_value()
            {
                return nullptr;
            }
        };


    };

    class StdAny;

    template<typename T, bool is_corelib = is_corelib_type<T>::value>
    struct get_cltype
    {};

    template<typename T>
    struct get_cltype<T, false>
    {
        using type = StdAny;
    };
    template<typename T>
    struct get_cltype<T*, false>
    {
        using type = get_cltype<T>::type*;
    };

    template<typename T>
    struct get_cltype<T, true>
    {
        using type = T;
    };
    template<typename T>
    struct get_cltype<T*, true>
    {
        using type = get_cltype<T>::type*;
    };


#define __CORELIB_DEF_BASE_TYPE(Class, DataType) \
    class Class : public Object \
    { \
        CORELIB_DEF_TYPE(JxCoreLib::Class, Object);\
        CORELIB_DECL_DYNCINST() { \
            if (params.Count() != 1 || !params.Check<DataType>()) \
            { \
                return nullptr; \
            } \
            return new Class{ params.Get<DataType>(0) }; \
        } \
    public: \
        using type = DataType; \
        DataType value; \
        Class(DataType value) : value(value) { } \
        operator DataType() { return value; } \
        DataType operator()() { \
            return value; \
        } \
        virtual string ToString() const override { return std::to_string(value); } \
    }; \
    static bool operator==(const Class& l, const DataType& r) { return l.value == r; } \
    static bool operator==(const DataType& l, const Class& r) { return l == r.value; } \
    static bool operator!=(const Class& l, const DataType& r) { return l.value != r; } \
    static bool operator!=(const DataType& l, const Class& r) { return l != r.value; } \
    template<> struct get_cltype<DataType> { using type = Class; }; \
    template<> inline Type* cltypeof<DataType>() { return cltypeof<Class>(); }

    __CORELIB_DEF_BASE_TYPE(CharType, char);
    __CORELIB_DEF_BASE_TYPE(Integer8, int8_t);
    __CORELIB_DEF_BASE_TYPE(UInteger8, uint8_t);
    __CORELIB_DEF_BASE_TYPE(Integer16, int16_t);
    __CORELIB_DEF_BASE_TYPE(UInteger16, uint16_t);
    __CORELIB_DEF_BASE_TYPE(Integer32, int32_t);
    __CORELIB_DEF_BASE_TYPE(UInteger32, uint32_t);
    __CORELIB_DEF_BASE_TYPE(Integer64, int64_t);
    __CORELIB_DEF_BASE_TYPE(UInteger64, uint64_t);
    __CORELIB_DEF_BASE_TYPE(Single32, float);
    __CORELIB_DEF_BASE_TYPE(Double64, double);
    __CORELIB_DEF_BASE_TYPE(Boolean, bool);

    class String : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::String, Object);
        CORELIB_DECL_DYNCINST()
        {
            if (params.Count() != 1 || !params.Check<const char*>())
            {
                return nullptr;
            }
            return new String{ params.Get<const char*>(0) };
        }
    private:
    public:
        using type = string;
        string value;
        String(const string& str) : value(str) {}
        String(const char* str) : value(str) {}
        operator string() { return value; }
        string operator()() { return value; }
        virtual string ToString() const override { return value; }
    };
    template<> struct get_cltype<string> { using type = String; };
    template<> inline Type* cltypeof<string>() { return cltypeof<String>(); }

    class StdAny : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::StdAny, Object);
        CORELIB_DECL_DYNCINST() {
            return nullptr;
        }
    public:
        std::any value;
        StdAny(std::any value) : value(value) { }
        operator std::any() { return this->value; }

    private:
        template<typename TValue>
        static bool _AnyCast(const std::any& any, TValue* t) { return false; }

        template<typename TValue, typename TCastable1, typename... TCastable>
        static bool _AnyCast(const std::any& any, TValue* t)
        {
            auto name = any.type().name();
            auto cname = typeid(TCastable1).name();
            if (any.type() == typeid(TCastable1))
            {
                *t = std::any_cast<TCastable1>(any);
                return true;
            }
            else
            {
                return _AnyCast<TValue, TCastable...>(any, t);
            }
            return false;
        }

    public:
        template<typename TValue, typename... TCastable>
        static bool AnyCast(const std::any& any, TValue* t)
        {
            return _AnyCast<TValue, TCastable...>(any, t);
        }
    };
    template<> struct get_cltype<std::any> { using type = StdAny; };
    template<> inline Type* cltypeof<std::any>() { return cltypeof<StdAny>(); }

    class StdMap : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::StdMap, Object);
    public:
        std::map<Object*, Object*> value;
        using type = std::map<Object*, Object*>;
        StdMap(const std::map<Object*, Object*>& map) : value(map) {}
        operator type() { return value; }
    };
    template<>
    struct get_cltype<std::map<Object*, Object*>> { using type = StdMap; };
    template<> inline Type* cltypeof<std::map<Object*, Object*>>() { return cltypeof<StdMap>(); }


    template<typename T, bool is_corelib = is_corelib_type<T>::value>
    struct get_object_pointer
    {
    };

    template<typename T>
    struct get_object_pointer<T, true>
    {
        static Object* get(const T* t)
        {
            return const_cast<T*>(t);
        }
        static Object* get(const T& t)
        {
            return &const_cast<T&>(t);
        }
    };

    template<typename T>
    struct get_object_pointer<T, false>
    {
        using _CTy = get_cltype<T>::type;
        static Object* get(const T* t)
        {
            return new _CTy{ *t };
        }
        static Object* get(const T& t)
        {
            return new _CTy{ t };
        }
    };

    template<typename T, bool is_add>
    struct find_pointer_if
    {
    };

    template<typename T>
    struct find_pointer_if<T, false>
    {
        using type = T*;
        static T* get(T* t) {
            return t;
        }
    };

    template<typename T>
    struct find_pointer_if<T, true>
    {
        using type = T;
        static T* get(T** t) {
            return *t;
        }
    };


    template <typename, template <typename...> class Op, typename... T>
    struct is_detected_impl : std::false_type {};
    template <template <typename...> class Op, typename... T>
    struct is_detected_impl<std::void_t<Op<T...>>, Op, T...> : std::true_type {};

    template <template <typename...> class Op, typename... T>
    using is_detected = is_detected_impl<void, Op, T...>;

}

#endif // !_CORELIB_TYPE_H