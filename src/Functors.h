#pragma once

template <class TIn, class TOut>
class IMapFunction {
public:
    virtual ~IMapFunction() = default;
    virtual TOut Map(TIn item) = 0; // Метод, который превращает TIn в TOut
};


template<class T>
class IWhereFunction {
public:
    virtual ~IWhereFunction() = default;
    
    // Возвращает true, если элемент подходит, и false иначе.
    virtual bool Check(const T& item) = 0;
};

template<class T>
class IReduceFunction {
public:
    virtual ~IReduceFunction() = default;
    
    // Метод для свертки двух элементов в один (например, сложение).
    virtual T Combine(const T& acc, const T& item) = 0;
};