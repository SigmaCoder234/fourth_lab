#include <gtest/gtest.h>
#include "OnlineStatistics.h"
#include "../second_lab/src/DynamicArray.h"
#include "../second_lab/src/MutableArraySequence.h"
#include "../second_lab/src/ArraySequence.h"

TEST(OnlineStatisticsTest, BasicMetrics) {
    OnlineStatistics<int> stats;
    
    EXPECT_THROW(stats.GetMean(), std::logic_error);
    EXPECT_THROW(stats.GetMedian(), std::logic_error);

    stats.Consume(10);
    EXPECT_EQ(stats.GetCount(), 1);
    EXPECT_EQ(stats.GetMin(), 10);
    EXPECT_EQ(stats.GetMax(), 10);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 10.0);
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 10.0);

    stats.Consume(20);
    stats.Consume(30);
    stats.Consume(40);
    stats.Consume(50);

    EXPECT_EQ(stats.GetCount(), 5);
    EXPECT_EQ(stats.GetMin(), 10);
    EXPECT_EQ(stats.GetMax(), 50);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 30.0); // 150 / 5 = 30
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 30.0); // middle of 10, 20, 30, 40, 50

    stats.Consume(60);
    
    EXPECT_EQ(stats.GetCount(), 6);
    EXPECT_EQ(stats.GetMin(), 10);
    EXPECT_EQ(stats.GetMax(), 60);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 35.0); // 210 / 6 = 35
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 35.0); // (30 + 40) / 2 = 35
}

TEST(OnlineStatisticsTest, ConsumeStreamTest) {
    int data[] = {15, 5, 25, 10, 20};
    MutableArraySequence<int> arr(data, 5);

    ReadOnlySequenceStream<int> stream(&arr);
    OnlineStatistics<int> stats;

    stats.ConsumeStream(&stream);

    EXPECT_EQ(stats.GetCount(), 5);
    EXPECT_EQ(stats.GetMin(), 5);
    EXPECT_EQ(stats.GetMax(), 25);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 15.0); // 75 / 5 = 15
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 15.0); // sorted: 5, 10, 15, 20, 25 -> median 15
}

TEST(OnlineStatisticsTest, SortedInputBalancingAndNegativeNumbers) {
    OnlineStatistics<int> stats;
    
    // Добавляем строго отсортированные данные от 1 до 100
    // Это заставит балансировщик (BalanceHeaps) перекидывать элементы между кучами
    for (int i = 1; i <= 100; ++i) {
        stats.Consume(i);
    }
    
    EXPECT_EQ(stats.GetCount(), 100);
    EXPECT_EQ(stats.GetMin(), 1);
    EXPECT_EQ(stats.GetMax(), 100);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 50.5); // Сумма 5050 / 100
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 50.5); // Медиана между 50 и 51
    
    // Проверка с отрицательными числами
    OnlineStatistics<int> negStats;
    negStats.Consume(-10);
    negStats.Consume(-20);
    negStats.Consume(-5);
    
    EXPECT_EQ(negStats.GetMin(), -20);
    EXPECT_EQ(negStats.GetMax(), -5);
    EXPECT_DOUBLE_EQ(negStats.GetMedian(), -10.0);
}