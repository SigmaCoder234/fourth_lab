#pragma once

// Интерфейс компаратора
template <class T>
class IComparer {
    public:
    virtual ~IComparer() = default;
    // Возвращает true, если a меньше b
    virtual bool Compare(const T& a, const T& b) const = 0;
};

template <class T>
class GreaterComparer : public IComparer<T> {
    public:
    virtual bool Compare(const T& a, const T& b) const override {
        return a > b;
    }
};

template <class T>
class LessComparer : public IComparer<T> {
    public:
    virtual bool Compare(const T& a, const T& b) const override {
        return a < b;
    }
};