#pragma once
#include "ListSequence.h"

template <class T>
class MutableListSequence : public ListSequence<T> {
protected:
    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableListSequence<T>();
    }

public:
    MutableListSequence() : ListSequence<T>() {}
    
    MutableListSequence(const T* items, int count) 
        : ListSequence<T>(items, count) {}
        
    MutableListSequence(const ListSequence<T>& other) : ListSequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
    
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>() {
        for (int i = 0; i < other.GetCount(); ++i) {
            this->AppendInternal(other.Get(i));
        }
    }
};