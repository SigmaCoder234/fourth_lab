#include <gtest/gtest.h>
#include "../src/DynamicArray.h"

TEST(DynamicArrayTest, Constructors) {
    DynamicArray<int> arr1(5);
    EXPECT_EQ(arr1.GetCount(), 5);
    
    int data[] = {1, 2, 3};
    DynamicArray<int> arr2(data, 3);
    EXPECT_EQ(arr2.GetCount(), 3);
    EXPECT_EQ(arr2.Get(0), 1);
    EXPECT_EQ(arr2.Get(2), 3);
}

TEST(DynamicArrayTest, Resize) {
    DynamicArray<int> arr(2);
    arr.Set(0, 10);
    arr.Set(1, 20);
    
    arr.Resize(4);
    EXPECT_EQ(arr.GetCount(), 4);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    arr.Resize(5);
    EXPECT_EQ(arr.GetCount(), 5);
    EXPECT_EQ(arr.GetCount(), 6);
}