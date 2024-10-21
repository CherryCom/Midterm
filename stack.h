#ifndef STACK_H
#define STACK_H

#include <vector>

template <typename T>
class Stack {
private:
    std::vector<T> data;

public:

    Stack() {}


    bool empty() const {
        return data.empty();
    }

 
    size_t size() const {
        return data.size();
    }

 
    void push(const T& value) {
        data.push_back(value);
    }

   
    void pop() {
        if (!empty()) {
            data.pop_back();
        }
    }

    T& top() {
        if (!empty()) {
            return data.back();
        }
        throw std::runtime_error("Stack is empty");
    }

    
    const T& top() const {
        if (!empty()) {
            return data.back();
        }

        throw std::runtime_error("Stack is empty");
    }
};

#endif
