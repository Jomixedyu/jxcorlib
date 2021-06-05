#ifndef CORELIB_TYPE_H
#define CORELIB_TYPE_H

#include <vector>
#include <any>

#include "CoreLibConfig.h"
#include "Object.h"

//元数据声明
#define DEF_OBJECT_META(NAME, BASE) \
private: \
    friend class Type; \
    static inline Type* __meta_type() { \
        static int id = -1; \
        if (id == -1) { \
            id = Type::Register(DynCreateInstance, typeof<BASE>(), NAMEOF(NAME), sizeof(NAME)); \
        } \
        return Type::GetType(id); \
    } \
public: \
    inline virtual Type* get_type() const override { \
        return __meta_type(); \
    } \
private: \
    using base = BASE; \
    static inline struct _TypeInit{ \
        _TypeInit() \
        { \
            if constexpr(CORELIB_AUTOINIT) {\
                NAME::__meta_type(); \
            }\
        } \
    } __type_init_; \

//反射工厂创建函数声明
#define DECL_OBJECT_DYNCREATEINSTANCE() \
    static Object* DynCreateInstance(const ParameterPackage& params)


namespace JxCoreLib
{
    struct ParameterPackage;

    class Type : public Object
    {
    private:
        using c_inst_ptr_t = Object * (*)(const ParameterPackage&);
    private:
        int id_;
        string name_;
        int structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;

    private:
        Type(int id, const string& name, Type* base, c_inst_ptr_t c_inst_ptr, int structure_size);
        Type(const Type& r) = delete;
        Type(Type&& r) = delete;
#ifdef CORELIB_AUTOINIT
        DEF_TYPE_INIT(Type);
#endif
    public:
        static Type* __meta_type();
        virtual Type* get_type() const;
    public:
        virtual int get_structure_size() const;
        const string& get_name() const;
        Type* get_base() const;
    public:
        virtual string ToString() const override;
    public:
        bool IsInstanceOfType(Object* object);
        /*
        * 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。
        */
        bool IsSubclassOf(Type* type);
        Object* CrearteInstance();
        Object* CreateInstance(const ParameterPackage& v);
    public:
        static Type* GetType(const string& str);
        static Type* GetType(const char*& str);
        static Type* GetType(int id);
        static std::vector<Type*> GetTypes();
    public:
        static int Register(c_inst_ptr_t dyncreate, Type* base, const string& name, int structure_size);
        template<typename T>
        static inline Type* Typeof()
        {
            return T::__meta_type();
        }
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
        DEF_OBJECT_META(JxCoreLib::ParameterPackage, Object);
        DECL_OBJECT_DYNCREATEINSTANCE();
    private:
        std::vector<std::any> data;
    public:
        ParameterPackage() {}
        ParameterPackage(std::initializer_list<std::any> lst) : data(lst.begin(), lst.end()) { }
        template<typename T>
        void Add(const T& v) {
            data.push_back(std::any(v));
        }
        template<typename T>
        T Get(const int& index) const {
            return std::any_cast<T>(data[index]);
        }
        size_t Count() const { return data.size(); }
        bool IsEmpty() const { return this->data.empty(); }
    private:
        template<int I>  bool _Check() const { return true; }

        template<int I, typename T, typename... TArgs>
        bool _Check() const {
            return (this->data[I].type() == typeid(T)) & _Check<I + 1, TArgs...>();
        }
    public:
        template<typename... TArgs>
        bool Check() const {
            if (this->data.size() != sizeof...(TArgs)) {
                return false;
            }
            return _Check<0, TArgs...>();
        }
    };
}

#endif