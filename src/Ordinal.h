#pragma once
#include <cstddef>
#include <stdexcept>

class Ordinal {
private:
    std::size_t omega;  // Сколько "бесконечностей" мы прошли
    std::size_t finite; // Конечный индекс внутри текущей бесконечности

public:
    // Конструктор по умолчанию (индекс 0)
    Ordinal() : omega(0), finite(0) {}
    
    // Конструктор из обычного числа
    explicit Ordinal(std::size_t f) : omega(0), finite(f) {}
    
    // Полный конструктор: (Омега, конечная часть)
    Ordinal(std::size_t o, std::size_t f) : omega(o), finite(f) {}

    // Быстрый способ получить ровно Омегу (переход на вторую бесконечность)
    static Ordinal Omega() {
        return Ordinal(1, 0);
    }

    // Геттеры
    std::size_t GetOmega() const { return omega; }
    std::size_t GetFinite() const { return finite; }
    bool IsInfinite() const { return omega > 0; }

    // --- Операторы сравнения ---
    bool operator==(const Ordinal& other) const {
        return omega == other.omega && finite == other.finite;
    }
    
    bool operator!=(const Ordinal& other) const {
        return !(*this == other);
    }

    bool operator<(const Ordinal& other) const {
        if (omega != other.omega) {
            return omega < other.omega;
        }
        return finite < other.finite; // Если Омеги равны, сравниваем конечные части
    }

    bool operator>(const Ordinal& other) const {
        return other < *this;
    }

    bool operator<=(const Ordinal& other) const {
        return !(*this > other);
    }

    bool operator>=(const Ordinal& other) const {
        return !(*this < other);
    }

    // --- Сложение ординалов ---
    // (w*A + B) + (w*C + D)
    Ordinal operator+(const Ordinal& other) const {
        if (other.omega == 0) {
            // Если прибавляем конечное число: (w*A + B) + D = w*A + (B+D)
            return Ordinal(this->omega, this->finite + other.finite);
        } else {
            // Свойство поглощения: (w*A + B) + (w*C + D) = w*(A+C) + D
            // Конечное число B растворяется в бесконечности C
            return Ordinal(this->omega + other.omega, other.finite);
        }
    }

    // --- Вычитание ординалов ---
    // (w*A + B) - (w*C + D)
    Ordinal operator-(const Ordinal& other) const {
        if (*this < other) {
            throw std::out_of_range("Result of ordinal subtraction cannot be negative");
        }
        if (this->omega > other.omega) {
            // По правилам ординальной арифметики:
            // Если A > C, то (w*C + D) + (w*(A-C) + B) = w*A + B
            // Значит, разность равна w*(A-C) + B
            return Ordinal(this->omega - other.omega, this->finite);
        } else {
            // Если omega == other.omega
            // Вычитание происходит в рамках одной бесконечности
            return Ordinal(0, this->finite - other.finite);
        }
    }
};