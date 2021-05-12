#ifndef CORELIB_PROPERTY_H
#define CORELIB_PROPERTY_H
#include <functional>

template<typename T>
class Property
{
private:
    std::function<T& ()> get;
    std::function<void(const T& value)> set;
public:
    Property(const std::function<T& ()>& get, const std::function<void(const T& value)>& set)
        : get(get), set(set)
    {
    }
    T& operator()() {
        return this->get();
    }
    void operator=(const T& v) {
        this->set(v);
    }
    operator T() {
        return this->get();
    }
};

#define PROP_GET(T) [this]()->T& 
#define PROP_SET(T) [this](const T& value) 


#endif // !CORELIB_PROPERTY_H
