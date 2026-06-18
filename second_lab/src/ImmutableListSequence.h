#pragma once
#include "ListSequence.h"

template <class T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableListSequence<T>();
    }

public:
    ImmutableListSequence() : ListSequence<T>() {}
    
    ImmutableListSequence(const T* items, int count) 
        : ListSequence<T>(items, count) {}
        
    // конструктор копирования от базового класса
    ImmutableListSequence(const ListSequence<T>& other) : ListSequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
    
    // конструктор копирования точного типа
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
};