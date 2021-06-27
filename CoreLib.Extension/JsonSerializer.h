#ifndef CORELIB_JSONSERIALIZER_H
#define CORELIB_JSONSERIALIZER_H

#include <CoreLib/CoreLib.h>
#include <CoreLib/Reflection.h>

namespace JxCoreLib
{
    class JsonSerializer
    {
    public:
        static string Serialize(Object* obj);
    public:
        static Object* Deserialize(const string& jstr, Type* type);
        template<typename T>
        static T* Deserialize(const string& str)
        {
            return static_cast<T*>(Deserialize(str, cltypeof<T>()));
        }
    };
}

#endif // !CORELIB_JSONSERIALIZER_H




