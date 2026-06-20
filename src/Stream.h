#pragma once
#include "Sequence.h"
#include "LazySequence.h"
#include "../second_lab/src/ICollection.h"
#include <string>

// поток только для чтения
template <class T>
class ReadOnlyStream {
public:
    virtual ~ReadOnlyStream() = default;

    // Считывает первый элемент потока и переходит на следующую позицию
    // Должен бросать исключение std::out_of_range, если достигнут конец потока.
    virtual T Read() = 0;

    // Возвращает признак того, что достигнут конец потока
    virtual bool IsEndOfStream() const = 0;

    // Сколько элементов было считано на данный момент
    virtual size_t GetPosition() const = 0;

};

// поток только для записи
template <class T>
class WriteOnlyStream {
public:
    virtual ~WriteOnlyStream() = default;

    // Записывает элемент в конец потока и возвращает новую позицию
    virtual size_t Write(const T& item) = 0;

    // Возвращает текущую позицию (сколько всего записано)
    virtual size_t GetPosition() const = 0;
};

template <class T>
class ReadOnlySequenceStream : public ReadOnlyStream<T> {
private:
    ICollection<T>* data;
    int currentIndex;

public:
    ReadOnlySequenceStream(ICollection<T>* sequence) : data(sequence), currentIndex(0) {}

    T Read() override {
        if (IsEndOfStream()) {
            throw std::out_of_range("End of stream reached");
        }
        // Возвращаем элемент и сразу сдвигаем currentIndex вперед
        return data->Get(currentIndex++);
    }

    bool IsEndOfStream() const override {
        // Поток закончен, когда currentIndex дошел до конца массива
        return currentIndex >= data->GetCount(); 
    }

    size_t GetPosition() const override {
        return currentIndex;
    }
};

template <class T>
class WriteOnlySequenceStream : public WriteOnlyStream<T> {
private:
    DynamicArray<T>* data;
    int currentIndex;

public:
    WriteOnlySequenceStream(DynamicArray<T>* array) : data(array), currentIndex(array->GetCount()) {}

    size_t Write(const T& item) override {
        data->Resize(currentIndex + 1);
        data->Set(currentIndex, item);
        currentIndex++;
        return currentIndex;
    }

    size_t GetPosition() const override {
        return currentIndex;
    }
};