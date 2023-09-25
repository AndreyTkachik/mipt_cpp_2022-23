#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
 private:
  std::vector<int> a_;
  size_t size_;
  size_t iter_ = 0;

 public:
  explicit RingBuffer(size_t capacity) {
    a_.reserve(capacity);
    size_ = capacity;
  }

  size_t Size() const { return iter_; }

  bool Empty() const { return iter_ == 0; }

  bool TryPush(int element) {
    if (iter_ == size_) {
      return false;
    }
    a_.push_back(element);
    ++iter_;
    return true;
  }

  bool TryPop(int* element) {
    if (iter_ == 0) {
      return false;
    }
    *element = a_[0];
    for (size_t i = 0; i < iter_ - 1; i++) {
      a_[i] = a_[i + 1];
    }
    a_.pop_back();
    --iter_;
    return true;
  }
};
