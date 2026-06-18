#pragma once
#include "IEnumerable.h"

template <class T>
class ICollection : public IEnumerable<T> {
public:
    virtual ~ICollection() = default;
    
    // Получить элемент по индексу
    virtual const T& Get(int index) const = 0;
    
    // Получить размер коллекции
    virtual int GetCount() const = 0;
};