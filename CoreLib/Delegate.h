#pragma once
#include "Assembly.h"
#include "Object.h"
#include "Type.h"
#include "BasicTypes.h"
#include <functional>
#include "CommonException.h"

namespace JxCoreLib
{
    class Delegate : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Delegate, Object);

    public:
        virtual Object_sp DynamicInvoke(const array_list<Object_sp>& params) = 0;
    };

    template<typename T>
    class MakeReturnObject
    {
    public:
        static Object_sp GetValue(T&& t)
        {
            if constexpr (cltype_concept<T>)
            {
                return t;
            }
            else
            {
                return BoxUtil::Box(t);
            }
        }
    };

    template<>
    class MakeReturnObject<void>
    {
    public:
        static Object_sp GetValue() { return nullptr; }
    };

    template<typename TReturn, typename... TArgs>
    class FunctionDelegate : public Delegate
    {
        CORELIB_DEF_TEMPLATE_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::FunctionDelegate, Delegate, TReturn, TArgs...);

        using This = FunctionDelegate<TReturn, TArgs...>;
        using FunctionType = std::function<TReturn(TArgs...)>;
        using FunctionPointer = TReturn(*)(TArgs...);

        enum class FunctionInfoType
        {
            Static, Lambda, Member
        };

        class FunctionInfo
        {
        public:
            FunctionInfoType type;
        public:
            FunctionInfo(const FunctionInfoType& type) : type(type)
            {
            }
        public:
            virtual bool Equals(FunctionInfo* Func) = 0;
            virtual TReturn Invoke(TArgs... args) = 0;
        };

        class StaticFunctionInfo : public FunctionInfo
        {
        public:
            FunctionPointer ptr_;
            StaticFunctionInfo(FunctionPointer ptr) : FunctionInfo(FunctionInfoType::Static), ptr_(ptr)
            {
            }
            virtual bool Equals(FunctionInfo* func) override
            {
                if (func == nullptr || this->type != func->type) return false;
                return this->ptr_ == static_cast<StaticFunctionInfo*>(func)->ptr_;
            }
            virtual TReturn Invoke(TArgs... args) override
            {
                return (*this->ptr_)(args...);
            }
        };

        class LambdaFunctionInfo : public FunctionInfo
        {
        public:
            FunctionType func_;
            LambdaFunctionInfo(const FunctionType& func)
                : FunctionInfo(FunctionInfoType::Lambda), func_(func)
            {
            }
            virtual bool Equals(FunctionInfo* func) override
            {
                if (func == nullptr || this->type != func->type) return false;
                //todo: error
                return this->func_.target() == static_cast<LambdaFunctionInfo*>(func)->func_.target();
            }
            virtual TReturn Invoke(TArgs... args) override
            {
                return this->func_(args...);
            }
        };

        template<typename TObj>
        class MemberFunctionInfo : public FunctionInfo
        {
        public:
            sptr<TObj> instance_;
            TReturn(TObj::* ptr_)(TArgs...);

            MemberFunctionInfo(const sptr<TObj>& instance, TReturn(TObj::* ptr)(TArgs...))
                : FunctionInfo(FunctionInfoType::Member), instance_(instance), ptr_(ptr)
            {
            }
            virtual bool Equals(FunctionInfo* func) override
            {
                if (func == nullptr || this->type != func->type) return false;
                //todo
                return false;
            }
            virtual TReturn Invoke(TArgs... args) override 
            {
                return (this->instance_.get()->*ptr_)(args...);
            }
        };

    protected:
        FunctionInfo* func_ptr_;

        FunctionDelegate(FunctionPointer funcptr) : func_ptr_(new StaticFunctionInfo(funcptr)) {}
        FunctionDelegate(const FunctionType& func) : func_ptr_(new LambdaFunctionInfo(func)) {}

        template<typename TObject>
        FunctionDelegate(const sptr<TObject>& obj, TReturn(TObject::* ptr)(TArgs...))
            : func_ptr_(new MemberFunctionInfo<TObject>(obj, ptr))
        {}
    public:

        static sptr<FunctionDelegate> FromRaw(FunctionPointer funcptr)
        {
            return mksptr(new FunctionDelegate(funcptr));
        }
        static sptr<FunctionDelegate> FromLambda(const FunctionType& func)
        {
            return mksptr(new FunctionDelegate(func));
        }

        template<typename TObject>
        static sptr<FunctionDelegate> FromMember(const sptr<TObject>& obj, TReturn(TObject::* ptr)(TArgs...))
        {
            return mksptr(new FunctionDelegate(obj, ptr));
        }

        FunctionDelegate() = delete;
        FunctionDelegate(const FunctionDelegate&) = delete;
        FunctionDelegate(FunctionDelegate&&) = delete;

        ~FunctionDelegate()
        {
            delete this->func_ptr_;
        }

        virtual bool Equals(Object* object) override
        {
            if (object == nullptr || object->GetType() != this->GetType()) return false;
            return this->func_ptr_->Equals(static_cast<This*>(object)->func_ptr_);
        }
        TReturn Invoke(TArgs... args)
        {
            return this->func_ptr_->Invoke(args...);
        }
        virtual Object_sp DynamicInvoke(const array_list<Object_sp>& params) override
        {
            //constexpr int size = sizeof...(TArgs);
            //auto arr = array_list<Type*>{ cltypeof<typename get_boxing_type<TArgs>::type>()... };
            //assert(arr.size() == size);
            //if constexpr (size == 0)
            //    return MakeReturnObject<TReturn>(this->func_ptr_->Invoke(UnboxUtil::Unbox<> params.at(0)));

            throw NotImplementException();
        }
    };

    //template<typename TReturn, typename... TArgs>
    //TReturn Invoke(auto func, const array_list<Object_sp>& params)
    //{
    //    return MakeReturnObject<TReturn>(func(UnboxUtil::Unbox<typename get_boxing_type<TArgs>::type>(params)...));
    //}
}