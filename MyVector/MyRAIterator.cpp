#include "MyRAIterator.h"

template <class T>
MyRAIterator<T>::MyRAIterator() {}

template <class T>
MyRAIterator<T>::MyRAIterator(pointer p) : ptr(p) {}

template <class T>
typename MyRAIterator<T>::reference MyRAIterator<T>::operator*() const {
    return *ptr;
}

template <class T>
typename MyRAIterator<T>::pointer MyRAIterator<T>::operator->() const {
    return ptr;
}

template <class T>
typename MyRAIterator<T>::reference MyRAIterator<T>::operator[](difference_type n) const {
    return *(ptr + n);
}

template <class T>
MyRAIterator<T>& MyRAIterator<T>::operator++() {
    ++ptr;
    return *this;
}

template <class T>
MyRAIterator<T> MyRAIterator<T>::operator++(int) {
    MyRAIterator temp = *this;
    ++ptr;
    return temp;
}

template <class T>
MyRAIterator<T>& MyRAIterator<T>::operator--() {
    --ptr;
    return *this;
}

template <class T>
MyRAIterator<T> MyRAIterator<T>::operator--(int) {
    MyRAIterator temp = *this;
    --ptr;
    return temp;
}

template <class T>
MyRAIterator<T>& MyRAIterator<T>::operator+=(difference_type n) {
    ptr += n;
    return *this;
}

template <class T>
MyRAIterator<T>& MyRAIterator<T>::operator-=(difference_type n) {
    ptr -= n;
    return *this;
}

template <class T>
MyRAIterator<T> MyRAIterator<T>::operator+(difference_type n) const {
    return MyRAIterator(ptr + n);
}

template <class T>
MyRAIterator<T> MyRAIterator<T>::operator-(difference_type n) const {
    return MyRAIterator(ptr - n);
}

template <class T>
typename MyRAIterator<T>::difference_type MyRAIterator<T>::operator-(const MyRAIterator& other) const {
    return ptr - other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator==(const MyRAIterator& other) const {
    return ptr == other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator!=(const MyRAIterator& other) const {
    return ptr != other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator<(const MyRAIterator& other) const {
    return ptr < other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator<=(const MyRAIterator& other) const {
    return ptr <= other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator>(const MyRAIterator& other) const {
    return ptr > other.ptr;
}

template <class T>
bool MyRAIterator<T>::operator>=(const MyRAIterator& other) const {
    return ptr >= other.ptr;
}
