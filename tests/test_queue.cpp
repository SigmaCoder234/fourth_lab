#include <gtest/gtest.h>
#include "PriorityQueue.h"

class IntMaxComparer : public IComparer<int> {
public:
    bool Compare(const int& a, const int& b) const override {
        return a > b; // Max-Heap
    }
};

class IntMinComparer : public IComparer<int> {
public:
    bool Compare(const int& a, const int& b) const override {
        return a < b; // Min-Heap
    }
};

TEST(PriorityQueueTest, MaxHeapTest) {
    IntMaxComparer comp;
    PriorityQueue<int> pq(&comp);
    
    EXPECT_TRUE(pq.IsEmpty());
    EXPECT_EQ(pq.GetSize(), 0);
    
    pq.Push(10);
    pq.Push(30);
    pq.Push(20);
    
    EXPECT_EQ(pq.GetSize(), 3);
    EXPECT_FALSE(pq.IsEmpty());
    
    // Максимальный элемент должен быть на вершине
    EXPECT_EQ(pq.Peek(), 30);
    EXPECT_EQ(pq.Pop(), 30);
    
    EXPECT_EQ(pq.Pop(), 20);
    EXPECT_EQ(pq.Pop(), 10);
    
    EXPECT_TRUE(pq.IsEmpty());
    EXPECT_THROW(pq.Pop(), std::out_of_range);
}

TEST(PriorityQueueTest, MinHeapTest) {
    IntMinComparer comp;
    PriorityQueue<int> pq(&comp);
    
    pq.Push(50);
    pq.Push(10);
    pq.Push(40);
    pq.Push(20);
    
    EXPECT_EQ(pq.Peek(), 10);
    EXPECT_EQ(pq.Pop(), 10);
    EXPECT_EQ(pq.Pop(), 20);
    EXPECT_EQ(pq.Pop(), 40);
    EXPECT_EQ(pq.Pop(), 50);
}