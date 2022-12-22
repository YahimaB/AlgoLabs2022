//
// Created by Yaroslav on 11/9/2022.
//

#ifndef DYNAMIC_ARRAY_ARRAY_H
#define DYNAMIC_ARRAY_ARRAY_H

#include <algorithm>
#include <iostream>

template<typename T>
class Array {
private:
    template<bool Const>
    class Iterator {
    private:
        typedef typename std::conditional<Const, const Array<T> *, Array<T> *>::type ArrayType;

        ArrayType array_;
        int current_index_ = 0;
        bool reverse_ = false;

    public:
        explicit Iterator(ArrayType array, bool reverse) {
            array_ = array;
            reverse_ = reverse;
            current_index_ = reverse ? array->size_ - 1 : 0;
        }

        const T &get() const {
            return (*array_)[current_index_];
        }

        void set(const T &value) {
            (*array_)[current_index_] = value;
        }

        [[nodiscard]] bool hasNext() const {
            if (reverse_) {
                return current_index_ >= 0;
            } else {
                return current_index_ < array_->size_;
            }
        }

        void next() {
            if (!hasNext()) {
                throw std::out_of_range("Iterator out of range");
            }

            if (reverse_) {
                --current_index_;
            } else {
                ++current_index_;
            }
        }
    };

public:
    Array() : Array(DEFAULT_CAPACITY) {};

    explicit Array(int capacity) {
        capacity_ = capacity;
        data_ = alloc(capacity_);
    };

    Array(const Array &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = alloc(capacity_);

        for (int i = 0; i < size_; i++) {
            new(data_ + i) T(other.data_[i]);
        }

    }

    Array(Array &&other) noexcept {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    ~Array() {
        free();
    }

    [[nodiscard]] int size() const {
        return size_;
    };

    [[nodiscard]] int capacity() const {
        return capacity_;
    };

    int insert(const T &value) {
        return insert(size_, value);
    };

    int insert(int index, const T &value) {
        if (size_ == capacity_) {
            resize();
        }

        if (index == size_) {
            new (data_ + size_) T(value);
        } else {
            new (data_ + size_) T(std::move(data_[size_ - 1]));
            for (int i = size_ - 1; i > index; i--) {
                data_[i] = std::move(data_[i - 1]);
            }

            data_[index] = value;
        }

        size_++;

        return index;
    };

    void remove(int index) {
        for (int i = index; i < size_ - 1; i++) {
            data_[i] = std::move(data_[i + 1]);
        }
        data_[size_ - 1].~T();
        size_--;
    };

    const T &operator[](int index) const {
        return data_[index];
    };

    T &operator[](int index) {
        return data_[index];
    };

    Array &operator=(const Array &other) {
        if (this != &other) {
            free();

            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = alloc(capacity_);

            for (int i = 0; i < other.size_; i++)
                new(data_ + i) T(other.data_[i]);
        }

        return *this;
    }

    Array &operator=(Array &&other) noexcept {
        if (this != &other) {
            free();

            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;

            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }

        return *this;
    }

    Iterator<false> iterator() { return Iterator<false>(this, false); }

    Iterator<true> iterator() const { return Iterator<true>(this, false); }

    Iterator<false> reverseIterator() { return Iterator<false>(this, true); }

    Iterator<true> reverseIterator() const { return Iterator<true>(this, true); }

private:
    static constexpr int DEFAULT_CAPACITY = 4;
    static constexpr int RESIZE_VALUE = 2;
    friend class ArrayTest;
    int size_ = 0;
    int capacity_ = 0;
    T *data_ = nullptr;

    void resize() {
        capacity_ = capacity_ == 0 ? DEFAULT_CAPACITY : capacity_ * RESIZE_VALUE;

        T *temp = alloc(capacity_);
        for (int i = 0; i < size_; i++) {
            new(temp + i) T{std::move(data_[i])};
        }

        std::free(data_);
        data_ = temp;
    }

    T *alloc(size_t size) {
        return (T *) std::malloc(size * sizeof(T));
    }

    void free() {
        for (int i = 0; i < size_; i++)
            data_[i].~T();

        std::free(data_);
    }
};

#endif //DYNAMIC_ARRAY_ARRAY_H