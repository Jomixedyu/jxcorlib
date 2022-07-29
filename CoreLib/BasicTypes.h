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
        string get_raw_value()  { return *this; }
        static sptr<String> FromString(string_view str)
        {
            return mksptr(new String(str));
        }
    };
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
        virtual void Contains(const sptr<Object>& value) = 0;
        virtual int32_t GetCount() const = 0;
    };


    template<typename T>
    struct BoxUtil
    {
        static inline sptr<Object> Box(const T& value) { return mksptr((Object*)new get_cltype<T>::type(value)); }
    };

    template<typename T>
    struct UnboxUtil
    {
        static inline T UnBox(Object* value)
        {
            //value->GetType()->is_valuetype(); //assert
            return static_cast<get_cltype<T>::type*>(value)->get_raw_data();
        }
    };


    template<typename T>
    class List : public Object, public array_list<T>, public IList
    {
        CORELIB_DEF_TEMPLATE_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::List, Object, T);
        static_assert(!cltype_concept<T> || (cltype_concept<T> && !std::is_pointer<T>::value), "as");
    public:
        virtual void Add(const sptr<Object>& value) override
        {
            if (!cltype_concept<T>)
            {
            this->push_back(UnboxUtil<T>::UnBox(value.get()));
            }
            else
            {

            }
        }
        virtual sptr<Object> At(int32_t index) override
        {

        }
        virtual void Clear() override;
        virtual void RemoveAt(int32_t index) override;
        virtual void Contains(const sptr<Object>& value) override;
        virtual int32_t GetCount() const override;
    };


}