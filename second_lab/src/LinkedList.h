#pragma once
#include <stdexcept>
#include "ICollection.h"

template <class T>
class LinkedList : public ICollection<T> {
private:
    // Внутренняя структура узла списка
    struct Node {
        T data;
        Node* next;
        
        explicit Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head; // Указатель на начало списка
    Node* tail; // Указатель на конец списка (для быстрого Append)
    int size;   // Текущее количество элементов

    // очистка списка
    void Clear() {
        Node* cur = head;
        while (cur) {
            Node* n = cur->next;
            delete cur;
            cur = n;
        }
        head = tail = nullptr;
        size = 0;
    }

    class ListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<T>* list;
        Node* current; // текущий узел (nullptr = позиция до начала)
        bool started;

    public:
        ListEnumerator(const LinkedList<T>* lst){
            list = lst;
            current = nullptr;
            started = false;
        }

        bool MoveNext() override {
            if (!started) {
                // Первый вызов - переход к голове
                current = list->head;
                started = true;
            } 
            
            else if (current != nullptr){
                current = current->next;
            }
            return current != nullptr;
        }

        const T& Current() const override {
            if ( current == nullptr )
                throw std::out_of_range("No current element in enumerator");
            return current->data;
        }

        void Reset() override {
            current = nullptr;
            started = false;
        }
    };

public:
    // --- Конструкторы и Деструктор ---
    
    // Создать пустой список
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    // Копировать элементы из переданного массива
    LinkedList(const T* items, int count);
    
    // Копирующий конструктор
    LinkedList(const LinkedList<T>& other);
    
    LinkedList<T>& operator=(const LinkedList<T>& other);

    // Деструктор
    ~LinkedList() override { Clear(); }

    // --- Декомпозиция ---
    const T& GetFirst() const;
    const T& GetLast() const;
    const T& Get(int index) const override;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetCount() const override { return size; }

    // --- Операции (Модификация) ---
    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, int index);
    void RemoveAt(int index);
    void Set(int index, const T& item);
    
    // Сцепляет два списка и возвращает новый
    LinkedList<T>* Concat(const LinkedList<T>* other) const;

    // --- Итератор ---
    IEnumerator<T>* GetEnumerator() const override { return new ListEnumerator(this); }
};

#include "tpps/LinkedList.tpp"