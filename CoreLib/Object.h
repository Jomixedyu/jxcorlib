/*
* @Moudule     : Object
* @Date        : 2021/04/17
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#pragma once

#include <vector>
#include <type_traits>
#include <memory>
#include "UString.h"


namespace JxCoreLib
{
    class Type;
    class Assembly;

    struct AssemblyTypeObject
    {
        const char* name;
    };

#define CORELIB_DECL_ASSEMBLY(NAME) \
    inline struct __corelib_AssemblyClass_##NAME : public ::JxCoreLib::AssemblyTypeObject \
    {  __corelib_AssemblyClass_##NAME##() { name = #NAME; } } AssemblyObject_##NAME;

    CORELIB_DECL_ASSEMBLY(JxCoreLib);



    template<typename T>
    using sptr = std::shared_ptr<T>;

    template<typename T>
    sptr<T> mksptr(T* t) { return sptr<T>(t); }

    template<typename T>
    using wptr = std::weak_ptr<T>;

    template<typename T>
    wptr<T> mkwptr(const sptr<T>& ptr) { return wptr<T>(ptr); }

    template<typename T, typename = void>
    struct is_shared_ptr
    {
        constexpr inline static bool value = false;
    };
    template<typename T>
    struct is_shared_ptr<T, std::void_t<typename T::element_type>>
    {
        constexpr inline static bool value = std::is_same<T, std::shared_ptr<typename T::element_type>>::value;
    };

    template<typename T, typename = void>
    struct remove_shared_ptr
    {
        using type = T;
    };
    template<typename T>
    struct remove_shared_ptr<T, std::void_t<typename T::element_type>>
    {
        using type = T::element_type;
    };

    class Object
    {
    public:
        static Type* StaticType();
    private:
        static inline struct _ObjectInit {
            _ObjectInit() {
                Object::StaticType();
            }
        } _object_init_;
    private:
    public:
        virtual Type* GetType() const;
        friend class Type;
    public:
        Object() {}
        virtual ~Object();
    public:
        virtual string ToString() const;
        virtual bool Equals(Object* object) const;
    };

#define SPTR_DECL(CLASS) using s##CLASS = sptr<class CLASS>; using rs##CLASS = const sptr<class CLASS>&;
    SPTR_DECL(Object);

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
