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

    MyVector() = default;

private:
    ptr data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = DEFAULT_CAPACITY_;

    static const size_t DEFAULT_CAPACITY_ = 32;
};