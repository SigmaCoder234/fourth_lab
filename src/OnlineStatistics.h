#pragma once
#include "Stream.h"
#include "PriorityQueue.h"
#include "IComparer.h"

template <class T>
class OnlineStatistics {
private:
    T minVal;
    T maxVal;
    double sum;
    int count;

    LessComparer<T> lessComp;
    GreaterComparer<T> greaterComp;

    PriorityQueue<T> lowerHalf; // Max-Heap (хранит меньшую половину чисел)
    PriorityQueue<T> upperHalf; // Min-Heap (хранит большую половину чисел)

    // Разница в размерах куч не должна превышать 1
    void BalanceHeaps() {
        // Если левая куча стала слишком большой
        if (lowerHalf.GetSize() > upperHalf.GetSize() + 1) {
            T item = lowerHalf.Pop(); // забираем вершину
            upperHalf.Push(item);     // перекидываем в правую
        }
        // Если правая куча стала слишком большой
        else if (upperHalf.GetSize() > lowerHalf.GetSize() + 1) {
            T item = upperHalf.Pop(); // забираем вершину
            lowerHalf.Push(item);     // перекидываем в левую
        }
    }

public:
    // Конструктор
    OnlineStatistics() : count(0), sum(0), lowerHalf(&greaterComp), upperHalf(&lessComp) {}
        // Инициализируем базовые значения
        // Инициализируем кучи, передавая им адреса компараторов

    // Деструктор
    ~OnlineStatistics() = default;

    // Метод, который забирает элемент из потока и обновляет все метрики
    void Consume(const T& item) {
        // Обновляем базовые переменные
        if (count == 0) {
            minVal = maxVal = item; // Если это первый элемент
        } else {
            if (item < minVal) minVal = item;
            if (item > maxVal) maxVal = item;
        }
        sum += item;
        count++;

        // Распределяем элемент по кучам
        // Если левая пуста или элемент меньше её вершины - кидаем в левую
        if (lowerHalf.IsEmpty() || item <= lowerHalf.Peek()) {
            lowerHalf.Push(item);
        } else {
            // Иначе кидаем в правую
            upperHalf.Push(item);
        }

        // Восстанавливаем баланс
        BalanceHeaps();
    }

    // Вывод состояния обеих куч
    void PrintHeaps() const {
        std::cout << "Lower Half (Max-Heap): ";
        lowerHalf.Print();
        std::cout << "\nUpper Half (Min-Heap): ";
        upperHalf.Print();
        std::cout << "\n";
    }

    // Метод для чтения всего потока целиком, пока он не закончится
    void ConsumeStream(ReadOnlyStream<T>* stream) {
        while (!stream->IsEndOfStream()) {
            Consume(stream->Read());
        }
    }

    // Геттеры метрик (выполняются за O(1))
    T GetMin() const {return minVal;}
    T GetMax() const {return maxVal;}
    int GetCount() const {return count;}
    double GetMean() const { 
        if (count == 0) {
            throw std::logic_error("No elements to calculate mean!");
        }
        return (double)sum / count;
    }

    // Если элементов четное количество - среднее арифметическое вершин обеих куч
    // Если нечетное - вершина той кучи, где элементов больше
    double GetMedian() const {
        if (count == 0) {
            throw std::logic_error("No elements to calculate median!");
        }
        if (count % 2 == 0) {
            return (double)(lowerHalf.Peek() + upperHalf.Peek()) / 2.0;
        } else if (lowerHalf.GetSize() > upperHalf.GetSize()){
            return (double)lowerHalf.Peek();
        } else{
            return (double)upperHalf.Peek();
        }
    }
};