#pragma once

template <class T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;
    virtual bool MoveNext() = 0;   // Шагнуть на следующий элемент. Возвращает true, если элемент есть
    virtual const T& Current() const = 0; // Получить текущий элемент
    virtual void Reset() = 0;      // Сбросить в начало
};