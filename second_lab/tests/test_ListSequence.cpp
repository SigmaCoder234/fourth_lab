#include <gtest/gtest.h>
#include "../src/MutableListSequence.h"
#include "../src/ImmutableListSequence.h"

TEST(ListSequenceTest, MutableOperations) {
    int data[] = {10, 20};
    MutableListSequence<int> seq(data, 2);
    
    Sequence<int>* res = seq.Prepend(0);
    EXPECT_EQ(res, &seq);
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(seq.Get(0), 0);
}

TEST(ListSequenceTest, ImmutableOperations) {
    int data[] = {10, 20};
    ImmutableListSequence<int> seq(data, 2);
    
    Sequence<int>* res = seq.Prepend(0);
    EXPECT_NE(res, &seq);
    EXPECT_EQ(seq.GetCount(), 2);
    EXPECT_EQ(res->GetCount(), 3);
    EXPECT_EQ(res->Get(0), 0);
    delete res;
}