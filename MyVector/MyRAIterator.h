#pragma once

#include <cstddef>
#include <iterator>

template <class T>
class MyRAIterator { // my random access iterator
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    MyRAIterator() = default;
    MyRAIterator(pointer p) : ptr(p) {}

    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }
    reference operator[](difference_type n) const { return *(ptr + n); }

    MyRAIterator& operator++() { ++ptr; return *this; }
    MyRAIterator operator++(int) { MyRAIterator temp = *this; ++ptr; return temp; }
    MyRAIterator& operator--() { --ptr; return *this; }
    MyRAIterator operator--(int) { MyRAIterator temp = *this; --ptr; return temp; }

    MyRAIterator& operator+=(difference_type n) { ptr += n; return *this; }
    MyRAIterator& operator-=(difference_type n) { ptr -= n; return *this; }

    MyRAIterator operator+(difference_type n) const { return MyRAIterator(ptr + n); }
    MyRAIterator operator-(difference_type n) const { return MyRAIterator(ptr - n); }

    difference_type operator-(const MyRAIterator& other) const { return ptr - other.ptr; }

    bool operator==(const MyRAIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const MyRAIterator& other) const { return ptr != other.ptr; }
    bool operator<(const MyRAIterator& other) const { return ptr < other.ptr; }
    bool operator<=(const MyRAIterator& other) const { return ptr <= other.ptr; }
    bool operator>(const MyRAIterator& other) const { return ptr > other.ptr; }
    bool operator>=(const MyRAIterator& other) const { return ptr >= other.ptr; }

private:
    pointer ptr = nullptr;
};
