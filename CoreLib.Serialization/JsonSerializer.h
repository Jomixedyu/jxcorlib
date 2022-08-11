#ifndef CORELIB_JSONSERIALIZER_H
#define CORELIB_JSONSERIALIZER_H

#include "Assembly.h"

namespace JxCoreLib::Serialization
{
    struct JsonSerializerSettings
    {
        int32_t indent_space = -1;
        bool string_enum = false;
    };

    class JsonSerializer
    {
    public:
        static string Serialize(Object* obj, const JsonSerializerSettings& settings);
    public:
        static sptr<Object> Deserialize(const string& jstr, Type* type);
        template<typename T>
        static sptr<T> Deserialize(const string& str)
        {
            return std::static_pointer_cast<T>(Deserialize(str, cltypeof<T>()));
        }
    };

}

#endif // !CORELIB_JSONSERIALIZER_H