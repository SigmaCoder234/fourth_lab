#pragma once
#include "Ordinal.h"

// Интерфейс для правила генерации элементов
template <class T>
class IGeneratorRule {
public:
    virtual ~IGeneratorRule() = default;
    
    // Генерирует элемент, находящийся на позиции index
    virtual T Generate(const Ordinal& index) = 0;

    // Метод для клонирования правила
    virtual IGeneratorRule<T>* Clone() const = 0;
};

// Forward declaration, так как Generator нужен в LazySequence
// Говорит, что класс LazySequence существует, но его определение будет позже
template <class T>
class LazySequence;

template <class T>
class Generator {
private:
    IGeneratorRule<T>* rule;
    Ordinal currentIndex;

public:
    // Конструктор: инициализирует индекс нулем (Ordinal(0))
    Generator(IGeneratorRule<T>* genRule) : rule(genRule), currentIndex(0) {}

    // Метод клонирования генератора
    Generator<T>* Clone() const {
        auto* clonedGen = new Generator<T>(rule->Clone());
        clonedGen->currentIndex = this->currentIndex;
        return clonedGen;
    }

    // Деструктор
    ~Generator() {
        if (rule) {
            delete rule;
        }
    }

    // Создает очередной элемент с помощью rule
    T GetNext() {
        // Генерируем элемент по текущему ординальному индексу
        T item = rule->Generate(currentIndex);
        
        // Увеличиваем индекс
        currentIndex = currentIndex + Ordinal(1);
        
        return item;
    }

    // Возвращает правило
    IGeneratorRule<T>* GetRule() const {
        return rule;
    }
};