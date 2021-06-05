/*
* require c++17
*/

#ifndef CORELIB_OBJECT_H
#define CORELIB_OBJECT_H

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
    public:
        virtual Type* get_type() const;
        friend class Type;
    public:
        Object();
    public:
        virtual string ToString() const;
    };

}
namespace std
{
    string to_string(JxCoreLib::Object* obj);
}
#endif // !CORELIB_OBJECT_H
