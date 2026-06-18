#pragma once
#include "Option.h"

template <class T>
class Sequence {
protected:
    // Возвращает this (для Mutable) или копию (для Immutable)
    virtual Sequence<T>* Instance() = 0;

    // Создает новый пустой экземпляр такого же типа
    virtual Sequence<T>* CreateEmpty() const = 0;

    // Внутренние методы модификации — реализуются в ArraySequence/ListSequence
    virtual void AppendInternal(const T& item) = 0;
    virtual void PrependInternal(const T& item) = 0;
    virtual void InsertAtInternal(const T& item, int index) = 0;

    virtual void SetInternal(int index, const T& item) = 0;
    virtual void RemoveAtInternal(int index) = 0;

public:
    virtual ~Sequence() = default;

    // --- Чтение ---
    virtual const T& Get(int index) const = 0;
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual int GetCount() const = 0;
    
    Sequence<T>* GetSubSequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= GetCount() || startIndex > endIndex) {
            throw std::out_of_range("Invalid indices for subsequence");
        }
        Sequence<T>* result = CreateEmpty();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(Get(i));
        }
        return result;
    }

    // --- Try-методы (используют Option) ---
    Option<T> TryGet(int index) const {
        if (index < 0 || index >= GetCount())
            return Option<T>();         // пустой
        return Option<T>(Get(index));   // со значением
    }

    Option<T> TryGetFirst() const {
        if (GetCount() == 0)
            return Option<T>();
        return Option<T>(GetFirst());
    }

    Option<T> TryGetLast() const {
        if (GetCount() == 0)
            return Option<T>();
        return Option<T>(GetLast());
    }

    // --- Модификация ---
    Sequence<T>* Append(const T& item) {
        Sequence<T>* result = Instance();
        result->AppendInternal(item);
        return result;
    }

    Sequence<T>* Concat(const Sequence<T>& list) {
        Sequence<T>* result = Instance();
        for (int i = 0; i < list.GetCount(); ++i) {
            result->AppendInternal(list.Get(i));
        }
        return result;
    }

    Sequence<T>* Prepend(const T& item) {
        Sequence<T>* result = Instance();
        result->PrependInternal(item);
        return result;
    }

    Sequence<T>* InsertAt(const T& item, int index) {
        Sequence<T>* result = Instance();
        result->InsertAtInternal(item, index);
        return result;
    }

    Sequence<T>* Set(int index, const T& item) {
        Sequence<T>* result = Instance();
        result->SetInternal(index, item);
        return result;
    }

    Sequence<T>* RemoveAt(int index) {
        Sequence<T>* result = Instance();
        result->RemoveAtInternal(index);
        return result;
    }

    Sequence<T>* Map(T (*mapper)(const T&)){
        Sequence<T>* result = Instance(); // создаем копию (или получаем this)
        for (int i = 0; i < result->GetCount(); ++i) {
            // Берем старый элемент
            T oldEl = result->Get(i);

            // Вычисляем новый с помощью переданной функции mapper
            T newEl = mapper(oldEl);

            // Записываем обратно
            result->SetInternal(i, newEl);
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) {
        Sequence<T>* result = Instance();
        for (int i = 0; i < result->GetCount(); ++i) {
            T oldEl = result->Get(i);

            bool condition = predicate(oldEl);

            if (!condition) {
                result->RemoveAtInternal(i);
                i--;
            }
        }
        return result;
    }

    T Reduce(T (*reducer)(const T&, const T&), const T& startValue) const {
        auto result = startValue;
        
        // Проходимся по всем элементам
        for (int i = 0; i < GetCount(); ++i) {
            // Берем текущий result и следующий элемент
            // Передаем их в reducer и обновляем result
            result = reducer(result, Get(i));
        }
        
        return result;
    }
};