#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
struct Node {
    T data;
    Node* next;

    Node(T value) : data(value), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void enqueue(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        Node<T>* temp = front;
        T value = front->data;
        front = front->next;
        delete temp;
        size--;
        if (isEmpty()) {
            rear = nullptr;
        }
        return value;
    }

    T getFront() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return front->data;
    }
};

#endif
