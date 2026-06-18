#include <stdexcept>

// --- Конструкторы и Деструктор ---
template <class T>
ArraySequence<T>::ArraySequence() {
    items = new DynamicArray<T>(0);
}

template <class T>
ArraySequence<T>::ArraySequence(const T* items_arr, int count) {
    items = new DynamicArray<T>(items_arr, count);
}

template <class T>
ArraySequence<T>::~ArraySequence() {
    delete items;
}

// --- Чтение ---
template <class T>
const T& ArraySequence<T>::Get(int index) const {
    return items->Get(index);
}

template <class T>
const T& ArraySequence<T>::GetFirst() const {
    return items->Get(0);
}

template <class T>
const T& ArraySequence<T>::GetLast() const {
    return items->Get(items->GetCount() - 1);
}

template <class T>
int ArraySequence<T>::GetCount() const {
    return items->GetCount();
}

// --- Внутренние методы модификации ---
template <class T>
void ArraySequence<T>::SetInternal(int index, const T& item) {
    items->Set(index, item);
}

template <class T>
void ArraySequence<T>::AppendInternal(const T& item) {
    int newSize = items->GetCount() + 1;
    items->Resize(newSize);
    items->Set(newSize - 1, item); // Ставим новый элемент в конец
}

template <class T>
void ArraySequence<T>::PrependInternal(const T& item) {
    InsertAtInternal(item, 0); // Используем InsertAtInternal для вставки на 0 позицию
}

template <class T>
void ArraySequence<T>::InsertAtInternal(const T& item, int index) {
    if (index < 0 || index > items->GetCount()) {
        throw std::out_of_range("Index out of range");
    }
    
    int size = items->GetCount();
    items->Resize(size + 1);
    
    // Сдвигаем элементы вправо, начиная с конца
    for (int i = size; i > index; --i) {
        items->Set(i, items->Get(i - 1));
    }
    
    items->Set(index, item);
}

template <class T>
void ArraySequence<T>::RemoveAtInternal(int index) {
    if (index < 0 || index >= items->GetCount()) {
        throw std::out_of_range("Index out of range");
    }

    int size = items->GetCount();
    
    // Сдвигаем элементы влево, затирая элемент на позиции index
    for (int i = index; i < size - 1; ++i) {
        items->Set(i, items->Get(i + 1));
    }
    
    // Уменьшаем размер массива
    items->Resize(size - 1);
}