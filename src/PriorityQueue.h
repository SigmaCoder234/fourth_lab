#pragma once
#include "DynamicArray.h"
#include "IComparer.h"
#include <stdexcept>
#include <functional>

template <class T>
class PriorityQueue {
private:
    DynamicArray<T> data;
    
    // Указатель на объект-компаратор.
    // Если это Max-Heap, компаратор должен возвращать a > b
    // Если это Min-Heap, компаратор должен возвращать a < b
    IComparer<T>* comparer;

    void SiftUp(int index) {
        if (index == 0) return; // Дошли до конца
        
        int parentIndex = (index - 1) / 2;
        
        // Если текущий элемент приоритетнее родителя - меняем местами и рекурсивно идем дальше
        if (comparer->Compare(data.Get(index), data.Get(parentIndex))) {
            T temp = data.Get(index); 
            data.Set(index, data.Get(parentIndex)); 
            data.Set(parentIndex, temp);
            
            SiftUp(parentIndex); // Рекурсивный вызов
        }
    }


    void SiftDown(int index){
        // Вычисляем индексы левого и правого потомков
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;
        int smallestIndex = index;

        // Если левый потомок существует и он приоритетнее текущего самого приоритетного
        if(leftChildIndex < data.GetCount() && comparer->Compare(data.Get(leftChildIndex), data.Get(smallestIndex))){
            smallestIndex = leftChildIndex; // Запоминаем его индекс
        }
        // Если правый потомок существует и он приоритетнее того, кого мы выбрали шагом выше
        if(rightChildIndex < data.GetCount() && comparer->Compare(data.Get(rightChildIndex), data.Get(smallestIndex))){
            smallestIndex = rightChildIndex; // Запоминаем правого
        }

        // Если оказалось, что один из потомков приоритетнее родителя
        if(smallestIndex != index){
            T temp = data.Get(index); 
            data.Set(index, data.Get(smallestIndex)); 
            data.Set(smallestIndex, temp);
            
            SiftDown(smallestIndex);
        }
    }

public:
    // Конструктор
    PriorityQueue(IComparer<T>* comp) : data(0), comparer(comp) {}

    // Вставка элемента в кучу O(log N)
    void Push(const T& item) {
        int currentSize = data.GetCount();
        // Увеличиваем размер массива на 1
        data.Resize(currentSize + 1);
        // Кладем элемент на последнее место
        data.Set(currentSize, item);
        // поднимаем его
        SiftUp(currentSize);
}

    // вывод кучи
    void Print() const {
        std::cout << "[ ";
        for (int i = 0; i < data.GetCount(); i++) {
            std::cout << data.Get(i) << " ";
        }
        std::cout << "]";
    }

    // Удаление и возврат вершины кучи O(log N)
    T Pop() {
    if (IsEmpty()) {
        throw std::out_of_range("Pop from empty PriorityQueue!");
    }
    
    // Запоминаем корень
    T root = data.Get(0);
    
    // Перемещаем последний элемент в корень (на место удаляемого)
    int lastIndex = data.GetCount() - 1;
    data.Set(0, data.Get(lastIndex));
    
    // Уменьшаем размер массива (отрезаем последний элемент)
    data.Resize(lastIndex);
    
    // Если куча не стала пустой, опускаем новый корень вниз
    if (!IsEmpty()) {
        SiftDown(0);
    }
    
    return root;
}

    // Возврат вершины кучи без удаления O(1)
    T Peek() const {
        if (IsEmpty()) {
            throw std::out_of_range("Peek from empty PriorityQueue!");
        }
        return data.Get(0);
    }

    // Количество элементов
    int GetSize() const {
        return data.GetCount();
    }

    // Проверка на пустоту
    bool IsEmpty() const {
        return data.GetCount() == 0;
    }
};