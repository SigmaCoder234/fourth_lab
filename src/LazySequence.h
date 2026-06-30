#pragma once
#include "Sequence.h"
#include "DynamicArray.h" // Используем как кэш
#include "Generator.h"
#include "Functors.h"

template <class T>
class LazySequence {
private:
    // Двумерный кэш: первый индекс = номер бесконечности (Омега), второй = конечный индекс
    DynamicArray<DynamicArray<T>*> cache;

    // Указатель на генератор
    Generator<T>* generator;

    // Длина последовательности
    Ordinal length;

public:
    // пустой ленивый список с заданным правилом генерации (по умолчанию длина = 1 бесконечность)
    LazySequence(IGeneratorRule<T>* rule) : cache(0), length(Ordinal::Omega()) {
        generator = new Generator<T>(rule);
    }
    
    // конструктор с указанием длины
    LazySequence(IGeneratorRule<T>* rule, Ordinal len) : cache(0), length(len) {
        generator = new Generator<T>(rule);
    }

    // Копирующий конструктор
    LazySequence(const LazySequence<T>& other) : cache(0), length(other.length) {
        generator = other.generator->Clone();
        cache.Resize(other.cache.GetCount());
        for (int i = 0; i < cache.GetCount(); ++i) {
            auto* inner = other.cache.Get(i);
            cache.Set(i, new DynamicArray<T>(*inner)); // Глубокое копирование
        }
    }

    //оператор присваивания для копирующего конструктора
    LazySequence<T>& operator=(const LazySequence<T>& other) {
        if (this == &other) return *this;
        if (generator) delete generator;
        for (int i = 0; i < cache.GetCount(); ++i) {
            delete cache.Get(i);
        }
        cache.Resize(0);
        
        length = other.length;
        generator = other.generator->Clone();
        cache.Resize(other.cache.GetCount());
        for (int i = 0; i < cache.GetCount(); ++i) {
            auto* inner = other.cache.Get(i);
            cache.Set(i, new DynamicArray<T>(*inner));
        }
        return *this;
    }

    // Деструктор
    ~LazySequence() {
        if (generator) delete generator;
        for (int i = 0; i < cache.GetCount(); ++i) {
            delete cache.Get(i);
        }
    }

    // Получить первый элемент
    T GetFirst() {return Get(0);}

    // Получить элемент по индексу
    T Get(int index) {
        if (index < 0) throw std::out_of_range("Index cannot be negative");
        return Get(Ordinal(index));
    }
    
    // Получить элемент по ординалу
    T Get(const Ordinal& index) {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds for LazySequence");
        }

        // 1. Убеждаемся, что внешний массив кэша (для Омег) достаточно велик
        while (cache.GetCount() <= index.GetOmega()) {
            int oldSize = cache.GetCount();
            cache.Resize(oldSize + 1);
            cache.Set(oldSize, new DynamicArray<T>(0));
        }

        DynamicArray<T>* inner = cache.Get(index.GetOmega());
        
        // 2. Довычисляем недостающие элементы последовательно
        while (inner->GetCount() <= index.GetFinite()) {
            int curI = inner->GetCount();
            Ordinal curIndex(index.GetOmega(), curI);
            
            T item = generator->GetRule()->Generate(curIndex);
            
            inner->Resize(curI + 1);
            inner->Set(curI, item);
        }

        return inner->Get(index.GetFinite());
    }

    // Получить подпоследовательность (материализует нужный кусок, если он еще не материализован)
    LazySequence<T>* GetSubsequence(int startIndex, int endIndex);

    // Склеить с другой последовательностью
    LazySequence<T>* Concat(LazySequence<T>* other);

    // Вставить другую последовательность по индексу
    LazySequence<T>* Insert(LazySequence<T>* other, const Ordinal& pos);

    // Длина последовательности
    Ordinal GetLength() const { return length; }

    // Количество материализованных элементов
    size_t GetMaterializedCount() const {
        size_t count = 0;
        for (int i = 0; i < cache.GetCount(); ++i) {
            count += cache.Get(i)->GetCount();
        }
        return count;
    }

    //map
    template <class TOut>
    LazySequence<TOut>* Map(IMapFunction<T, TOut>* mapper);

    //where
    LazySequence<T>* Where(IWhereFunction<T>* whereFunc);

    //reduce
    T Reduce(IReduceFunction<T>* reduceFunc, T initialValue, int countToReduce) {
        if (countToReduce < 0) 
            throw std::invalid_argument("Count cannot be negative");
        
        T result = initialValue;
        for (int i = 0; i < countToReduce; ++i) {
            T item = Get(i);
            result = reduceFunc->Combine(result, item); 
        }
        return result;
    }
};

