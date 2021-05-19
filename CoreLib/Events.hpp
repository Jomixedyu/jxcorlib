#ifndef CORELIB_EVENTS_HPP
#define CORELIB_EVENTS_HPP

#include <list>
#include <functional>

template<typename TReturn, typename... TArgs>
class Events
{
public:
    using FunctionType = std::function<TReturn(TArgs...)>;
    using FunctionPointer = TReturn(*)(TArgs...);

protected:
    enum class FunctionInfoType
    {
        Static,
        Lambda,
        Member
    };
    class FunctionInfo
    {
    public:
        unsigned int index;
        FunctionInfoType type;
    public:
        FunctionInfo(const unsigned int& index, const FunctionInfoType& type)
            : index(index), type(type)
        {
        }
    public:
        bool operator ==(const FunctionInfo& r) {
            return this->index == r.index;
        }
        virtual TReturn Invoke(TArgs...) = 0;
    };
    class StaticFunctionInfo : public FunctionInfo
    {
    public:
        FunctionPointer ptr;
        StaticFunctionInfo(const unsigned int& index, FunctionPointer ptr)
            : FunctionInfo(index, FunctionInfoType::Static), ptr(ptr)
        {
        }
        virtual TReturn Invoke(TArgs... args) override {
            return ptr(args...);
        }
    };
    class LambdaFunctionInfo : public FunctionInfo
    {
    public:
        FunctionType func;
        void* instance;
        LambdaFunctionInfo(
            const unsigned int& index,
            void* instance,
            const FunctionType& func)
            : FunctionInfo(index, FunctionInfoType::Lambda), instance(instance), func(func)
        {
        }
        virtual TReturn Invoke(TArgs... args) override {
            return func(args...);
        }
    };
    template<typename TObj>
    class MemberFunctionInfo : public FunctionInfo
    {
    public:
        TObj* instance;
        TReturn(TObj::* ptr)(TArgs...);

        MemberFunctionInfo(
            const unsigned int& index,
            TObj* instance,
            TReturn(TObj::* ptr)(TArgs...))
            : FunctionInfo(index, FunctionInfoType::Member), instance(instance), ptr(ptr)
        {
        }
        virtual TReturn Invoke(TArgs... args) override {
            return (instance->*ptr)(args...);
        }
    };

protected:
    unsigned int index;
    std::list<FunctionInfo*> eventList;
public:
    int Count() const {
        return this->eventList.size();
    }
public:
    Events() : index(0) {}
    Events(const Events& right) = delete;
    Events(Events&& right) = delete;
    ~Events() {
        this->RemoveAllListener();
    }
protected:
    void RemoveAllListener() {
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            delete* it;
        }
        this->eventList.clear();
    }
public:
    //static
    unsigned int AddListener(FunctionPointer funcPtr) {
        if (funcPtr == nullptr) {
            return 0;
        }
        this->eventList.push_back(new StaticFunctionInfo(++this->index, funcPtr));
        return this->index;
    }
    //member
    template<typename TObj>
    unsigned int AddListener(TObj* obj, TReturn(TObj::* ptr)(TArgs...)) {
        if (obj == nullptr) {
            return 0;
        }
        this->eventList.push_back(new MemberFunctionInfo<TObj>(++this->index, obj, ptr));
        return this->index;
    }
    //lambda
    template<typename TObj>
    unsigned int AddListener(TObj* obj, const FunctionType& func) {
        this->eventList.push_back(new LambdaFunctionInfo(++this->index, obj, func));
        return this->index;
    }

    //static
    unsigned int RemoveListener(FunctionPointer funcPtr) {
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if ((*it)->type == FunctionInfoType::Static
                && static_cast<StaticFunctionInfo*>(*it)->ptr == funcPtr) {
                auto index = (*it)->index;
                delete* it;
                this->eventList.erase(it);
                return index;
            }
        }
        return 0;
    }
    //member
    template<typename TObj>
    unsigned int RemoveListener(TObj* obj, TReturn(TObj::* ptr)(TArgs...)) {
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++)
        {
            if ((*it)->type == FunctionInfoType::Member
                && static_cast<MemberFunctionInfo<TObj>*>(*it)->instance == obj
                && static_cast<MemberFunctionInfo<TObj>*>(*it)->ptr == ptr)
            {
                auto index = (*it)->index;
                delete* it;
                this->eventList.erase(it);
                return index;
            }
        }
        return 0;
    }


    unsigned int RemoveListenerByIndex(unsigned int index) {
        if (index <= 0) {
            return 0;
        }
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if ((*it)->index == index) {
                delete* it;
                this->eventList.erase(it);
                return index;
            }
        }
        return 0;
    }

    unsigned int operator+=(FunctionPointer ptr) {
        return this->AddListener(ptr);
    }

    unsigned int operator-=(FunctionPointer ptr) {
        return this->RemoveListener(ptr);
    }
};

template<typename TReturn, typename... TArgs>
class Delegate : public Events<TReturn, TArgs...>
{
    using  base = Events<TReturn, TArgs...>;
public:
    void Invoke(TArgs... t) {
        for (auto& item : this->eventList) {
            item->Invoke(t...);
        }
    }

    void RemoveAllListener() {
        base::RemoveAllListener();
    }

    //member lambda
    template<typename TObj>
    void RemoveByInstance(TObj* obj) {
        using MemberFunInfo = typename Events<TReturn, TArgs...>::template MemberFunctionInfo<TObj>;
        using LambdaFunInfo = typename Events<TReturn, TArgs...>::LambdaFunctionInfo;

        for (auto it = this->eventList.begin(); it != this->eventList.end(); ) {
            if ((
                (*it)->type == base::FunctionInfoType::Member
                && (static_cast<MemberFunInfo*>(*it))->instance == obj
                ) || (
                (*it)->type == base::FunctionInfoType::Lambda
                && static_cast<LambdaFunInfo*>(*it)->instance == obj
                )
                )
            {
                delete* it;
                it = this->eventList.erase(it);
            }
            else {
                it++;
            }
        }
    }
};

template<typename... TArgs>
using ActionEvents = Events<void, TArgs...>;

template<typename... TArgs>
using Action = Delegate<void, TArgs...>;

template<typename TReturn, typename... TArgs>
using FunctionEvents = Events<TReturn, TArgs...>;

template<typename TReturn, typename... TArgs>
class Function : public Delegate<TReturn, TArgs...>
{
public:
    std::vector<TReturn> InvokeResult(TArgs... args) {
        std::vector<TReturn> retList;
        for (auto& item : this->eventList) {
            retList.push_back(item->Invoke(args...));
        }
        return retList;
    }
};

template<>
class Function<bool> : public Delegate<bool>
{
public:
    std::vector<bool> InvokeResult() {
        std::vector<bool> retList;
        for (auto& item : this->eventList) {
            retList.push_back(item->Invoke());
        }
        return retList;
    }

    bool IsValidReturnInvoke() {
        for (const bool& item : this->InvokeResult()) {
            if (!item) return false;
        }
        return true;
    }
};


#endif // !CORELIB_EVENTS_HPP