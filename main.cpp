#include <iostream>

template<typename T>
class MyUniquePtr {
private:
    T* ptr;

public:
    MyUniquePtr(T* p) : ptr(p) {}

    ~MyUniquePtr() {
        delete ptr;
    }

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    
    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }
};

template<typename T>
class MySharedPtr {
private:
    T* ptr;
    size_t* count;

public:
    MySharedPtr(T* p) : ptr(p), count(new size_t(1)) {}

    ~MySharedPtr() {
        if (--(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) {
        (*count)++;
    }

    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            if (--(*count) == 0) {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            (*count)++;
        }
        return *this;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    size_t use_count() const {
        return *count;
    }
};


int main() {
    
    MyUniquePtr<int> uniquePtr(new int(5));
    std::cout << *uniquePtr << std::endl;

    
    MySharedPtr<int> sharedPtr1(new int(10));
    MySharedPtr<int> sharedPtr2 = sharedPtr1;
    std::cout << *sharedPtr1 << " " << *sharedPtr2 << std::endl;
    std::cout << "Use count: " << sharedPtr1.use_count() << std::endl;

    return 0;
}
