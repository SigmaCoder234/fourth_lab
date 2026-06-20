#include <gtest/gtest.h>
#include "Stream.h"
#include "../second_lab/src/DynamicArray.h"

TEST(StreamsTest, ReadOnlyReadOnlySequenceStreamTest) {
    DynamicArray<int> arr(3);
    arr.Set(0, 5);
    arr.Set(1, 15);
    arr.Set(2, 25);
    
    ReadOnlySequenceStream<int> stream(&arr);
    
    EXPECT_EQ(stream.GetPosition(), 0);
    EXPECT_FALSE(stream.IsEndOfStream());
    
    EXPECT_EQ(stream.Read(), 5);
    EXPECT_EQ(stream.GetPosition(), 1);
    
    EXPECT_EQ(stream.Read(), 15);
    EXPECT_EQ(stream.Read(), 25);
    
    EXPECT_TRUE(stream.IsEndOfStream());
    EXPECT_THROW(stream.Read(), std::out_of_range);
}

TEST(StreamsTest, WriteOnlySequenceStreamTest) {
    DynamicArray<int> arr(0);
    WriteOnlySequenceStream<int> stream(&arr);
    
    EXPECT_EQ(stream.GetPosition(), 0);
    
    size_t pos1 = stream.Write(100);
    EXPECT_EQ(pos1, 1);
    EXPECT_EQ(stream.GetPosition(), 1);
    
    size_t pos2 = stream.Write(200);
    EXPECT_EQ(pos2, 2);
    EXPECT_EQ(stream.GetPosition(), 2);
    
    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(0), 100);
    EXPECT_EQ(arr.Get(1), 200);
}