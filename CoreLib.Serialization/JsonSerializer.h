#ifndef CORELIB_JSONSERIALIZER_H
#define CORELIB_JSONSERIALIZER_H

#include "Module.h"
#include <CoreLib/Reflection.h>
#include <memory>
#include "json.hpp"

namespace JxCoreLib::Serialization
{
    class JsonSerializer
    {
    public:
        static string Serialize(Object* obj, bool isIndent = false);
    public:
        static Object* Deserialize(const string& jstr, Type* type);
        template<typename T>
        static T* Deserialize(const string& str)
        {
            return static_cast<T*>(Deserialize(str, cltypeof<T>()));
        }
        static sptr<Object> DeserializeShared(const string& str, Type* type)
        {
            return mksptr(Deserialize(str, type));
        }

        template<typename T>
        static sptr<T> DeserializeShared(const string& str)
        {
            return std::static_pointer_cast<T>(DeserializeShared(str, cltypeof<T>()));
        }
    };

    class JsonSerializable
    {
    public:
        virtual nlohmann::json Serialize() = 0;
        virtual void Deserialize(const nlohmann::json& json) = 0;
    };
}

#endif // !CORELIB_JSONSERIALIZER_H