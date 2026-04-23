/*
# Names: Brandon Daniel and Cameron O'Dell
# Date: 4/24/2026
# File: deque.h
*/
#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class deque {
public:
    deque();
    ~deque();

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    T front() const;
    T back() const;

    void push_front(const T& value);
    void push_back(const T& value);

    void pop_front();
    void pop_back();

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

private:
    static constexpr std::size_t BLOCK_SIZE = 16;
    static constexpr std::size_t INITIAL_MAP_SIZE = 8;

    T** blockmap;
    std::size_t map_capacity;
    std::size_t map_front;
    std::size_t map_back;
    std::size_t front_offset;
    std::size_t back_offset;
    std::size_t size_;

    void allocate_map(std::size_t new_capacity);
    void ensure_map_capacity_front();
    void ensure_map_capacity_back();
    void allocate_block(std::size_t block_index);
    void deallocate_block(std::size_t block_index);
    void ensure_nonempty() const;
    std::size_t element_block(std::size_t index) const;
    std::size_t element_offset(std::size_t index) const;
};

template <typename T>
deque<T>::deque()
    : blockmap(nullptr),
      map_capacity(0),
      map_front(0),
      map_back(0),
      front_offset(0),
      back_offset(0),
      size_(0) {
    allocate_map(INITIAL_MAP_SIZE);
    map_front = map_back = map_capacity / 2;
    front_offset = back_offset = BLOCK_SIZE / 2;
}

template <typename T>
deque<T>::~deque() {
    if (blockmap) {
        for (std::size_t i = 0; i < map_capacity; ++i) {
            delete[] blockmap[i];
        }
        delete[] blockmap;
    }
}

template <typename T>
bool deque<T>::empty() const noexcept {
    return size_ == 0;
}

template <typename T>
std::size_t deque<T>::size() const noexcept {
    return size_;
}

template <typename T>
T deque<T>::front() const {
    ensure_nonempty();
    return blockmap[map_front][front_offset];
}

template <typename T>
T deque<T>::back() const {
    ensure_nonempty();
    return blockmap[map_back][back_offset];
}

template <typename T>
void deque<T>::push_back(const T& value) {
    if (empty()) {
        allocate_block(map_back);
        blockmap[map_back][back_offset] = value;
        size_ = 1;
        return;
    }

    if (back_offset + 1 < BLOCK_SIZE) {
        ++back_offset;
        blockmap[map_back][back_offset] = value;
    } else {
        ensure_map_capacity_back();
        ++map_back;
        allocate_block(map_back);
        back_offset = 0;
        blockmap[map_back][back_offset] = value;
    }
    ++size_;
}

template <typename T>
void deque<T>::push_front(const T& value) {
    if (empty()) {
        allocate_block(map_front);
        blockmap[map_front][front_offset] = value;
        size_ = 1;
        return;
    }

    if (front_offset > 0) {
        --front_offset;
        blockmap[map_front][front_offset] = value;
    } else {
        ensure_map_capacity_front();
        --map_front;
        allocate_block(map_front);
        front_offset = BLOCK_SIZE - 1;
        blockmap[map_front][front_offset] = value;
    }
    ++size_;
}

template <typename T>
void deque<T>::pop_front() {
    ensure_nonempty();
    if (size_ == 1) {
        size_ = 0;
        return;
    }

    if (front_offset + 1 < BLOCK_SIZE) {
        ++front_offset;
    } else {
        deallocate_block(map_front);
        ++map_front;
        front_offset = 0;
    }
    --size_;
}

template <typename T>
void deque<T>::pop_back() {
    ensure_nonempty();
    if (size_ == 1) {
        size_ = 0;
        return;
    }

    if (back_offset > 0) {
        --back_offset;
    } else {
        deallocate_block(map_back);
        --map_back;
        back_offset = BLOCK_SIZE - 1;
    }
    --size_;
}

template <typename T>
T& deque<T>::operator[](std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("deque index out of range");
    }
    std::size_t block = element_block(index);
    std::size_t offset = element_offset(index);
    return blockmap[block][offset];
}

template <typename T>
const T& deque<T>::operator[](std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("deque index out of range");
    }
    std::size_t block = element_block(index);
    std::size_t offset = element_offset(index);
    return blockmap[block][offset];
}

template <typename T>
void deque<T>::allocate_map(std::size_t new_capacity) {
    T** new_map = new T*[new_capacity];
    for (std::size_t i = 0; i < new_capacity; ++i) {
        new_map[i] = nullptr;
    }

    if (blockmap) {
        std::size_t current_blocks = map_back - map_front + 1;
        std::size_t new_front = (new_capacity - current_blocks) / 2;
        for (std::size_t i = 0; i < current_blocks; ++i) {
            new_map[new_front + i] = blockmap[map_front + i];
        }
        map_front = new_front;
        map_back = map_front + current_blocks - 1;
        delete[] blockmap;
    }

    blockmap = new_map;
    map_capacity = new_capacity;
}

template <typename T>
void deque<T>::ensure_map_capacity_front() {
    if (map_front == 0) {
        allocate_map(map_capacity * 2);
    }
}

template <typename T>
void deque<T>::ensure_map_capacity_back() {
    if (map_back + 1 >= map_capacity) {
        allocate_map(map_capacity * 2);
    }
}

template <typename T>
void deque<T>::allocate_block(std::size_t block_index) {
    if (!blockmap[block_index]) {
        blockmap[block_index] = new T[BLOCK_SIZE];
    }
}

template <typename T>
void deque<T>::deallocate_block(std::size_t block_index) {
    delete[] blockmap[block_index];
    blockmap[block_index] = nullptr;
}

template <typename T>
void deque<T>::ensure_nonempty() const {
    if (empty()) {
        throw std::out_of_range("deque is empty");
    }
}

template <typename T>
std::size_t deque<T>::element_block(std::size_t index) const {
    std::size_t linear = front_offset + index;
    return map_front + (linear / BLOCK_SIZE);
}

template <typename T>
std::size_t deque<T>::element_offset(std::size_t index) const {
    std::size_t linear = front_offset + index;
    return linear % BLOCK_SIZE;
}

#endif // DEQUE_H
