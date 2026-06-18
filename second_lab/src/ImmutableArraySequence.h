#pragma once
#include "ArraySequence.h"

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    
    ImmutableArraySequence(const T* items, int count) 
        : ArraySequence<T>(items, count) {}
        
    // конструктор копирования от базового класса
    ImmutableArraySequence(const ArraySequence<T>& other) : ArraySequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
    
    // конструктор копирования точного типа
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
};