/*
* @Moudule     : Object
* @Date        : 2021/04/17
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JxmiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef _CORELIB_OBJECT_H
#define _CORELIB_OBJECT_H

#include <vector>
#include <type_traits>
#include "UString.h"

namespace JxCoreLib
{
    class Type;

    class Object
    {
    private:
        static Type* __meta_type();
        static inline struct _ObjectInit {
            _ObjectInit() {
                Object::__meta_type();
            }
        } _object_init_;
    private:
        std::vector<Object*>* managed_child_ = nullptr;
        Object* parent_ = nullptr;
    private:
        void AddManagedChild(Object* child);
        void RemoveManagedChild(Object* child);
    public:
        void SetManagedParent(Object* parent);
    public:
        virtual Type* get_type() const;
        friend class Type;
    public:
        Object() {}
        virtual ~Object();
    public:
        virtual string ToString() const;
    };

    template<typename T>
    concept baseof_object_concept = std::is_base_of<Object, T>::value;

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
