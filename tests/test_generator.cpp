#include <gtest/gtest.h>
#include "Generator.h"

class SimpleRule : public IGeneratorRule<int> {
public:
    int Generate(const Ordinal& index) override {
        return index.GetFinite() * 10;
    }
    IGeneratorRule<int>* Clone() const override { return new SimpleRule(); }
};

TEST(GeneratorTest, BasicOperations) {
    Generator<int> gen(new SimpleRule());
    
    // Индекс начинается с 0, поэтому 0 * 10 = 0
    EXPECT_EQ(gen.GetNext(), 0);
    // Индекс увеличился до 1, 1 * 10 = 10
    EXPECT_EQ(gen.GetNext(), 10);
    // Индекс увеличился до 2, 2 * 10 = 20
    EXPECT_EQ(gen.GetNext(), 20);
}

TEST(GeneratorTest, CloneOperation) {
    Generator<int> gen(new SimpleRule());
    gen.GetNext(); // 0
    gen.GetNext(); // 10
    
    // Клонируем генератор (должен скопировать состояние индекса, который сейчас 2)
    Generator<int>* cloned = gen.Clone();
    
    EXPECT_EQ(cloned->GetNext(), 20);
    EXPECT_EQ(gen.GetNext(), 20); // Оригинальный тоже должен вернуть 20
    
    delete cloned;
}
