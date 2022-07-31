#include "JsonSerializer.h"
#include "json.hpp"
#include <CoreLib/Reflection.h>

namespace JxCoreLib::Serialization
{
    using namespace nlohmann;

    //在子函数内处理对象

    static void _SerializeArray(IList* list, json& js)
    {
        int32_t count = list->GetCount();
        for (int32_t i = 0; i < count; i++)
        {
            Object_rsp element = list->At(i);
            _Serialize(element.get(), )
        }
    }
    static void _SerializeClassObject(Object* obj, json& obj_js)
    {
        for (FieldInfo* info : obj->GetType()->get_fieldinfos(TypeBinding::NonPublic))
        {
            Object_sp field_inst = info->GetValue(obj);

            json item_js;

            _Serialize(field_inst.get(), info->get_name(), item_js);
            obj_js[info->get_name()] = item_js;
        }
    }
    static void _Serialize(Object* obj, const string& name, json& js)
    {
        if (obj == nullptr)
        {
            js[name] = json::object();
            return;
        }

        if (obj->GetType()->is_primitive_type())
        {
            PrimitiveObject::Assign(&js[name], obj);
        }
        else
        {
            //object

            if (IList* list = cast_interface<IList>(obj))
            {
                json arr = json::array();
                _SerializeArray(list, arr);
                js[name] = arr;
            }
            else
            {
                //other object

                json _js;

                for (FieldInfo* info : obj->GetType()->get_fieldinfos(TypeBinding::NonPublic))
                {
                    Object_sp field_inst = info->GetValue(obj);

                    json obj_js = json::object();

                    _Serialize(field_inst.get(), info->get_name(), obj_js);
                    js[info->get_name()] = obj_js;
                }
            }
        }
    }
    

    string JsonSerializer::Serialize(Object* obj, bool isIndent)
    {
        using namespace nlohmann;
        json js = json::object();
        _SerializeClassObject(obj, js);
        return js.dump(isIndent ? 4 : -1);
    }



    //template<typename T>
    //static bool _GetPrimitiveValue(const json& js, Type* type, Object** out_obj)
    //{
    //    if (type == cltypeof<T>())
    //    {
    //        *out_obj = new T{ js.get<typename T::type>() };
    //    }
    //}

    //static Object* _GetPrimitiveValue(const json& js, Type* field_type)
    //{
    //    if (js.is_boolean()) return new Boolean{ js.get<bool>() };
    //    if (js.is_number_integer())
    //    {
    //        Object* obj = nullptr;
    //        auto r =
    //            _GetPrimitiveValue<Integer32>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<Integer64>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<Integer16>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<Integer8>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<UInteger32>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<UInteger64>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<UInteger16>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<UInteger8>(js, field_type, &obj);
    //        return obj;
    //    }

    //    if (js.is_number_float())
    //    {
    //        Object* obj = nullptr;
    //        auto r =
    //            _GetPrimitiveValue<Single32>(js, field_type, &obj) ||
    //            _GetPrimitiveValue<Double64>(js, field_type, &obj);
    //        return obj;
    //    }
    //    if (js.is_string())
    //    {
    //        return new String{ js.get<string>() };
    //    }
    //    return nullptr;
    //}

    template<typename T>
    static bool _DeserializeSetValue(Object* obj, FieldInfo* info, const nlohmann::json& js, bool b)
    {
        if (b) {
            sptr<Object> p = mksptr((Object*)new get_cltype<T>::type(js.get<T>()));
            info->SetValue(obj, p);
            return true;
        }
        return false;
    }


    static sptr<Object> _Deserialize(const json& js, Type* type)
    {

        sptr<Object> obj = type->CreateSharedInstance({});

        for (auto it = js.begin(); it != js.end(); it++)
        {
            FieldInfo* field_info = type->get_fieldinfo(it.key().c_str());
            if (field_info == nullptr)
            {
                continue;
            }


            if ((*it).is_object())
            {
                //if (field_info->get_field_type()->IsSubclassOf(cltypeof<ManagedMapTemplateBase>()))
                if (false)
                {
                    //map
                    //ManagedMap<Object*, Object*>* map = new ManagedMap<Object*, Object*>;

                    for (auto map_js_it = (*it).begin(); map_js_it != (*it).end(); map_js_it++)
                    {
                        if ((*map_js_it).is_primitive())
                        {
                            //map->insert(new String{ map_js_it.key() }, _GetPrimitiveValue(*map_js_it, ));
                        }
                        else
                        {
                            //deser
                        }
                    }

                    //field_info->SetValue(obj, map);
                }
                else
                {
                    //field object
                    field_info->SetValue(obj.get(), _Deserialize(*it, field_info->get_field_type()));
                }
            }
            else
            {
                bool result =
                    _DeserializeSetValue<bool>(obj.get(), field_info, *it, (*it).is_boolean()) ||
                    _DeserializeSetValue<int>(obj.get(), field_info, *it, (*it).is_number_integer()) ||
                    _DeserializeSetValue<string>(obj.get(), field_info, *it, (*it).is_string()) ||
                    _DeserializeSetValue<float>(obj.get(), field_info, *it, (*it).is_number_float());
            }

        }

        return obj;
    }

    sptr<Object> JsonSerializer::Deserialize(const string& jstr, Type* type)
    {
        return _Deserialize(nlohmann::json::parse(jstr), type);
    }

}
