#pragma once
#include "ArraySequence.h"

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableArraySequence<T>();
    }

public:
    MutableArraySequence() : ArraySequence<T>() {}
    
    MutableArraySequence(const T* items, int count) 
        : ArraySequence<T>(items, count) {}
        
    MutableArraySequence(const ArraySequence<T>& other) : ArraySequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i)); 
        }
    }
    
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i)); 
        }
    }
};