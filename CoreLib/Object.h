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



    template<typename T>
    concept cltype_concept =
        std::is_base_of<Object, typename std::remove_pointer<T>::type>::value;

    template<typename T>
    concept cltype_ptr_concept = cltype_concept<T> && std::is_pointer<T>::value;

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
