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
            return StaticType(); \
        } \
    private: \
        static inline struct _TypeInit { \
            _TypeInit() { NAME::StaticType(); } \
        } __corelib_type_init_;

//声明CoreLib元数据
#define CORELIB_DEF_TYPE(NAME, BASE) \
public: static inline Type* StaticType() \
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
public: static inline Type* StaticType() \
{ \
    static int id = -1; \
    if (id == -1) \
    { \
        auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value(); \
        if (dynptr == nullptr) \
        { \
            dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get(); \
        } \
        id = Type::Register(dynptr, cltypeof<BASE>(), StringUtil::Concat( \
            #NAME, "<", typeid(JxCoreLib::TemplateTypePair<__VA_ARGS__>).name(), ">"), \
            typeid(NAME<__VA_ARGS__>), sizeof(NAME<__VA_ARGS__>), \
            JxCoreLib::TemplateTypePair<__VA_ARGS__>::GetTemplateTypes()); \
    } \
    return Type::GetType(id); \
} \
private: \
    using base = BASE; \
    using __corelib_curclass = NAME<__VA_ARGS__>; \
    friend class Type; \
    friend class TypeTraits; \
    public: \
        inline virtual Type* GetType() const override { \
            return StaticType(); \
        } \
    private: \
        static inline struct _TypeInit { \
            _TypeInit() { NAME<__VA_ARGS__>::StaticType(); } \
        } __corelib_type_init_;

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
#include "EnumUtil.h"

namespace JxCoreLib
{
    struct ParameterPackage;

    class MemberInfo;
    class FieldInfo;
    class MethodInfo;
    class ReflectionBuilder;


    enum class TypeBinding : int32_t
    {
        None = 0,
        NonPublic = 1,
        Static = 1 << 1,
    };
    ENUM_CLASS_FLAGS(TypeBinding)

        template<typename... T>
    struct TemplateTypePair
    {
        static std::vector<Type*>* GetTemplateTypes()
        {
            std::vector<Type*>* vec = new std::vector<Type*>;
            (vec->push_back(cltypeof<T>()), ...);
            return vec;
        }
    };

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
        std::vector<Type*>* template_types_;

    private:
        Type(int32_t id,
            const string& name,
            Type* base,
            c_inst_ptr_t c_inst_ptr,
            const std::type_info& typeinfo,
            int32_t structure_size,
            std::vector<Type*>* template_types = nullptr);

        Type(const Type& r) = delete;
        Type(Type&& r) = delete;
        
        static inline struct _TypeInit {
            _TypeInit() {
                Type::StaticType();
            }
        } _type_init_;
    public:
        static Type* StaticType();
        virtual Type* GetType() const;
    public:
        virtual int get_structure_size() const;
        const string& get_name() const;
        Type* get_base() const;
        const std::type_info& get_typeinfo() const;
        bool is_primitive_type() const;
        std::vector<Type*>* const get_template_types() const;
    public:
        virtual string ToString() const override;
    public:
        bool IsInstanceOfType(Object* object);
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
            int32_t structure_size,
            std::vector<Type*>* template_types = nullptr);

        template<cltype_concept T>
        static inline Type* Typeof()
        {
            return T::StaticType();
        }
        /* Reflection */
    public:
        friend class MemberInfo;
        friend class FieldInfo;
        friend class MethodInfo;
        friend class ReflectionBuilder;
    public:
        std::vector<MemberInfo*> get_memberinfos(TypeBinding attr = TypeBinding::None);
        MemberInfo* get_memberinfo(const string& name);
        std::vector<FieldInfo*> get_fieldinfos(TypeBinding attr = TypeBinding::None);
        FieldInfo* get_fieldinfo(const string& name);
        std::vector<MethodInfo*> get_methodinfos(TypeBinding attr = TypeBinding::None);
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

    template<cltype_concept T>
    inline Type* cltypeof()
    {
        return Type::Typeof<T>();
    }

    //[[ deprecated ("cltype_concept") ]]
    //template<typename T>
    //struct is_corelib_type
    //{
    //    static constexpr inline bool value = std::is_base_of<JxCoreLib::Object, T>::value;
    //};

    inline bool istype(Object* obj, Type* type)
    {
        return type->IsInstanceOfType(obj);
    }
    template<cltype_concept T>
    inline bool isinstof(Object* obj)
    {
        return cltypeof<T>()->IsInstanceOfType(obj);
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

    template<typename T, bool b = cltype_concept<T>>
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


    //class StdMap : public Object
    //{
    //    CORELIB_DEF_TYPE(JxCoreLib::StdMap, Object);
    //public:
    //    std::map<Object*, Object*> value;
    //    using type = std::map<Object*, Object*>;
    //    StdMap(const std::map<Object*, Object*>& map) : value(map) {}
    //    operator type() { return value; }
    //};

    //template<cltype_ptr_concept K, cltype_ptr_concept V>
    //struct get_cltype<std::map<K, V>, false> { using type = StdMap; };

    //template<cltype_ptr_concept K, cltype_ptr_concept V>
    //inline Type* cltypeof<std::map<K, V>>() { return cltypeof<StdMap>(); }

    /*
    class ManagedMapTemplateBase : public Object
    {
        CORELIB_DEF_TYPE(JxCoreLib::ManagedMapTemplateBase, Object);
    public:
        std::map<Object*, Object*> value;
    };

    template<typename K, typename V>
    class ManagedMap : public ManagedMapTemplateBase
    {
    public:
        //CORELIB_DEF_TEMPLATE_TYPE(JxCoreLib::ManagedMap, ManagedMapTemplateBase, K, V);
        static inline Type* StaticType()
        {
            static int id = -1;
            if (id == -1)
            {
                auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value();
                if (dynptr == nullptr)
                {
                    dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get();
                }
                id = Type::Register(dynptr, cltypeof<ManagedMapTemplateBase>(), StringUtil::Concat(
                    "JxCoreLib::ManagedMap", "<", typeid(JxCoreLib::TemplateTypePair<K, V>).name(), ">"),
                    typeid(JxCoreLib::ManagedMap<K, V>), sizeof(JxCoreLib::ManagedMap<K, V>),
                    JxCoreLib::TemplateTypePair<K, V>::GetTemplateTypes());
            }
            return Type::GetType(id);
        }
    private:
        using base = ManagedMapTemplateBase;
        using __corelib_curclass = JxCoreLib::ManagedMap<K, V>;
        friend class Type;
        friend class TypeTraits;
    public:
        inline virtual Type* GetType() const override {

            return StaticType();
        }
    private:
        static inline struct _TypeInit {

            _TypeInit() { JxCoreLib::ManagedMap<K, V>::StaticType(); }
        } __corelib_type_init_;
    public:
        V& operator[](K k)
        {
            return value[k];
        }
        void insert(K k, V v)
        {
            value[k] = v;
        }
        void at(K k) { return value[k]; }
    };

    */
    template<typename T, bool iscl = cltype_concept<T>>
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