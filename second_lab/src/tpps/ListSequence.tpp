#include <stdexcept>

// --- Конструкторы и Деструктор ---
template <class T>
ListSequence<T>::ListSequence() {
    items = new LinkedList<T>();
}

template <class T>
ListSequence<T>::ListSequence(const T* items_arr, int count) {
    if (count < 0) {
        throw std::invalid_argument("ListSequence: size cannot be negative");
    }
    items = new LinkedList<T>(items_arr, count);
}

template <class T>
ListSequence<T>::~ListSequence() {
    delete items;
}

// --- Чтение ---
template <class T>
const T& ListSequence<T>::Get(int index) const {
    return items->Get(index);
}

template <class T>
const T& ListSequence<T>::GetFirst() const {
    return items->GetFirst();
}

template <class T>
const T& ListSequence<T>::GetLast() const {
    return items->GetLast();
}

template <class T>
int ListSequence<T>::GetCount() const {
    return items->GetCount();
}

// --- Внутренние методы модификации ---
template <class T>
void ListSequence<T>::SetInternal(int index, const T& item) {
    items->Set(index, item);
}

template <class T>
void ListSequence<T>::AppendInternal(const T& item) {
    items->Append(item);
}

template <class T>
void ListSequence<T>::PrependInternal(const T& item) {
    items->Prepend(item);
}

template <class T>
void ListSequence<T>::InsertAtInternal(const T& item, int index) {
    items->InsertAt(item, index);
}

template <class T>
void ListSequence<T>::RemoveAtInternal(int index) {
    items->RemoveAt(index);
}