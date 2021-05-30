#ifndef CORELIB_TYPE_H
#define CORELIB_TYPE_H

#include <vector>
#include "Object.h"

namespace JxCoreLib
{
    struct CreateInstParamData
    {
        std::vector<Object*> data;
        int len;
    };

    class Type : public Object
    {
    private:
        using c_inst_ptr_t = Object * (*)(CreateInstParamData*);
    private:
        int id_;
        String name_;
        int structure_size_;
        Type* base_;
        c_inst_ptr_t c_inst_ptr_;

    private:
        Type(int id, const String& name, Type* base, c_inst_ptr_t c_inst_ptr, int structure_size);
        Type(const Type& r) = delete;
        Type(Type&& r) = delete;
    public:
        static Type* __meta_type();
        virtual Type* get_type() const;
    public:
        virtual int get_structure_size() const;
        const String& get_name() const;
        Type* get_base() const;
    public:
        virtual String ToString() const override;
    public:
        bool IsInstanceOfType(Object* object);
        /*
        * 确定当前 Type 表示的类是否是从指定的 Type 表示的类派生的。
        */
        bool IsSubclassOf(Type* type);
        Object* CreateInstance(CreateInstParamData* v = nullptr);
    public:
        static Type* GetType(const String& str);
        static Type* GetType(const char*& str);
        static Type* GetType(int id);
        static std::vector<Type*> GetTypes();
    public:
        static int Register(Object* (*dynCreate)(CreateInstParamData*), Type* base, const String& name, int structure_size);
    };

#ifdef CORELIB_AUTOINIT
    DEF_TYPE_INIT(Type);
#endif

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
    inline void RegisterClass()
    {
        typeof<T>();
    }
}

#endif