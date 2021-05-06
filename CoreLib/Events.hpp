#pragma once
#include <vector>
#include <functional>
#include <cstdint>

template<typename TReturn, typename ...T>
class Events
{
public:
    using FunctionType = std::function<TReturn(T...)>;
    using FunctionPointer = TReturn(*)(T...);
protected:
    class FunctionInfo
    {
    public:
        uint32_t index;
        FunctionType func;
        intptr_t staticFuncPtr;
    public:
        FunctionInfo() : index(0), func(nullptr), staticFuncPtr((intptr_t)0)
        {
        }
        FunctionInfo(
            const uint32_t& index,
            const FunctionType& func,
            const intptr_t& staticFuncPtr
        ) : index(index), func(func), staticFuncPtr(staticFuncPtr)
        {
        }
    public:
        bool operator ==(const FunctionInfo& r) {
            return this->index == r.index;
        }
    };

protected:
    uint32_t index;
    std::vector<FunctionInfo> eventList;
public:
    int Count() const {
        return this->eventList.size();
    }
public:
    Events() : index(0) {}
protected:
    uint32_t Push(const FunctionType& func, const intptr_t& staticFuncPtr = 0) {
        FunctionInfo f{ ++this->index, func, staticFuncPtr };
        this->eventList.push_back(f);
        return this->index;
    }
public:
    uint32_t AddListener(FunctionPointer funcPtr) {
        if ((intptr_t)funcPtr == 0) {
            return -1;
        }
        FunctionType func{ funcPtr };
        this->Push(func, (intptr_t)funcPtr);
        return this->index;
    }

    template<typename TOBJ>
    int AddInstListener(TOBJ* obj, TReturn(TOBJ::* ptr)(T...)) {
        return this->Push(std::bind(ptr, obj, T...));
    }

    void RemoveListener(FunctionPointer funcPtr) {
        if ((intptr_t)funcPtr == 0) {
            return;
        }
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if (it->staticFuncPtr == (intptr_t)funcPtr) {
                this->eventList.erase(it);
                return;
            }
        }
    }

    void RemoveListenerByIndex(uint32_t index) {
        if (index == -1) {
            return;
        }
        for (auto it = this->eventList.begin(); it != this->eventList.end(); it++) {
            if (it->index == index) {
                this->eventList.erase(it);
                break;
            }
        }
    }

    void Invoke(const T&... t) {
        for (auto item : this->eventList) {
            item.func(t...);
        }
    }

    void RemoveAllListener() {
        this->eventList.clear();
    }

};

template<typename TReturn, typename... TParams>
class Function : public Events<TReturn, TParams...>
{
public:
    std::vector<TReturn> Invoke(TParams&... args) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(args...));
        }
        return retList;
    }
};

template<typename... TParam>
using Action = Events<void, TParam...>;