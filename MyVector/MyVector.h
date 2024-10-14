#pragma once

#include "MyRAIterator.h"

#include <memory>

template<class T, class Allocator = std::allocator<T>>
class MyVector {
public:
    using ref = T&;
    using const_ref = const T&;
    using ptr = typename std::allocator_traits<Allocator>::pointer;
    using const_ptr = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = MyRAIterator<T>;
    using const_iterator = MyRAIterator<const T>;

    MyVector() : size_(0), capacity_(DEFAULT_CAPACITY_), allocator_() {
        data_ = allocator_.allocate(capacity_);
    }

    MyVector(size_t count);

    bool empty() const;
    size_t size() const;
    size_t capacity() const;

private:
    ptr data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;

    static const size_t DEFAULT_CAPACITY_ = 32;
};