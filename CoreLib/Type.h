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

#include "UString.h"
#include "Object.h"
#include "EnumUtil.h"
#include "IInterface.h"


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
    __CORELIB_DEF_BASETYPE_META(NAME, BASE)

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
        type = new Type(dynptr, assm, cltypeof<BASE>(), StringUtil::Concat(#NAME, "<", typeid(TemplateType).name(), ">"), typeid(NAME), sizeof(NAME)); \
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
        static inline struct _TypeInit { \
            _TypeInit() { NAME<__VA_ARGS__>::StaticType(); } \
        } __corelib_type_init_;

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

    class Type : public Object
    {
    private:
        friend class Assembly;
        using c_inst_ptr_t = Object * (*)(const ParameterPackage&);
    private:
        string name_;
        int32_t structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;
        const std::type_info& typeinfo_;
        array_list<Type*>* template_types_;
        Assembly* assembly_;
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

    public:
        virtual string ToString() const override;
    public:
        bool IsInstanceOfType(const Object* object) const;
        bool IsSharedInstanceOfType(const sptr<Object>& ptr) const;
        bool IsSubclassOf(const Type* type) const;
    public:
        
        Object* CreateInstance(const ParameterPackage& v);
        sptr<Object> CreateSharedInstance(const ParameterPackage& v);

    public:
        Type(
            c_inst_ptr_t dyncreate,
            Assembly* assembly,
            Type* base,
            const string& name,
            const std::type_info& info,
            int32_t structure_size)
            :
            c_inst_ptr_(dyncreate),
            base_(base),
            name_(name),
            typeinfo_(info),
            structure_size_(structure_size),
            assembly_(assembly)
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
    struct get_cltype
    {};

    template<typename T>
    struct get_cltype<T, true>
    {
        using type = remove_shared_ptr<T>::type;
    };

    template<typename T>
    struct get_cltype<T, false>
    {
        using type = StdAny;
    };




    //template<> struct get_cltype<array_list> { using type = ArrayList; };

    //using map = std::map<sptr<Object>, sptr<Object>>;
    //class Map : public Object, public map
    //{
    //    CORELIB_DEF_TYPE(JxCoreLib::Map, Object);
    //public:
    //};
    //template<> struct get_cltype<map> { using type = map; };

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
        using ClType = get_cltype<T>::type;
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
        using ClType = get_cltype<T>::type;
        static void assign(T* target, sptr<Object>& value)
        {
            auto ptr = static_cast<ClType*>(value.get());
            *target = ptr->get_raw_value();
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

