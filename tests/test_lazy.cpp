#include <gtest/gtest.h>
#include "LazySequence.h"
#include "Generator.h"
#include "Ordinal.h"

// Простой генератор константы
class ConstRule : public IGeneratorRule<int> {
private:
    int val;
public:
    ConstRule(int v) : val(v) {}
    int Generate(const Ordinal& index) override {
        return val;
    }
    IGeneratorRule<int>* Clone() const override {
        return new ConstRule(val);
    }
};

// Генератор арифметической прогрессии
class TestArithmeticProgressionRule : public IGeneratorRule<int> {
private:
    int start;
    int step;
public:
    TestArithmeticProgressionRule(int s, int st) : start(s), step(st) {}
    int Generate(const Ordinal& index) override {
        return start + step * index.GetFinite();
    }
    IGeneratorRule<int>* Clone() const override {
        return new TestArithmeticProgressionRule(start, step);
    }
};

TEST(LazySequenceTest, Concatenation) {
    auto* b1 = new TestArithmeticProgressionRule(1, 1);
    auto* b2 = new TestArithmeticProgressionRule(1, -1);
    auto* b3 = new ConstRule(42);

    // Создаем три бесконечные последовательности
    LazySequence<int> seq1(b1);
    LazySequence<int> seq2(b2);
    LazySequence<int> seq3(b3);

    // Concat возвращает новый указатель на LazySequence
    LazySequence<int>* result_seq = seq1.Concat(&seq2)->Concat(&seq3);

    // Омега 0: относится к первой бесконечности (seq1). Старт 1, шаг 1. Элемент с индексом 5: 1 + 1*5 = 6.
    EXPECT_EQ(result_seq->Get(Ordinal(0, 5)), 6);
    
    // Омега 1: относится ко второй бесконечности (seq2). Старт 1, шаг -1. Элемент с индексом 5: 1 - 1*5 = -4.
    EXPECT_EQ(result_seq->Get(Ordinal(1, 5)), -4);
    
    // Омега 2: относится к третьей бесконечности (seq3). Всегда 42.
    EXPECT_EQ(result_seq->Get(Ordinal(2, 5)), 42);

    delete result_seq;
}

// Генератор для проверки Reduce и GetSubsequence
class SumReducerTest : public IReduceFunction<int> {
public:
    int Combine(const int& acc, const int& item) override { return acc + item; }
};

TEST(LazySequenceTest, ReduceAndSubsequenceTest) {
    auto* b1 = new TestArithmeticProgressionRule(1, 1); // 1, 2, 3, 4, 5...
    LazySequence<int> seq(b1);
    
    // Проверка Reduce: сумма первых 4 элементов (1 + 2 + 3 + 4 = 10)
    SumReducerTest reducer;
    int sum = seq.Reduce(&reducer, 0, 4);
    EXPECT_EQ(sum, 10);
    
    // Проверка исключения для отрицательного числа элементов
    EXPECT_THROW(seq.Reduce(&reducer, 0, -1), std::invalid_argument);
    
    // Проверка GetSubsequence (элементы со 2-го по 4-й: это 3, 4, 5)
    LazySequence<int>* subSeq = seq.GetSubsequence(2, 4);
    EXPECT_EQ(subSeq->Get(Ordinal(0, 0)), 3);
    EXPECT_EQ(subSeq->Get(Ordinal(0, 1)), 4);
    EXPECT_EQ(subSeq->Get(Ordinal(0, 2)), 5);
    
    // Ожидается ошибка выхода за пределы конечной длины подпоследовательности (длина = 3)
    EXPECT_THROW(subSeq->Get(Ordinal(0, 3)), std::out_of_range);
    
    delete subSeq;
}

// Тест на мемоизацию кэша
// Специальное правило, считающее количество вызовов метода Generate
class CallCountingRule : public IGeneratorRule<int> {
private:
    int* counter;
public:
    CallCountingRule(int* c) : counter(c) {}
    int Generate(const Ordinal& index) override {
        (*counter)++;
        return index.GetFinite() * 10;
    }
    IGeneratorRule<int>* Clone() const override {
        return new CallCountingRule(counter);
    }
};

