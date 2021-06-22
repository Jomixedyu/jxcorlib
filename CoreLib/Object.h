/*
* @Moudule     : Object
* @Date        : 2021/04/17
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JxmiXedYu/JxCode.CoreLib
* @StdRequired : c++20
*/

#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

#include <vector>
#include "String.h"

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

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
#endif // !CORELIB_OBJECT_H
