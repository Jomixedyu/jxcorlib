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
        static sptr<Object> Deserialize(const string& jstr, Type* type);
        template<typename T>
        static sptr<T> Deserialize(const string& str)
        {
            return std::static_pointer_cast<T>(Deserialize(str, cltypeof<T>()));
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