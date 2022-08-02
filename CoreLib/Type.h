/*
* @Moudule     : Type
* @Date        : 2021/05/15
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#pragma once
#include <vector>
#include <map>
#include <any>
#include <type_traits>
#include <memory>
#include <functional>

#include "UString.h"
#include "Object.h"
#include "EnumUtil.h"
#include "IInterface.h"


//声明CoreLib元数据
#define CORELIB_DEF_TYPE(ASSEMBLY, NAME, BASE) \
public: static inline Type* StaticType() \
    { \
        static_assert(std::is_base_of<BASE, NAME>::value, "The base class does not match"); \
        static Type* type = nullptr; \
        if (type == nullptr) \
        { \
            auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value(); \
            if (dynptr == nullptr) \
            { \
                dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get(); \
            } \
            Assembly* assm = ::JxCoreLib::Assembly::StaticBuildAssembly(ASSEMBLY); \
            type = new Type(dynptr, assm, cltypeof<BASE>(), #NAME, typeid(NAME), sizeof(NAME)); \
            assm->RegisterType(type); \
        } \
        return type; \
    } \
private: \
    using base = BASE; \
    using __corelib_curclass = NAME; \
    friend class Type; \
    friend class TypeTraits; \
public: \
    inline virtual Type* GetType() const { \
        return StaticType(); \
    } \
private: \
    static inline struct __corelib_type { \
        __corelib_type() { NAME::StaticType(); } \
    } __corelib_type_init_;



#define CORELIB_DEF_INTERFACE(ASSEMBLY, NAME, BASE) \
public: static inline Type* StaticType() \
    { \
        static_assert(std::is_base_of<BASE, NAME>::value, "The base class does not match"); \
        static Type* type = nullptr; \
        if (type == nullptr) \
        { \
            Assembly* assm = ::JxCoreLib::Assembly::StaticBuildAssembly(ASSEMBLY); \
            type = new Type(nullptr, assm, cltypeof<BASE>(), #NAME, typeid(NAME), sizeof(NAME), true); \
            assm->RegisterType(type); \
        } \
        return type; \
    } \
private: \
    using __corelib_curclass = NAME; \
    friend class Type; \
    friend class TypeTraits; \
private: \
    static inline struct __corelib_type { \
        __corelib_type() { NAME::StaticType(); } \
    } __corelib_type_init_; \
public:


#define CORELIB_DEF_TINTERFACE(ASSEMBLY, NAME, BASE, ...) \
public: static inline Type* StaticType() \
    { \
        static_assert(std::is_base_of<BASE, NAME<__VA_ARGS__>>::value, "The base class does not match"); \
        static Type* type = nullptr; \
        if (type == nullptr) \
        { \
            Assembly* assm = ::JxCoreLib::Assembly::StaticBuildAssembly(ASSEMBLY); \
            type = new Type(nullptr, assm, cltypeof<BASE>(), #NAME, typeid(NAME<__VA_ARGS__>), sizeof(NAME<__VA_ARGS__>), true); \
            assm->RegisterType(type); \
        } \
        return type; \
    } \
private: \
    using base = BASE; \
    using __corelib_curclass = NAME__VA_ARGS__<>; \
    friend class Type; \
    friend class TypeTraits; \
private: \
    static inline struct __corelib_type { \
        __corelib_type() { NAME<__VA_ARGS__>::StaticType(); } \
    } __corelib_type_init_; \
public:

//声明CoreLib模板元数据
#define CORELIB_DEF_TEMPLATE_TYPE(ASSEMBLY, NAME, BASE, ...) \
public: static inline Type* StaticType() \
{ \
    static Type* type = nullptr; \
    if (type == nullptr) \
    { \
        auto dynptr = TypeTraits::get_dyninstpointer<__corelib_curclass>::get_value(); \
        if (dynptr == nullptr) \
        { \
            dynptr = TypeTraits::get_zeroparam_object<__corelib_curclass>::get(); \
        } \
        using TemplateType = JxCoreLib::TemplateTypePair<__VA_ARGS__>; \
        Assembly* assm = ::JxCoreLib::Assembly::StaticBuildAssembly(ASSEMBLY); \
        type = new Type(dynptr, assm, cltypeof<BASE>(), StringUtil::Concat(#NAME, "<", typeid(TemplateType).name(), ">"), typeid(NAME<__VA_ARGS__>), sizeof(NAME<__VA_ARGS__>)); \
        assm->RegisterType(type); \
    } \
    return type; \
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
        static inline struct __corelib_type { \
            __corelib_type() { NAME<__VA_ARGS__>::StaticType(); } \
        } __corelib_type_init_;

#define CORELIB_IMPL_INTERFACES(...) \
    static inline struct __corelib_interface_list { \
        __corelib_interface_list() { StaticType()->RegisterInterfaces<__corelib_curclass, __VA_ARGS__>(); } \
    } __corelib_interface_list_init_;

//反射工厂创建函数声明
#define CORELIB_DECL_DYNCINST() \
    static Object* DynCreateInstance(const ParameterPackage& params)



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
    ENUM_CLASS_FLAGS(TypeBinding);

    template<typename T>
    using array_list = std::vector<T>;

    class IInterface
    {
    private:
        friend class Type;
        friend class TypeTraits;
        using __corelib_curclass = IInterface;
        static inline struct __corelib_type {
            __corelib_type() { IInterface::StaticType(); }
        } __corelib_type_init_;
    public:
        static Type* StaticType();
    };
    CORELIB_DECL_SHORTSPTR(IInterface);

    template<typename T>
    concept cltype_concept =
        std::is_base_of<Object, typename remove_shared_ptr<typename std::remove_pointer<T>::type>::type>::value ||
        std::is_base_of<IInterface, typename remove_shared_ptr<typename std::remove_pointer<T>::type>::type>::value;

    template<typename T>
    concept newable_concept = requires { new T; };

    template<typename T>
    concept non_newable_concept = !requires { new T; };

    class Type : public Object
    {
    private:
        friend class Assembly;
        using c_inst_ptr_t = Object * (*)(const ParameterPackage&);
    public:
        using SharedInterfaceGetter = std::function<IInterface_sp(Object_rsp)>;
        using InterfaceGetter = std::function<IInterface*(Object*)>;
    private:
        string name_;
        int32_t structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;
        const std::type_info& typeinfo_;
        array_list<Type*>* template_types_;
        Assembly* assembly_;
        array_list<std::tuple<Type*, InterfaceGetter, SharedInterfaceGetter>> interfaces_;
        bool is_interface_;
    private:
        Type(const Type& r) = delete;
        Type(Type&& r) = delete;

        static inline struct _TypeInit {
            _TypeInit() {
                Type::StaticType();
            }
        } _type_init_;
    public:
        static Type* StaticType();
        virtual Type* GetType() const { return StaticType(); }
        Assembly* GetAssembly() const { return this->assembly_; }
    public:
        virtual int32_t get_structure_size() const { return this->structure_size_; }
        const string& get_name() const { return this->name_; }
        Type* get_base() const { return this->base_; }
        const std::type_info& get_typeinfo() const { return this->typeinfo_; }
        bool is_primitive_type() const;
        bool is_valuetype() const;
        bool is_interface() const { return this->is_interface_; }
    public:
        bool IsImplementedInterface(Type* type);
        //array_list<Type*> GetInterfaces() const;

        bool IsInstanceOfType(const Object* object) const;
        bool IsSharedInstanceOfType(const sptr<Object>& ptr) const;
        bool IsSubclassOf(const Type* type) const;

        Object* CreateInstance(const ParameterPackage& v);
        sptr<Object> CreateSharedInstance(const ParameterPackage& v);
    public:
        virtual string ToString() const override;
    public:
        Type(
            c_inst_ptr_t dyncreate,
            Assembly* assembly,
            Type* base,
            const string& name,
            const std::type_info& info,
            int32_t structure_size,
            bool is_interface = false)
            :
            c_inst_ptr_(dyncreate),
            base_(base),
            name_(name),
            typeinfo_(info),
            structure_size_(structure_size),
            assembly_(assembly),
            is_interface_(is_interface)
        {
        }

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
    public:
        void RegisterInterface(Type* type, const InterfaceGetter& cast, const SharedInterfaceGetter& scast)
        {
            this->interfaces_.push_back({ type, cast, scast });
        }
        IInterface_sp GetSharedInterface(Object_rsp instance, Type* type)
        {
            for (auto& [type, func, sfunc] : this->interfaces_)
            {
                if (type == type)
                {
                    return sfunc(instance);
                }
            }
            return nullptr;
        }
        IInterface* GetInterface(Object* instance, Type* type)
        {
            for (auto& [type, func, sfunc] : this->interfaces_)
            {
                if (type == type)
                {
                    return func(instance);
                }
            }
            return nullptr;
        }
    private:
        template<typename T>
        void _RegisterInterfaces() { }

        template<typename T, typename TInterface, typename... TInterfaces>
        void _RegisterInterfaces()
        {
            RegisterInterface(Typeof<TInterface>(),
                [](Object* obj) -> IInterface* {
                    return (IInterface*)(T*)obj;
                },
                [](Object_rsp obj) -> IInterface_sp {
                    auto sobj = sptr_cast<T>(obj);
                    return sptr_cast<IInterface>(sobj);
                }
            );
            _RegisterInterfaces<T, TInterfaces...>();
        }
    public:
        template<typename T, typename... TInterfaces>
        void RegisterInterfaces()
        {
            _RegisterInterfaces<T, TInterfaces...>();
        }

    public:

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

    //inline bool istype(Object* obj, Type* type)
    //{
    //    return type->IsInstanceOfType(obj);
    //}

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
    struct get_boxing_type
    {};

    template<typename T>
    struct get_boxing_type<T, true>
    {
        using type = remove_shared_ptr<T>::type;
    };

    template<typename T>
    struct get_boxing_type<T, false>
    {
        static_assert(true, "not boxing type!");
        //using type = StdAny;
    };

    //TODO: testing
    template<typename T, typename Ty>
    struct has_boxtype
    {
    };
    template<typename T>
    struct has_boxtype<T, typename get_boxing_type<T>::type> : public std::bool_constant<true>
    {
    };
    template<typename T>
    struct has_boxtype<T, void> : public std::bool_constant<false>
    {
    };


    template<typename T>
    sptr<T> interface_sptr_cast(Object_rsp obj)
    {
        return obj->GetType()->GetInterface(obj, cltypeof<T>());
    }

    template<typename T>
    T* interface_cast(Object* obj)
    {
        return (T*)obj->GetType()->GetInterface(obj, cltypeof<T>());
    }

    template<typename T, bool iscl = cltype_concept<T>>
    struct get_object_pointer
    {
    };

    template<typename T>
    struct get_object_pointer<T, true>
    {
        static sptr<Object> get(sptr<T>& t)
        {
            return t;
        }
    };

    template<typename T>
    struct get_object_pointer<T, false>
    {
        using ClType = get_boxing_type<T>::type;
        static sptr<Object> get(const T& t)
        {
            return mksptr(new ClType(t));
        }
    };

    template<typename T, bool iscl = cltype_concept<T>>
    struct object_assign
    {};

    template<typename T>
    struct object_assign<T, true>
    {
        static void assign(sptr<T>* target, sptr<Object>& value)
        {
            *target = std::static_pointer_cast<T>(value);
        }
    };
    template<typename T>
    struct object_assign<T, false>
    {
        using ClType = get_boxing_type<T>::type;
        static void assign(T* target, sptr<Object>& value)
        {
            auto ptr = static_cast<ClType*>(value.get());
            *target = ptr->get_unboxing_value();
        }
    };


    template <typename, template <typename...> class Op, typename... T>
    struct is_detected_impl : std::false_type {};
    template <template <typename...> class Op, typename... T>
    struct is_detected_impl<std::void_t<Op<T...>>, Op, T...> : std::true_type {};

    template <template <typename...> class Op, typename... T>
    using is_detected = is_detected_impl<void, Op, T...>;

}

