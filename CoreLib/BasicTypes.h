#pragma once
#include "Object.h"
#include "Type.h"
#include "Assembly.h"

namespace JxCoreLib
{
    class ValueTypeObject : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::ValueTypeObject, Object);

    };

    class PrimitiveObject : public ValueTypeObject
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::PrimitiveObject, ValueTypeObject);

    };

#define __CORELIB_DEF_BASE_TYPE(Class, DataType) \
    class Class final : public PrimitiveObject \
    { \
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Class, PrimitiveObject);\
        CORELIB_DECL_DYNCINST() { \
            if (params.Count() != 1 || !params.Check<DataType>()) \
            { \
                return nullptr; \
            } \
            return new Class{ params.Get<DataType>(0) }; \
        } \
    public: \
        using type = DataType; \
        DataType value; \
        DataType get_raw_value() { return this->value; } \
        Class(DataType value) : value(value) { } \
        operator DataType() { return value; } \
        virtual string ToString() const override { return std::to_string(value); } \
    }; \
    CORELIB_DECL_SHORTSPTR(Class); \
    static bool operator==(const Class& l, const DataType& r) { return l.value == r; } \
    static bool operator==(const DataType& l, const Class& r) { return l == r.value; } \
    static bool operator!=(const Class& l, const DataType& r) { return l.value != r; } \
    static bool operator!=(const DataType& l, const Class& r) { return l != r.value; } \
    template<> struct get_cltype<DataType> { using type = Class; };


    __CORELIB_DEF_BASE_TYPE(CharObject, char);
    __CORELIB_DEF_BASE_TYPE(Integer8, int8_t);
    __CORELIB_DEF_BASE_TYPE(UInteger8, uint8_t);
    __CORELIB_DEF_BASE_TYPE(Integer16, int16_t);
    __CORELIB_DEF_BASE_TYPE(UInteger16, uint16_t);
    __CORELIB_DEF_BASE_TYPE(Integer32, int32_t);
    __CORELIB_DEF_BASE_TYPE(UInteger32, uint32_t);
    __CORELIB_DEF_BASE_TYPE(Integer64, int64_t);
    __CORELIB_DEF_BASE_TYPE(UInteger64, uint64_t);
    __CORELIB_DEF_BASE_TYPE(Single32, float);
    __CORELIB_DEF_BASE_TYPE(Double64, double);
    __CORELIB_DEF_BASE_TYPE(Boolean, bool);

    class String final : public PrimitiveObject, public string
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::String, PrimitiveObject);
        CORELIB_DECL_DYNCINST()
        {
            if (params.Count() != 1 || !params.Check<const char*>())
            {
                return nullptr;
            }
            return new String{ params.Get<const char*>(0) };
        }
    public:
        using string::basic_string;
        String(const string& right) : string::basic_string(right) {  }

        virtual string ToString() const override { return *this; }
        string get_raw_value() { return *this; }
        static sptr<String> FromString(string_view str)
        {
            return mksptr(new String(str));
        }
    };
    CORELIB_DECL_SHORTSPTR(String);
    template<> struct get_cltype<string> { using type = String; };

    class StdAny : public Object, public std::any
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::StdAny, Object);
        CORELIB_DECL_DYNCINST() {
            return nullptr;
        }
    public:

    private:
        template<typename TValue>
        static bool _AnyCast(const std::any& any, TValue* t) { return false; }

        template<typename TValue, typename TCastable1, typename... TCastable>
        static bool _AnyCast(const std::any& any, TValue* t)
        {
            auto name = any.type().name();
            auto cname = typeid(TCastable1).name();
            if (any.type() == typeid(TCastable1))
            {
                *t = std::any_cast<TCastable1>(any);
                return true;
            }
            else
            {
                return _AnyCast<TValue, TCastable...>(any, t);
            }
            return false;
        }

    public:
        template<typename TValue, typename... TCastable>
        static bool AnyCast(const std::any& any, TValue* t)
        {
            return _AnyCast<TValue, TCastable...>(any, t);
        }
    };
    template<> struct get_cltype<std::any> { using type = StdAny; };



    template<typename... T>
    struct TemplateTypePair
    {
        static std::vector<Type*>* GetTemplateTypes()
        {
            std::vector<Type*>* vec = new std::vector<Type*>;
            (vec->push_back(cltypeof<typename get_cltype<T>::type>()), ...);
            return vec;
        }
    };


    class IList : public IInterface
    {
        CORELIB_DEF_INTERFACE(AssemblyObject_JxCoreLib, JxCoreLib::IList, IInterface);

        virtual void Add(const sptr<Object>& value) = 0;
        virtual sptr<Object> At(int32_t index) = 0;
        virtual void Clear() = 0;
        virtual void RemoveAt(int32_t index) = 0;
        virtual int32_t IndexOf(const sptr<Object>& value) = 0;
        virtual bool Contains(const sptr<Object>& value) = 0;
        virtual int32_t GetCount() const = 0;
    };
    CORELIB_DECL_SHORTSPTR(IList);

    template<typename T>
    struct BoxUtil
    {
        static inline sptr<Object> Box(const T& value) { return mksptr((Object*)new get_cltype<T>::type(value)); }
    };

    template<typename T>
    struct UnboxUtil
    {
        static inline T Unbox(Object* value)
        {
            //value->GetType()->is_valuetype(); //assert
            return static_cast<get_cltype<T>::type*>(value)->get_raw_value();
        }
        static inline T Unbox(const sptr<Object>& value)
        {
            //value->GetType()->is_valuetype(); //assert
            return static_cast<get_cltype<T>::type*>(value.get())->get_raw_value();
        }
    };


    template<typename T>
    class List : public Object, public array_list<T>, public IList
    {
        CORELIB_DEF_TEMPLATE_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::List, Object, T);
        CORELIB_IMPL_INTERFACES(IList);
        static_assert( (cltype_concept<T> && is_shared_ptr<T>::value) || !cltype_concept<T>, "");
        constexpr static bool is_shared_cltype = cltype_concept<T> && is_shared_ptr<T>::value;
    public:
        virtual void Add(const sptr<Object>& value) override
        {
            if constexpr (is_shared_cltype)
            {
                this->push_back(std::static_pointer_cast<typename remove_shared_ptr<T>::type>(value));
            }
            else
            {
                this->push_back(UnboxUtil<T>::Unbox(value.get()));
            }
        }
        virtual sptr<Object> At(int32_t index) override
        {
            if constexpr (is_shared_cltype)
            {
                return std::static_pointer_cast<Object>(this->at(index));
            }
            else
            {
                return BoxUtil<T>::Box(this->at(index));
            }
        }
        virtual void Clear() override { this->clear(); }
        virtual void RemoveAt(int32_t index) override
        {
            this->erase(this->begin() + index);
        }
        virtual int32_t IndexOf(const sptr<Object>& value) override
        {
            for (int32_t i = 0; i < this->size(); i++)
            {
                auto& item = this->at(i);
                if constexpr (is_shared_cltype)
                {
                    if (value == nullptr && item == nullptr)
                    {
                        return i;
                    }
                    else if (item != nullptr && item->Equals(value))
                    {
                        return i;
                    }
                }
                else
                {
                    if (item == UnboxUtil<T>::Unbox(value.get()))
                    {
                        return i;
                    }
                }
            }
            return -1;
        }
        virtual bool Contains(const sptr<Object>& value) override
        {
            return this->IndexOf(value) >= 0;
        }
        virtual int32_t GetCount() const override { return static_cast<int32_t>(this->size()); }
    };

}