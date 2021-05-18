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
    class FunctionInfo
    {
    public:
        unsigned int index;
        FunctionType func;
        void* instance;
    public:
        FunctionInfo() : index(0), func(nullptr), instance(nullptr)
        {
        }
        FunctionInfo(
            const unsigned int& index,
            const FunctionType& func,
            void* instance
        ) : index(index), func(func), instance(instance)
        {
        }
    public:
        bool operator ==(const FunctionInfo& r) {
            return this->index == r.index;
        }
    };

#ifdef _MSC_VER
    template<int I>
    using PlaceHolder = std::_Ph<I>;
#else
    using PlaceHolder = std::_Placeholder;
#endif

    template<int N, int... I>
    struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {};

    template<int...I>
    struct MakeSeqs<1, I...>
    {
        template<typename T, typename...Args>
        static FunctionType bind(T* obj, TReturn(T::* _Func)(Args...))
        {
            return std::bind(_Func, obj, PlaceHolder<I>{}...);
        }
    };

protected:
    unsigned int index;
    std::list<FunctionInfo> eventList;
public:
    int Count() const {
        return this->eventList.size();
    }
public:
    Events() : index(0) {}
protected:
    unsigned int Push(const FunctionType& func, void* instance = nullptr) {
        this->eventList.push_back(FunctionInfo(++this->index, func, instance));
        return this->index;
    }
public:
    unsigned int AddListener(FunctionPointer funcPtr) {
        if (funcPtr == nullptr) {
            return 0;
        }
        return this->Push(FunctionType{ funcPtr }, nullptr);
    }

    template<typename TObj>
    unsigned int AddListener(TObj* obj, TReturn(TObj::* ptr)(TArgs...)) {
        return this->Push(MakeSeqs<sizeof...(TArgs) + 1>::bind(obj, ptr), obj);
    }

    template<typename TObj>
    unsigned int AddListener(TObj* obj, const FunctionType& func) {
        return this->Push(func, obj);
    }


    unsigned int RemoveListener(FunctionPointer funcPtr) {
        if (funcPtr == nullptr) {
            return 0;
        }
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if (it->instance == nullptr) {
                if (*it->func.target<FunctionPointer>() == funcPtr) {
                    auto index = it->index;
                    this->eventList.erase(it);
                    return index;
                }
            }
        }
        return 0;
    }
    template<typename TObj>
    unsigned int RemoveListener(TObj* obj, TReturn(TObj::* ptr)(TArgs...)) {
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if (it->instance == obj 
                && *it->func.target<TReturn(TObj::*)(TArgs...)>() == ptr) {

                auto index = it->index;
                this->eventList.erase(it);

                return index;
            }
        }
        return 0;
    }

    unsigned int RemoveListenerByIndex(unsigned int index) {
        if (index == -1) {
            return;
        }
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if (it->index == index) {
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
public:
    void Invoke(TArgs... t) {
        for (auto item : this->eventList) {
            item.func(t...);
        }
    }

    void RemoveAllListener() {
        this->eventList.clear();
    }

    template<typename TObj>
    void RemoveByInstance(TObj* obj) {
        for (auto it = this->eventList.begin(); it != this->eventList.end(); ) {
            if (it->instance == obj) {
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
            retList.push_back(item.func(args...));
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
            retList.push_back(item.func());
        }
        return retList;
    }

    bool IsValidReturnInvoke() {
        for (const bool& item : InvokeResult()) {
            if (!item) return false;
        }
        return true;
    }
};


#endif // !CORELIB_EVENTS_HPP