// ----------- ПРАВИЛА ДЛЯ ЛЕНИВЫХ СПИСКОВ -----------
// Подпоследовательность
template <class T>
class SubSequenceRule : public IGeneratorRule<T> {
private:
    LazySequence<T>* source;
    Ordinal current;
    Ordinal length;

public:
    SubSequenceRule(LazySequence<T>* src, const Ordinal& start, const Ordinal& len) 
        : source(src), current(start), length(len) {}

    IGeneratorRule<T>* Clone() const override {
        return new SubSequenceRule<T>(source, current, length); 
    }

    T Generate(const Ordinal& index) override {
        if (index >= length) {
            throw std::out_of_range("Generator has no more elements!");
        }
        return source->Get(current + index);
    }
};

// Map
template <class T, class TOut>
class MapRule : public IGeneratorRule<TOut> {
private:
    LazySequence<T>* source;
    IMapFunction<T, TOut>* mapper;

public:
    MapRule(LazySequence<T>* src, IMapFunction<T, TOut>* func) : source(src), mapper(func) {}

    IGeneratorRule<TOut>* Clone() const override {
        return new MapRule<T, TOut>(source, mapper);
    }

    TOut Generate(const Ordinal& index) override {
        return mapper->Map(source->Get(index));
    }
};

// Where
template <class T>
class WhereRule : public IGeneratorRule<T> {
private:
    LazySequence<T>* source;
    IWhereFunction<T>* filter;
    Ordinal currentIndex; // Индекс в исходном потоке
    
public:
    WhereRule(LazySequence<T>* src, IWhereFunction<T>* func) : source(src), filter(func), currentIndex(0) {}

    IGeneratorRule<T>* Clone() const override {
        auto* cloned = new WhereRule<T>(source, filter);
        cloned->currentIndex = this->currentIndex; // копируем состояние
        return cloned;
    }

    T Generate(const Ordinal& index) override {
        while (true) {
            T item = source->Get(currentIndex);
            currentIndex = currentIndex + Ordinal(1);
            if (filter->Check(item)) {
                return item;
            }
        }
    }
};

// Concat
template <class T>
class ConcatRule : public IGeneratorRule<T> {
private:
    LazySequence<T>* A;
    LazySequence<T>* B;
public:
    ConcatRule(LazySequence<T>* seq1, LazySequence<T>* seq2) : A(seq1), B(seq2) {}

    IGeneratorRule<T>* Clone() const override {
        return new ConcatRule<T>(A, B);
    }

    T Generate(const Ordinal& index) override {
        if (index < A->GetLength()) {
            return A->Get(index);
        }
        return B->Get(index - A->GetLength());
    }
};

// Insert
template <class T>
class InsertRule : public IGeneratorRule<T> {
private:
    LazySequence<T>* A;
    LazySequence<T>* B;
    Ordinal pos;
public:
    InsertRule(LazySequence<T>* seq1, LazySequence<T>* seq2, const Ordinal& position) 
        : A(seq1), B(seq2), pos(position) {}

    IGeneratorRule<T>* Clone() const override {
        return new InsertRule<T>(A, B, pos);
    }

    T Generate(const Ordinal& index) override {
        if (index < pos) {
            return A->Get(index);
        }
        Ordinal bLength = B->GetLength();
        if (index < pos + bLength) {
            return B->Get(index - pos);
        }
        // Хвост последовательности A
        return A->Get(pos + (index - (pos + bLength)));
    }
};

// ----------- Реализация -----------

// Получить подпоследовательность
template <class T>
LazySequence<T>* LazySequence<T>::GetSubsequence(int startIndex, int endIndex) {
    if (startIndex > endIndex || startIndex < 0) {
        throw std::out_of_range("Invalid subsequence indices");
    }
    auto* rule = new SubSequenceRule<T>(this, Ordinal(startIndex), Ordinal(endIndex - startIndex + 1));
    return new LazySequence<T>(rule, Ordinal(endIndex - startIndex + 1));
}

template <class T>
LazySequence<T>* LazySequence<T>::Concat(LazySequence<T>* other) {
    auto* rule = new ConcatRule<T>(this, other);
    return new LazySequence<T>(rule, this->length + other->GetLength());
}

template <class T>
LazySequence<T>* LazySequence<T>::Insert(LazySequence<T>* other, const Ordinal& pos) {
    if (pos > this->length) throw std::out_of_range("Insert position out of bounds");
    auto* rule = new InsertRule<T>(this, other, pos);
    return new LazySequence<T>(rule, this->length + other->GetLength());
}

// --- ОПЕРАЦИИ (Map, Where) ---
template <class T>
template <class TOut>
LazySequence<TOut>* LazySequence<T>::Map(IMapFunction<T, TOut>* mapper){
    auto* rule = new MapRule<T, TOut>(this, mapper);
    return new LazySequence<TOut>(rule);
}

template <class T>
LazySequence<T>* LazySequence<T>::Where(IWhereFunction<T>* whereFunc) {
    auto* rule = new WhereRule<T>(this, whereFunc);
    return new LazySequence<T>(rule);
}