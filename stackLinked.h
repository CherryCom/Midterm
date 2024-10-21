#ifndef STACKLINKED_H
#define STACKLINKED_H

#include <iostream>
#include <stdexcept>

template <typename T>
class StackLinked {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* topNode;  // Pointer to the top node
    size_t stackSize;  // Size of the stack

public:
    // Constructor
    StackLinked() : topNode(nullptr), stackSize(0) {}

    // Destructor to free memory
    ~StackLinked() {
        while (!empty()) {
            pop();
        }
    }

    // Check if the stack is empty
    bool empty() const {
        return stackSize == 0;
    }

    // Get the size of the stack
    size_t size() const {
        return stackSize;
    }

    // Push a new value onto the stack
    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;  // Link the new node to the former top
        topNode = newNode;  // Update the top to the new node
        ++stackSize;  // Increase the size
    }

    // Pop the top value off the stack
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = topNode;  // Temporary node to hold the top node
        topNode = topNode->next;  // Move the top to the next node
        delete temp;  // Free the memory of the old top node
        --stackSize;  // Decrease the size
    }

    // Get the top value of the stack
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;  // Return the data of the top node
    }

    // Get the top value of the stack (const version)
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;  // Return the data of the top node
    }
};

#endif
