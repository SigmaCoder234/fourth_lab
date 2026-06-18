#include <stdexcept>

template <class T>
DynamicArray<T>::DynamicArray(const T* items, int count) {
    if (count < 0) {
        throw std::invalid_argument("Size cannot be negative");
    }

    this -> size = count;

    if (size > 0) {
        capacity = size;
    }

    else capacity = 1;

    data = new T[capacity]{};
    for (int i = 0; i < size; i++) {
        data[i] = items[i];
    }
}

template<class T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) {
        throw std::invalid_argument("Size cannot be negative");
    }
    this->size = size;

    if (size > 0) {
        capacity = size;
    }

    else capacity = 1;

    data = new T[capacity]{};
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray& dynamic_array) {
    this->size = dynamic_array.size;
    capacity = dynamic_array.capacity;

    data = new T[capacity]{};
    for (int i = 0; i < size; i++)
        data[i] = dynamic_array.data[i];
}

template<class T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& dynamic_array) {
    if (this == &dynamic_array) {
        return *this;
    }
    
    delete[] data;
    
    this->size = dynamic_array.size;
    this->capacity = dynamic_array.capacity;
    
    data = new T[capacity]{};
    for (int i = 0; i < size; i++) {
        data[i] = dynamic_array.data[i];
    }
    
    return *this;
}

template<class T>
const T& DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<class T>
int DynamicArray<T>::GetCount() const {
    return size;
}

template<class T>
void DynamicArray<T>::Set(int index, const T& value) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }

    data[index] = value;
}

template<class T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0)
        throw std::invalid_argument("Size cannot be negative");

    if (newSize <= capacity) {
        size = newSize;
        return;
    }

    int newCapacity = capacity;

    while (newCapacity < newSize)
        newCapacity *= 2;

    T* newData = new T[newCapacity]{};

    int elementsToCopy = (size < newSize) ? size : newSize;
    for (int i = 0; i < elementsToCopy; i++)
        newData[i] = data[i];

    delete[] data;
    data = newData;
    size = newSize;
    capacity = newCapacity;
}