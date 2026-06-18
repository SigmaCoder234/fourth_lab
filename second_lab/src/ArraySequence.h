#pragma once
#include "Sequence.h"
#include "DynamicArray.h"

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

    // Внутренние методы модификации (наследуются из Sequence)
    void AppendInternal(const T& item) override;
    void PrependInternal(const T& item) override;
    void InsertAtInternal(const T& item, int index) override;
    void SetInternal(int index, const T& item) override;
    void RemoveAtInternal(int index) override;

public:
    // --- Конструкторы ---
    ArraySequence();
    ArraySequence(const T* items, int count);
    
    // --- Деструктор ---
    ~ArraySequence() override;

    // --- Чтение (наследуются из Sequence) ---
    const T& Get(int index) const override;
    const T& GetFirst() const override;
    const T& GetLast() const override;
    int GetCount() const override;
};

#include "tpps/ArraySequence.tpp"