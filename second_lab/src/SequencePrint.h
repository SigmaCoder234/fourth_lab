#pragma once
#include "Sequence.h"
#include <iostream>

// Перегрузка оператора << для вывода любой Sequence
template <class T>
std::ostream& operator<<(std::ostream& os, const Sequence<T>& seq) {
    os << "[";
    for (int i = 0; i < seq.GetCount(); ++i) {
        os << seq.Get(i);
        if (i < seq.GetCount() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template <class T>
void PrintSequence(const Sequence<T>& seq) {
    std::cout << seq << std::endl;
}