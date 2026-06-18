#include <stdexcept>

template <class T>
LinkedList<T>::LinkedList(const T* items, int count) : head(nullptr), tail(nullptr), size(0) {
    if (count < 0)
        throw std::invalid_argument("Count cannot be negative");

    for (int i = 0; i < count; ++i)
        Append(items[i]);
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), size(0) {
    Node* cur = other.head;
    while (cur){
        Append(cur->data);
        cur = cur->next;
    }
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other){
    if (this == &other) return *this;
    Clear();

    Node* cur = other.head;
    while (cur){
        Append(cur->data);
        cur = cur->next;
    }

    return *this;
}

template<class T>
const T& LinkedList<T>::GetFirst() const {
        if (head == nullptr)
            throw std::out_of_range("List is empty");
        return head->data;
    }

template <class T>
const T& LinkedList<T>::GetLast() const {
    if (tail == nullptr)
        throw std::out_of_range("List is empty");
    return tail->data;
}

template <class T>
const T& LinkedList<T>::Get(int index) const{
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    
        Node* cur = head;
        for (int i = 0; i < index; ++i)
            cur = cur->next;
        return cur->data;
}

template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw std::out_of_range("Invalid indexes for sublist");

    LinkedList<T>* result = new LinkedList<T>();
    Node* cur = head;

    for (int i = 0; i < startIndex; ++i)
        cur = cur->next;

    for (int i = startIndex; i < endIndex; ++i){
        result->Append(cur->data);
        cur = cur->next;
    }

    return result;
}

template <class T>
void LinkedList<T>::Append(const T& item) {
    Node* newNode = new Node(item);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <class T>
void LinkedList<T>::Prepend(const T& item) {
    Node* newNode = new Node(item);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template <class T>
void LinkedList<T>::InsertAt(const T& item, int index) {
    if (index < 0 || index > size) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        Prepend(item);
    } else if (index == size) {
        Append(item);
    } else {
        Node* newNode = new Node(item);
        Node* cur = head;
        for (int i = 0; i < index - 1; ++i) {
            cur = cur->next;
        }
        newNode->next = cur->next;
        cur->next = newNode;
        size++;
    }
}

template <class T>
void LinkedList<T>::RemoveAt(int index) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    
    Node* temp = head;
    
    if (index == 0) { // Удаление из начала
        head = head->next;
        if (!head) tail = nullptr; // Если список стал пустым
        delete temp;
    } else { // Удаление из середины или конца
        Node* prev = nullptr;
        // Идем до элемента перед удаляемым
        for (int i = 0; i < index; ++i) {
            prev = temp;
            temp = temp->next;
        }
        
        // temp указывает на удаляемый элемент
        prev->next = temp->next;
        if (temp == tail) tail = prev; // Если удалили последний, обновляем tail
        delete temp;
    }
    size--;
}

template <class T>
void LinkedList<T>::Set(int index, const T& item) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
    
    Node* cur = head;
    for (int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    cur->data = item;
}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* other) const {
    LinkedList<T>* result = new LinkedList<T>(*this);
    if (other != nullptr) {
        Node* cur = other->head;
        while (cur != nullptr) {
            result->Append(cur->data);
            cur = cur->next;
        }
    }
    return result;
}