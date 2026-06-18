#pragma once
#include <stdexcept>

template <class T>
class Option {
    T value;
    bool hasValue;
public:
    Option() : hasValue(false), value() {}
    Option(const T& v) : value(v), hasValue(true) {}
    
    bool HasValue() const { return hasValue; }
    const T& GetValue() const {
        if (!hasValue){
            throw std::out_of_range("Option is empty");
        }
        return value;
    }
};