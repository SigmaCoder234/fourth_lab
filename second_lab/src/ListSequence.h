#pragma once
#include "Sequence.h"
#include "LinkedList.h"

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* items;

    // Внутренние методы модификации (наследуются из Sequence)
    void AppendInternal(const T& item) override;
    void PrependInternal(const T& item) override;
    void InsertAtInternal(const T& item, int index) override;
    void SetInternal(int index, const T& item) override;
    void RemoveAtInternal(int index) override;

public:
    // --- Конструкторы ---
    ListSequence();
    ListSequence(const T* items_arr, int count);
    
    // --- Деструктор ---
    ~ListSequence() override;

    // --- Чтение (наследуются из Sequence) ---
    const T& Get(int index) const override;
    const T& GetFirst() const override;
    const T& GetLast() const override;
    int GetCount() const override;
};

#include "tpps/ListSequence.tpp"