#pragma once
#include <stdexcept>
#include "IEnumerator.h"
#include "ICollection.h"

template <class T>
class DynamicArray : public ICollection<T>{
private:
    T* data;
    int size;
    int capacity; //перенести в ArrSeq

    class ArrayEnumerator : public IEnumerator<T> {
    private:
        const DynamicArray<T>* array; // Указатель на массив, который мы перебираем
        int current_index;            // Текущая позиция "закладки"

    public:
        // Конструктор итератора
        ArrayEnumerator(const DynamicArray<T>* arr) {
            this->array = arr;
            this->current_index = -1; // Начинаем ДО первого элемента
        }

        bool MoveNext() override {
            current_index++;
            return current_index < array -> GetCount();
        }

        const T& Current() const override {
            return array -> Get(current_index);
        }

        void Reset() override {
            this -> current_index = -1;
        }
    };

public:

    // constructors
    DynamicArray(const T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray& dynamic_array);
    DynamicArray<T>& operator=(const DynamicArray<T>& dynamic_array);

    // destructor
    ~DynamicArray();

    // decomposers
    const T& Get(int index) const override;
    int GetCount() const override;

    // operations
    void Set(int index, const T& value);
    void Resize(int newSize);
    
    // Iterator
    IEnumerator<T>* GetEnumerator() const override { return new ArrayEnumerator(this); }
};

#include "tpps/DynamicArray.tpp"    