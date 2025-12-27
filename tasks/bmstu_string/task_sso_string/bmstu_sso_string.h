#pragma once

#include <exception>
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>

namespace bmstu {
template <typename T>
class basic_string {
private:
    static constexpr size_t SSO_CAPACITY = (sizeof(T*) + 2 * sizeof(size_t)) / sizeof(T) - 1;

    struct LongString {
        T* ptr;
        size_t size;
        size_t capacity;
    };

    struct ShortString {
        T buffer[SSO_CAPACITY + 1];
        unsigned char size;
    };

    union Data {
        LongString l;
        ShortString s;
    };

    Data data_;
    bool is_long_;

    T* get_ptr() { return is_long_ ? data_.l.ptr : data_.s.buffer; }
    const T* get_ptr() const { return is_long_ ? data_.l.ptr : data_.s.buffer; }

public:
    basic_string() : is_long_(false) {
        data_.s.size = 0;
        data_.s.buffer[0] = 0;
    }

    basic_string(size_t size) : is_long_(false) {
        if (size > SSO_CAPACITY) {
            is_long_ = true;
            data_.l.ptr = new T[size + 1];
            data_.l.size = size;
            data_.l.capacity = size;
            for (size_t i = 0; i < size; ++i) data_.l.ptr[i] = static_cast<T>(' ');
            data_.l.ptr[size] = 0;
        } else {
            data_.s.size = static_cast<unsigned char>(size);
            for (size_t i = 0; i < size; ++i) data_.s.buffer[i] = static_cast<T>(' ');
            data_.s.buffer[size] = 0;
        }
    }

    basic_string(const T* c_str) : is_long_(false) {
        size_t len = strlen_(c_str);
        if (len > SSO_CAPACITY) {
            is_long_ = true;
            data_.l.ptr = new T[len + 1];
            for (size_t i = 0; i <= len; ++i) data_.l.ptr[i] = c_str[i];
            data_.l.size = len;
            data_.l.capacity = len;
        } else {
            for (size_t i = 0; i <= len; ++i) data_.s.buffer[i] = c_str[i];
            data_.s.size = static_cast<unsigned char>(len);
        }
    }

    basic_string(const basic_string& other) : is_long_(other.is_long_) {
        if (is_long_) {
            data_.l.ptr = new T[other.data_.l.capacity + 1];
            data_.l.size = other.data_.l.size;
            data_.l.capacity = other.data_.l.capacity;
            for (size_t i = 0; i <= data_.l.size; ++i) data_.l.ptr[i] = other.data_.l.ptr[i];
        } else {
            data_.s = other.data_.s;
        }
    }

    basic_string(basic_string&& dying) noexcept : is_long_(dying.is_long_), data_(dying.data_) {
        dying.is_long_ = false;
        dying.data_.s.size = 0;
        dying.data_.s.buffer[0] = 0;
    }

    ~basic_string() {
        if (is_long_) delete[] data_.l.ptr;
    }

    const T* c_str() const { return get_ptr(); }
    size_t size() const { return is_long_ ? data_.l.size : data_.s.size; }
    size_t capacity() const { return is_long_ ? data_.l.capacity : SSO_CAPACITY; }
    bool is_using_sso() const { return !is_long_; }

    basic_string& operator=(const basic_string& other) {
        if (this != &other) {
            if (is_long_) delete[] data_.l.ptr;
            is_long_ = other.is_long_;
            if (is_long_) {
                data_.l.ptr = new T[other.data_.l.capacity + 1];
                data_.l.size = other.data_.l.size;
                data_.l.capacity = other.data_.l.capacity;
                for (size_t i = 0; i <= data_.l.size; ++i) data_.l.ptr[i] = other.data_.l.ptr[i];
            } else {
                data_.s = other.data_.s;
            }
        }
        return *this;
    }

    T& operator[](size_t index) noexcept { return get_ptr()[index]; }

    template <typename S>
    friend S& operator<<(S& os, const basic_string& obj) {
        os << obj.c_str();
        return os;
    }

private:
    static size_t strlen_(const T* str) {
        size_t len = 0;
        while (str && str[len]) ++len;
        return len;
    }

    void clean_() {
        if (is_long_) delete[] data_.l.ptr;
    }
};

using string = basic_string<char>;
}