#include "JsonSerializer.h"
#include "json.hpp"

namespace JxCoreLib
{
    static void _Serialize(Object* obj, std::vector<FieldInfo*> infos, nlohmann::json& js)
    {
        using namespace nlohmann;
        for (FieldInfo* info : infos)
        {
            if (info->get_field_type()->is_primitive_type())
            {
                FieldInfo::Assign(&js[info->get_name()], obj, info);
            }
            else
            {
                json _js;
                Object* inst = info->GetValue(obj);
                if (inst != nullptr)
                {
                    _js = json::object();
                }
                _Serialize(inst, info->get_field_type()->get_fieldinfos(), _js);
                js[info->get_name()] = _js;
            }
        }
    }

    string JxCoreLib::JsonSerializer::Serialize(Object* obj, bool isIndent)
    {
        using namespace nlohmann;
        json js;
        _Serialize(obj, obj->GetType()->get_fieldinfos(), js);
        return js.dump(isIndent ? 4 : -1);
    }



    template<typename T>
    static bool _DeserializeSetValue(Object* obj, FieldInfo* info, const nlohmann::json& js, bool b)
    {
        if (b) {
            auto p = new get_cltype<T>::type(js.get<T>());
            info->SetValue(obj, p);
            return true;
        }
        return false;
    }
    static Object* _Deserialize(const nlohmann::json& js, Type* type)
    {
        Object* obj = type->CreateInstance();

        for (auto it = js.begin(); it != js.end(); it++)
        {
            FieldInfo* info = type->get_fieldinfo(it.key().c_str());
            if (info == nullptr)
            {
                continue;
            }
            if ((*it).is_object())
            {
                info->SetValue(obj, _Deserialize(*it, info->get_field_type()));
            }
            else
            {
                bool result =
                    _DeserializeSetValue<bool>(obj, info, *it, (*it).is_boolean()) ||
                    _DeserializeSetValue<int>(obj, info, *it, (*it).is_number_integer()) ||
                    _DeserializeSetValue<string>(obj, info, *it, (*it).is_string()) ||
                    _DeserializeSetValue<float>(obj, info, *it, (*it).is_number_float());
            }

        }

        return obj;
    }

    Object* JsonSerializer::Deserialize(const string& jstr, Type* type)
    {
        return _Deserialize(nlohmann::json::parse(jstr), type);
    }

}
