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

// Forward declaration, так как Generator тесно связан с LazySequence
// Для ссылки на LazySequence без его импорта (избавиться от рекурсивного включения)
// Говорит, что класс LazySequence существует, но его определение будет позже
template <class T>
class LazySequence;

template <class T>
class Generator {
private:
    LazySequence<T>* owner;
    IGeneratorRule<T>* rule;
    Ordinal currentIndex;

public:
    // Конструктор: инициализирует индекс нулем (Ordinal(0))
    Generator(LazySequence<T>* ownerSeq, IGeneratorRule<T>* genRule) : owner(ownerSeq), rule(genRule), currentIndex(0) {}

    // Метод клонирования генератора
    Generator<T>* Clone(LazySequence<T>* newOwner) const {
        auto* clonedGen = new Generator<T>(newOwner, rule->Clone());
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

    // Возвращает true, если можно породить следующий элемент
    bool HasNext() const {
        return true; // Пока считаем, что все генераторы бесконечны
    }

    // Возвращает правило
    IGeneratorRule<T>* GetRule() const {
        return rule;
    }
};