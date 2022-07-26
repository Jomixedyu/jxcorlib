/*
* @Moudule     : Object
* @Date        : 2021/04/17
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef _CORELIB_OBJECT_H
#define _CORELIB_OBJECT_H

#include <vector>
#include <type_traits>
#include <memory>
#include "UString.h"
#include "istring.h"

namespace JxCoreLib
{
    class Type;

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
        virtual bool Equals(const sptr<Object>& object) const;
    };
    
#define SPTR_DECL(CLASS) using s##CLASS = sptr<class CLASS>; using rs##CLASS = const sptr<class CLASS>&;

    SPTR_DECL(Object);

    template<typename T>
    concept cltype_concept =
        std::is_base_of<Object, typename remove_shared_ptr<typename std::remove_pointer<T>::type>::type>::value;

    template<typename T>
    concept newable_concept = requires { new T; };

    template<typename T>
    concept non_newable_concept = !requires { new T; };

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
#endif // !_CORELIB_OBJECT_H
