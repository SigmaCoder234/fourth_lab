#include <gtest/gtest.h>
#include "../src/LinkedList.h"

TEST(LinkedListTest, AppendPrepend) {
    LinkedList<int> list;
    list.Append(2);
    list.Append(3);
    list.Prepend(1);
    
    EXPECT_EQ(list.GetCount(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTest, InsertRemove) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    list.InsertAt(2, 1);
    
    EXPECT_EQ(list.Get(1), 2);
    
    list.RemoveAt(1);
    EXPECT_EQ(list.Get(1), 3);
    EXPECT_EQ(list.GetCount(), 2);
}