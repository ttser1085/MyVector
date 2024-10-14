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

    MyRAIterator();
    MyRAIterator(pointer p);

    reference operator*() const;
    pointer operator->() const;
    reference operator[](difference_type n) const;

    MyRAIterator& operator++();
    MyRAIterator operator++(int);
    MyRAIterator& operator--();
    MyRAIterator operator--(int);

    MyRAIterator& operator+=(difference_type n);
    MyRAIterator& operator-=(difference_type n);
    MyRAIterator operator+(difference_type n) const;
    MyRAIterator operator-(difference_type n) const;
    difference_type operator-(const MyRAIterator& other) const;

    bool operator==(const MyRAIterator& other) const;
    bool operator!=(const MyRAIterator& other) const;
    bool operator<(const MyRAIterator& other) const;
    bool operator<=(const MyRAIterator& other) const;
    bool operator>(const MyRAIterator& other) const;
    bool operator>=(const MyRAIterator& other) const;

private:
    pointer ptr = nullptr;
};