TEST(LazySequenceTest, MemoizationCacheTest) {
    int calls = 0;
    LazySequence<int> seq(new CallCountingRule(&calls));
    
    EXPECT_EQ(calls, 0);
    
    // Первое обращение к элементу 0 вызывает генератор (0..0)
    EXPECT_EQ(seq.Get(Ordinal(0, 0)), 0);
    EXPECT_EQ(calls, 1);
    
    // Повторные обращения не должны увеличивать счетчик (данные берутся из кэша)
    EXPECT_EQ(seq.Get(Ordinal(0, 0)), 0);
    EXPECT_EQ(seq.Get(Ordinal(0, 0)), 0);
    EXPECT_EQ(calls, 1); // Счетчик остался 1
    
    // Обращение к 3-му элементу вынудит вычислить 1, 2 и 3
    EXPECT_EQ(seq.Get(Ordinal(0, 3)), 30);
    EXPECT_EQ(calls, 4); // Вычислены элементы с индексами 0, 1, 2, 3
    
    // Проверка счетчика материализованных элементов
    EXPECT_EQ(seq.GetMaterializedCount(), 4);
}

// Тест на корректный выброс исключения
TEST(LazySequenceTest, OutOfBoundsFiniteTest) {
    // Конечное правило из 3 элементов (например, 0, 10, 20)
    class FiniteRule : public IGeneratorRule<int> {
    public:
        int Generate(const Ordinal& index) override {
            return index.GetFinite() * 10;
        }
        IGeneratorRule<int>* Clone() const override { return new FiniteRule(); }
    };

    LazySequence<int> seq(new FiniteRule(), Ordinal(0, 3));

    EXPECT_EQ(seq.Get(Ordinal(0, 0)), 0);
    EXPECT_EQ(seq.Get(Ordinal(0, 1)), 10);
    EXPECT_EQ(seq.Get(Ordinal(0, 2)), 20);
    
    // Попытка обратиться к элементу за пределами длины (индекс 3 при длине 3)
    EXPECT_THROW(seq.Get(Ordinal(0, 3)), std::out_of_range);
}

// Тест на Map (умножение на 2)
class MultiplyByTwo : public IMapFunction<int, int> {
public:
    int Map(int item) override { return item * 2; }
};

TEST(LazySequenceTest, MapTest) {
    auto* b1 = new TestArithmeticProgressionRule(1, 1); // 1, 2, 3, ...
    LazySequence<int> seq(b1);
    MultiplyByTwo mapper;
    LazySequence<int>* mapped = seq.Map(&mapper);
    
    EXPECT_EQ(mapped->Get(Ordinal(0, 0)), 2);
    EXPECT_EQ(mapped->Get(Ordinal(0, 4)), 10);
    delete mapped;
}

// Тест на Where (четные числа)
class IsEven : public IWhereFunction<int> {
public:
    bool Check(const int& item) override { return item % 2 == 0; }
};

TEST(LazySequenceTest, WhereTest) {
    auto* b1 = new TestArithmeticProgressionRule(1, 1); // 1, 2, 3, 4, 5, 6...
    LazySequence<int> seq(b1);
    IsEven filter;
    LazySequence<int>* filtered = seq.Where(&filter);
    
    EXPECT_EQ(filtered->Get(Ordinal(0, 0)), 2);
    EXPECT_EQ(filtered->Get(Ordinal(0, 1)), 4);
    EXPECT_EQ(filtered->Get(Ordinal(0, 2)), 6);
    delete filtered;
}

// Тест на Insert
TEST(LazySequenceTest, InsertTest) {
    auto* b1 = new TestArithmeticProgressionRule(1, 1); // 1, 2, 3...
    LazySequence<int> seq1(b1);
    
    class FiniteRule100 : public IGeneratorRule<int> {
    public:
        int Generate(const Ordinal& index) override { return 100; }
        IGeneratorRule<int>* Clone() const override { return new FiniteRule100(); }
    };
    LazySequence<int> seq2(new FiniteRule100(), Ordinal(0, 2)); // Два элемента по 100
    
    // Вставляем seq2 в seq1 на позицию 2
    LazySequence<int>* inserted = seq1.Insert(&seq2, Ordinal(0, 2));
    
    // Ожидаемые элементы: 1, 2, 100, 100, 3, 4...
    EXPECT_EQ(inserted->Get(Ordinal(0, 0)), 1);
    EXPECT_EQ(inserted->Get(Ordinal(0, 1)), 2);
    EXPECT_EQ(inserted->Get(Ordinal(0, 2)), 100);
    EXPECT_EQ(inserted->Get(Ordinal(0, 3)), 100);
    EXPECT_EQ(inserted->Get(Ordinal(0, 4)), 3);
    EXPECT_EQ(inserted->Get(Ordinal(0, 5)), 4);

    delete inserted;
}