#pragma once

#include <exception>
#include <iostream>
#include <initializer_list>
#include <stdexcept>

namespace bmstu {
template <typename T>
class simple_basic_string {
public:
    simple_basic_string() : ptr_(new T[1]{0}), size_(0) {}

    simple_basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {
        for (size_t i = 0; i < size; ++i) ptr_[i] = static_cast<T>(' ');
        ptr_[size] = 0;
    }

    simple_basic_string(std::initializer_list<T> il) : ptr_(new T[il.size() + 1]), size_(il.size()) {
        size_t i = 0;
        for (auto it = il.begin(); it != il.end(); ++it) ptr_[i++] = *it;
        ptr_[size_] = 0;
    }

    simple_basic_string(const T* c_str) {
        size_ = strlen_(c_str);
        ptr_ = new T[size_ + 1];
        for (size_t i = 0; i <= size_; ++i) ptr_[i] = c_str[i];
    }

    simple_basic_string(const simple_basic_string& other) : ptr_(new T[other.size_ + 1]), size_(other.size_) {
        for (size_t i = 0; i <= size_; ++i) ptr_[i] = other.ptr_[i];
    }

    simple_basic_string(simple_basic_string&& dying) noexcept : ptr_(dying.ptr_), size_(dying.size_) {
        dying.ptr_ = new T[1]{0};
        dying.size_ = 0;
    }

    ~simple_basic_string() {
        delete[] ptr_;
    }

    const T* c_str() const { return ptr_; }

    size_t size() const { return size_; }

    simple_basic_string& operator=(simple_basic_string&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = new T[1]{0};
            other.size_ = 0;
        }
        return *this;
    }

    simple_basic_string& operator=(const T* c_str) {
        delete[] ptr_;
        size_ = strlen_(c_str);
        ptr_ = new T[size_ + 1];
        for (size_t i = 0; i <= size_; ++i) ptr_[i] = c_str[i];
        return *this;
    }

    simple_basic_string& operator=(const simple_basic_string& other) {
        if (this != &other) {
            T* new_ptr = new T[other.size_ + 1];
            for (size_t i = 0; i <= other.size_; ++i) new_ptr[i] = other.ptr_[i];
            delete[] ptr_;
            ptr_ = new_ptr;
            size_ = other.size_;
        }
        return *this;
    }

    friend simple_basic_string<T> operator+(const simple_basic_string<T>& left, const simple_basic_string<T>& right) {
        simple_basic_string<T> res(left.size_ + right.size_);
        for (size_t i = 0; i < left.size_; ++i) res.ptr_[i] = left.ptr_[i];
        for (size_t i = 0; i < right.size_; ++i) res.ptr_[left.size_ + i] = right.ptr_[i];
        res.ptr_[res.size_] = 0;
        return res;
    }

    template <typename S>
    friend S& operator<<(S& os, const simple_basic_string& obj) {
        for (size_t i = 0; i < obj.size_; ++i) os << obj.ptr_[i];
        return os;
    }

    template <typename S>
    friend S& operator>>(S& is, simple_basic_string& obj) {
        std::string tmp;
        is >> tmp;
        obj = tmp.c_str();
        return is;
    }

    simple_basic_string& operator+=(const simple_basic_string& other) {
        *this = *this + other;
        return *this;
    }

    simple_basic_string& operator+=(T symbol) {
        T str[2] = {symbol, 0};
        *this = *this + simple_basic_string(str);
        return *this;
    }

    T& operator[](size_t index) noexcept { return ptr_[index]; }

    T& at(size_t index) {
        if (index >= size_) throw std::out_of_range("Wrong index");
        return ptr_[index];
    }

    T* data() { return ptr_; }

private:
    static size_t strlen_(const T* str) {
        size_t len = 0;
        while (str && str[len]) ++len;
        return len;
    }

    T* ptr_ = nullptr;
    size_t size_;
};

using string = simple_basic_string<char>;
using wstring = simple_basic_string<wchar_t>;
}