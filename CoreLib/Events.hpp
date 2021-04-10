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
        FunctionInfo f(++this->index, func, staticFuncPtr);
        this->eventList.push_back(f);
        return this->index;
    }
public:
    uint32_t AddListener(FunctionPointer funcPtr) {
        if ((intptr_t)funcPtr == 0) {
            return -1;
        }
        FunctionType func(funcPtr);
        this->Push(func, (intptr_t)funcPtr);
        return this->index;
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

    void Invoke(T... t) {
        for (auto item : this->eventList) {
            item.func(t...);
        }
    }
    void RemoveAllListener() {
        this->eventList.clear();
    }

};

template<typename TReturn>
class _Events0 : public Events<TReturn>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)()) {
        return this->Push(std::bind(funcPtr, obj));
    }
};

template<typename TReturn, typename T1>
class _Events1 : public Events<TReturn, T1>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)(T1)) {
        return this->Push(std::bind(funcPtr, obj,
            std::placeholders::_1
        ));
    }
};

template<typename TReturn, typename T1, typename T2>
class _Events2 : public Events<TReturn, T1, T2>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)(T1, T2)) {
        return this->Push(std::bind(funcPtr, obj,
            std::placeholders::_1,
            std::placeholders::_2
        ));
    }
};

template<typename TReturn, typename T1, typename T2, typename T3>
class _Events3 : public Events<TReturn, T1, T2, T3>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)(T1, T2, T3)) {
        return this->Push(std::bind(funcPtr, obj,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
        ));
    }
};

template<typename TReturn, typename T1, typename T2, typename T3, typename T4>
class _Events4 : public Events<TReturn, T1, T2, T3, T4>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)(T1, T2, T3, T4)) {
        return this->Push(std::bind(funcPtr, obj,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4
        ));
    }
};

template<typename TReturn, typename T1, typename T2, typename T3, typename T4, typename T5>
class _Events5 : public Events<TReturn, T1, T2, T3, T4, T5>
{
public:
    template<typename Tobj>
    uint32_t AddInstListener(Tobj* obj, TReturn(Tobj::* funcPtr)(T1, T2, T3, T4, T5)) {
        return this->Push(std::bind(funcPtr, obj,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5
        ));
    }
};

template<typename TReturn>
class Function0 : public _Events0<TReturn>
{
public:
    std::vector<TReturn> Invoke() {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func());
        }
        return retList;
    }
};

template<typename TReturn, typename T1>
class Function1 : public _Events1<TReturn, T1>
{
public:
    std::vector<TReturn> Invoke(T1 t1) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(t1));
        }
        return retList;
    }
};

template<typename TReturn, typename T1, typename T2>
class Function2 : _Events2<TReturn, T1, T2>
{
public:
    std::vector<TReturn> Invoke(T1 t1, T2 t2) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(t1, t2));
        }
        return retList;
    }
};

template<typename TReturn, typename T1, typename T2, typename T3>
class Function3 : public _Events3<TReturn, T1, T2, T3>
{
public:
    std::vector<TReturn> Invoke(T1 t1, T2 t2, T3 t3) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(t1, t2, t3));
        }
        return retList;
    }
};

template<typename TReturn, typename T1, typename T2, typename T3, typename T4>
class Function4 : public _Events4<TReturn, T1, T2, T3, T4>
{
public:
    std::vector<TReturn> Invoke(T1 t1, T2 t2, T3 t3, T4 t4) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(t1, t2, t3, t4));
        }
        return retList;
    }
};

template<typename TReturn, typename T1, typename T2, typename T3, typename T4, typename T5>
class Function5 : public _Events5<TReturn, T1, T2, T3, T4, T5>
{
public:
    std::vector<TReturn> Invoke(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) {
        std::vector<TReturn> retList;
        for (auto item : this->eventList) {
            retList.push_back(item.func(t1, t2, t3, t4, t5));
        }
        return retList;
    }
};

using Action0 = _Events0<void>;

template<typename T1>
using Action1 = _Events1<void T1>;

template<typename T1, typename T2>
using Action2 = _Events2<void, T1, T2>;

template<typename T1, typename T2, typename T3>
using Action3 = _Events3<void, T1, T2, T3>;

template<typename T1, typename T2, typename T3, typename T4>
using Action4 = _Events4<void, T1, T2, T3, T4>;

template<typename T1, typename T2, typename T3, typename T4, typename T5>
using Action5 = _Events5<void, T1, T2, T3, T4, T5>;
