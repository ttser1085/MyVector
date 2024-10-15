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
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
            other.allocator_ = Allocator();
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

            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
            other.allocator_ = Allocator();
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

    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ != 0 ? capacity_ * 2 : 1);
        }

        allocator_.construct(data_ + size_, value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ != 0 ? capacity_ * 2 : 1);
        }

        allocator_.construct(data_ + size_, std::move(value));
        ++size_;
    }

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

    // Assign operators:

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value &&
                allocator_ != other.allocator_) {
                
                for (size_t i = 0; i < size_; ++i) {
                    allocator_.destroy(&data_[i]);
                }
                allocator_.deallocate(data_, capacity_);
                allocator_ = other.allocator_;

                capacity_ = other.capacity_;
                data_ = allocator_.allocate(capacity_);
                std::uninitialized_copy(other.data_, other.data_ + other.size_, data_);

            } else {
                if (other.size_ > capacity_) {
                    T* new_data = allocator_.allocate(other.size_);

                    for (size_t i = 0; i < size_; ++i) {
                        allocator_.destroy(&data_[i]);
                    }
                    allocator_.deallocate(data_, capacity_);

                    data_ = new_data;
                    capacity_ = other.size_;
                }

                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }

                for (size_t i = size_; i < other.size_; ++i) {
                    allocator_.construct(data_ + i, other.data_[i]);
                }

                for (size_t i = other.size_; i < size_; ++i) {
                    allocator_.destroy(&data_[i]);
                }
            }
        }

        return *this;
    }

    MyVector& operator=(MyVector&& other)
        noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
        || std::allocator_traits<Allocator>::is_always_equal::value) {

        if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            for (size_t i = 0; i < size_; ++i) {
                allocator_.destroy(&data_[i]);
            }
            allocator_.deallocate(data_, capacity_);
            allocator_ = std::move(other.allocator_);
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        } else {
            if (allocator_ != other.allocator_) {
                if (other.size_ > capacity_) {
                    T* new_data = allocator_.allocate(other.capacity_);

                    for (size_t i = 0; i < size_; ++i) {
                        allocator_.destroy(&data_[i]);
                    }
                    allocator_.deallocate(data_, capacity_);

                    data_ = new_data;
                    capacity_ = other.capacity_;
                }

                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = std::move(other.data_[i]);
                }

                for (size_t i = size_; i < other.size_; ++i) {
                    allocator_.construct(data_ + i, std::move(other.data_[i]));
                }

                for (size_t i = other.size_; i < size_; ++i) {
                    allocator_.destroy(&data_[i]);
                }

            } else {
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;

                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }

            size_ = other.size_;
        }

        return *this;
    }

    MyVector& operator=(std::initializer_list<T> ilist) {
        if (ilist.size() > capacity_) {
            T* new_data = allocator_.allocate(ilist.size());

            for (size_t i = 0; i < size_; ++i) {
                allocator_.destroy(&data_[i]);
            }
            allocator_.deallocate(data_, capacity_);

            data_ = new_data;
            capacity_ = ilist.size();
        }

        size_t i = 0;
        for (const T& elem : ilist) {
            if (i < size_) {
                data_[i] = elem;
            } else {
                allocator_.construct(data_ + i, elem);
            }
            ++i;
        }

        for (size_t j = ilist.size(); j < size_; ++j) {
            allocator_.destroy(&data_[i]);
        }

        size_ = ilist.size();
        return *this;
    }

    // ------

    ~MyVector() {
        for (size_t i = 0; i < size_; ++i) {
            allocator_.destroy(&data_[i]);
        }

        allocator_.deallocate(data_, capacity_);
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

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = allocator_.allocate(new_capacity);
            std::uninitialized_move(data_, data_ + size_, new_data);

            for (size_t i = 0; i < size_; ++i) {
                allocator_.destroy(&data_[i]);
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