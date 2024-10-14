#pragma once

#include "MyRAIterator.h"

#include <memory>
#include <utility>
#include <exception>
#include <algorithm>
#include <initializer_list>

template<class T, class Allocator = std::allocator<T>>
class MyVector {
public:
    using ref = T&;
    using const_ref = const T&;
    using ptr = typename std::allocator_traits<Allocator>::pointer;
    using const_ptr = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = MyRAIterator<T>;
    using const_iterator = MyRAIterator<const T>;

    // Constructors:

    MyVector() noexcept(noexcept(Allocator())) : data_(nullptr), size_(0), capacity_(0), allocator_() {}

    explicit MyVector(const Allocator& alloc) noexcept : data_(nullptr), size_(0), capacity_(0), allocator_(alloc) {}

    explicit MyVector(size_t count, const Allocator& alloc = Allocator()) : size_(count), capacity_(count), allocator_(alloc) {
        data_ = nullptr;
        if (count != 0) {
            data_ = allocator_.allocate(capacity_); // can throw bad_alloc
            std::uninitialized_default_construct_n(data_, size_);
        }
    }

    explicit MyVector(size_t count, const T& value, const Allocator& alloc = Allocator())
        : size_(count), capacity_(count), allocator_(alloc) {
        data_ = nullptr;
        if (count != 0){
            data_ = allocator_.allocate(capacity_); // can throw bad_alloc
            std::uninitialized_fill_n(data_, size_, value);
        }
    }

    MyVector(const MyVector& other) : size_(other.size_), capacity_(other.capacity_) {
        allocator_ = std::allocator_traits<Allocator>::select_on_container_copy_construction(other.allocator_);
        if (capacity_ != 0) {
            data_ = allocator_.allocate(capacity_); // can throw bad_alloc
            std::uninitialized_copy(other.data_, other.data_ + size_, data_);
        }
    }

    MyVector(const MyVector& other, const Allocator& alloc) : size_(other.size_), capacity_(other.capacity_), allocator_(alloc) {
        if (capacity_ != 0) {
            data_ = allocator_.allocate(capacity_); // can throw bad_alloc
            std::uninitialized_copy(other.data_, other.data_ + size_, data_);
        }
    }

    MyVector(MyVector&& other) noexcept : size_(other.size_), capacity_(other.capacity_),
        data_(other.data_), allocator_(std::move(other.allocator_)) {
            other = MyVector();
    }

    MyVector(MyVector&& other, const Allocator& alloc) {
        if (alloc != other.allocator_) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            if (capacity_ != 0) {
                data_ = allocator_.allocate(capacity_);
                std::uninitialized_move(other.data_, other.data_ + size_, data_);
            }

        } else {
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            allocator_ = alloc;

            other = MyVector();
        }
    }

    MyVector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
        size_(init.size()), capacity_(init.size()), allocator_(alloc) {
        
        if (capacity_ != 0) {
            data_ = allocator_.allocate(capacity_);
            std::uninitialized_copy(init.begin(), init.end(), data_);
        }
    }

    // ------

    void swap(MyVector& other) 
        noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value
        || std::allocator_traits<Allocator>::is_always_equal::value) {

        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);

        if (std::allocator_traits<Allocator>::propagate_on_container_swap::value) {
            std::swap(allocator_, other.allocator_);
        } // else ub :)
    }
    
    // ------

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = allocator_.allocate(new_capacity);
            std::uninitialized_move(data_, data_ + size_, new_data);

            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            allocator_.deallocate(data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;
};