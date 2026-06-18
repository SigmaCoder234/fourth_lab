#include <gtest/gtest.h>
#include "OnlineStatistics.h"
#include "../second_lab/src/DynamicArray.h"

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
    DynamicArray<int> arr(5);
    arr.Set(0, 15);
    arr.Set(1, 5);
    arr.Set(2, 25);
    arr.Set(3, 10);
    arr.Set(4, 20);

    ReadOnlySequenceStream<int> stream(&arr);
    OnlineStatistics<int> stats;

    stats.ConsumeStream(&stream);

    EXPECT_EQ(stats.GetCount(), 5);
    EXPECT_EQ(stats.GetMin(), 5);
    EXPECT_EQ(stats.GetMax(), 25);
    EXPECT_DOUBLE_EQ(stats.GetMean(), 15.0); // 75 / 5 = 15
    EXPECT_DOUBLE_EQ(stats.GetMedian(), 15.0); // sorted: 5, 10, 15, 20, 25 -> median 15
}