#include <gtest/gtest.h>
#include "Ordinal.h"

// Проверка операторов сравнения
TEST(OrdinalTest, ComparisonOperators) {
    Ordinal a(0, 5);
    Ordinal b(0, 10);
    Ordinal c(1, 0); // Омега
    Ordinal d(1, 5); // Омега + 5

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c); // Любое конечное число меньше бесконечности
    EXPECT_TRUE(c < d);
    EXPECT_TRUE(d > a);
    EXPECT_TRUE(c >= Ordinal(1, 0));
    EXPECT_TRUE(a == Ordinal(0, 5));
    EXPECT_TRUE(a != b);
}

// Проверка арифметики (сложение и свойство поглощения)
// По правилам: (w*A + B) + (w*C + D) = w*(A+C) + D
TEST(OrdinalTest, AdditionAndAbsorption) {
    Ordinal a(1, 5); // w + 5
    Ordinal b(2, 3); // w*2 + 3
    Ordinal c(0, 10); // 10

    // Прибавление конечного числа
    Ordinal sum1 = a + c; // (w + 5) + 10 = w + 15
    EXPECT_EQ(sum1.GetOmega(), 1);
    EXPECT_EQ(sum1.GetFinite(), 15);

    // Прибавление бесконечности (поглощение конечной части левого операнда)
    Ordinal sum2 = a + b; // (w + 5) + (w*2 + 3) = w*3 + 3
    EXPECT_EQ(sum2.GetOmega(), 3);
    EXPECT_EQ(sum2.GetFinite(), 3);
}

// Проверка вычитания и генерации исключения
TEST(OrdinalTest, Subtraction) {
    Ordinal a(3, 10);
    Ordinal b(1, 5);
    
    // (w*3 + 10) - (w*1 + 5) = w*2 + 10
    Ordinal diff1 = a - b;
    EXPECT_EQ(diff1.GetOmega(), 2);
    EXPECT_EQ(diff1.GetFinite(), 10);

    // Вычитание в пределах одной бесконечности
    Ordinal c(3, 20);
    Ordinal diff2 = c - a; // (w*3 + 20) - (w*3 + 10) = 10
    EXPECT_EQ(diff2.GetOmega(), 0);
    EXPECT_EQ(diff2.GetFinite(), 10);

    // Отрицательный результат должен вызывать исключение
    EXPECT_THROW(b - a, std::out_of_range);
}