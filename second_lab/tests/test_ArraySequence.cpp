#include <gtest/gtest.h>
#include "../src/MutableArraySequence.h"
#include "../src/ImmutableArraySequence.h"

int multiplyByTwo(const int& x) { return x * 2; }
bool isEven(const int& x) { return x % 2 == 0; }
int sum(const int& a, const int& b) { return a + b; }

TEST(ArraySequenceTest, MutableOperations) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    
    Sequence<int>* res = seq.Append(4);
    // Mutable должен менять сам себя и возвращать this
    EXPECT_EQ(res, &seq);
    EXPECT_EQ(seq.GetCount(), 4);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(ArraySequenceTest, ImmutableOperations) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);
    
    Sequence<int>* res = seq.Append(4);
    // Immutable должен возвращать новый объект, а старый не меняется
    EXPECT_NE(res, &seq);
    EXPECT_EQ(seq.GetCount(), 3);
    EXPECT_EQ(res->GetCount(), 4);
    EXPECT_EQ(res->Get(3), 4);
    delete res;
}

TEST(ArraySequenceTest, MapReduceWhere_Mutable) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);
    
    Sequence<int>* res = seq.Map(multiplyByTwo);
    EXPECT_EQ(res, &seq);
    // Теперь массив {2, 4, 6, 8}
    EXPECT_EQ(seq.GetCount(), 4);
    EXPECT_EQ(seq.Get(0), 2);
    EXPECT_EQ(seq.Get(3), 8);
    
    res = seq.Where(isEven);
    EXPECT_EQ(res, &seq);
    // Все {2, 4, 6, 8} четные, ничего не удалится
    EXPECT_EQ(seq.GetCount(), 4);
    
    int total = seq.Reduce(sum, 0);
    EXPECT_EQ(total, 20); // 2+4+6+8
}

TEST(ArraySequenceTest, MapReduceWhere_Immutable) {
    int data[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(data, 4);
    
    Sequence<int>* mapped = seq.Map(multiplyByTwo);
    EXPECT_NE(mapped, &seq);
    EXPECT_EQ(seq.GetCount(), 4);
    EXPECT_EQ(seq.Get(3), 4); // Старый не изменился
    EXPECT_EQ(mapped->Get(3), 8); // Новый изменился
    
    Sequence<int>* filtered = seq.Where(isEven);
    EXPECT_NE(filtered, &seq);
    EXPECT_EQ(filtered->GetCount(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);
    
    int total = seq.Reduce(sum, 0);
    EXPECT_EQ(total, 10);
    
    delete mapped;
    delete filtered;
}