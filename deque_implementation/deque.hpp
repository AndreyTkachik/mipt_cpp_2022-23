#include <iostream>
#include <memory>
#include <vector>

template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 public:
  /*Class Iterator fo Deque*/
  template <bool IsConst>
  class Iterator
      : public std::iterator<std::random_access_iterator_tag,
                             std::conditional_t<IsConst, const T, T>> {
    friend Deque;

   public:
    Iterator(T** cont, const size_t kIndx, const bool& emp) {
      if (emp) {
        iter_container_ = nullptr;
        index_ = 0;
        empty_ = true;
      } else {
        iter_container_ = cont;
        index_ = kIndx;
        empty_ = false;
      }
    }

    Iterator<IsConst> operator++(int);

    Iterator<IsConst>& operator++();

    Iterator<IsConst> operator--(int);

    Iterator<IsConst>& operator--();

    std::conditional_t<IsConst, const T&, T&> operator*() const;

    std::conditional_t<IsConst, const T*, T*> operator->() const;

    Iterator<IsConst>& operator+=(const int64_t& number);

    Iterator<IsConst> operator+(const int64_t& number) const;

    Iterator<IsConst>& operator-=(const int64_t& number);

    Iterator<IsConst> operator-(const int64_t& number) const;

    std::ptrdiff_t operator-(const Iterator<IsConst>& other);

    bool operator<(const Iterator<IsConst>& other) const;

    bool operator==(const Iterator<IsConst>& other) const;

    bool operator<=(const Iterator<IsConst>& other) const;

    bool operator>(const Iterator<IsConst>& other) const;

    bool operator>=(const Iterator<IsConst>& other) const;

    bool operator!=(const Iterator<IsConst>& other) const;

    size_t get_iter_index();

    T** get_iter_pointer();

   private:
    T** iter_container_;
    int64_t index_;
    bool empty_ = false;
  };

  /*Main public functions for Deque*/
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin();

  const_iterator cbegin() const;

  const_iterator begin() const;

  iterator end();

  const_iterator cend() const;

  const_iterator end() const;

  reverse_iterator rbegin();

  const_reverse_iterator crbegin() const;

  const_reverse_iterator rbegin() const;

  reverse_iterator rend();

  const_reverse_iterator crend() const;

  const_reverse_iterator rend() const;

  Deque() = default;

  explicit Deque(const Allocator& alloc);

  Deque(const Deque<T, Allocator>& from);

  explicit Deque(const size_t& count, const Allocator& alloc = Allocator());

  explicit Deque(const size_t& count, const T& value,
                 const Allocator& alloc = Allocator());

  Deque(Deque<T, Allocator>&& from);

  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator());

  Deque<T, Allocator>& operator=(const Deque<T, Allocator>& other);

  Deque<T, Allocator>& operator=(Deque<T, Allocator>&& from);

  ~Deque();

  size_t size() const { return size_; }

  bool empty() { return (size_ == 0); }

  T& operator[](size_t indx);

  const T& operator[](size_t indx) const;

  T& at(size_t indx);

  const T& at(size_t indx) const;

  template <class... Args>
  void emplace_back(Args&&... args);

  template <class... Args>
  void emplace_front(Args&&... args);

  void push_back(const T& value) { emplace_back(value); }

  void push_front(const T& value) { emplace_front(value); }

  void push_back(T&& value) { emplace_back(std::move(value)); }

  void push_front(T&& value) { emplace_front(std::move(value)); }

  void pop_back();

  void pop_front();

  void insert(iterator iter, const T& value);

  void emplace(iterator iter, T&& value);

  void erase(iterator iter);

  using value_type = T;
  using allocator_type = Allocator;

  Allocator get_allocator() { return alloc_; }

  /*Private fields of Deque*/
 private:
  void resize();

  void clear();

  void need_of_alloc();

  void deallocation_process();

  using alloc_traits = std::allocator_traits<Allocator>;

  using cont_allocator = typename alloc_traits::template rebind_alloc<T*>;
  using cont_alloc_traits = typename alloc_traits::template rebind_traits<T*>;

  Allocator alloc_;
  cont_allocator cont_alloc_;

  T** container_ = nullptr;
  T** front_chunk_ = nullptr;
  T** back_chunk_ = nullptr;
  size_t front_elem_ = 0;
  size_t back_elem_ = 0;
  size_t capacity_ = 0;
  size_t size_ = 0;

  const size_t kChunkSize = 32;
};

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>
Deque<T, Allocator>::Iterator<IsConst>::operator++(int) {
  Iterator<IsConst> tmp = *this;
  const int64_t kIterSize = 32;
  ++index_;
  if (index_ >= kIterSize) {
    index_ = 0;
    ++iter_container_;
  }
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>&
Deque<T, Allocator>::Iterator<IsConst>::operator++() {
  const int64_t kIterSize = 32;
  ++index_;
  if (index_ >= kIterSize) {
    index_ = 0;
    ++iter_container_;
  }
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>
Deque<T, Allocator>::Iterator<IsConst>::operator--(int) {
  Iterator<IsConst> tmp = *this;
  const int64_t kIterSize = 32;
  --index_;
  if (index_ < 0) {
    index_ = kIterSize - 1;
    --iter_container_;
  }
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>&
Deque<T, Allocator>::Iterator<IsConst>::operator--() {
  const int64_t kIterSize = 32;
  --index_;
  if (index_ < 0) {
    index_ = kIterSize - 1;
    --iter_container_;
  }
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
std::conditional_t<IsConst, const T&, T&>
Deque<T, Allocator>::Iterator<IsConst>::operator*() const {
  return *(*(iter_container_) + index_);
}

template <typename T, typename Allocator>
template <bool IsConst>
std::conditional_t<IsConst, const T*, T*>
Deque<T, Allocator>::Iterator<IsConst>::operator->() const {
  return *(iter_container_) + index_;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>&
Deque<T, Allocator>::Iterator<IsConst>::operator+=(const int64_t& number) {
  const int64_t kIterSize = 32;
  index_ += number;
  if (index_ >= kIterSize) {
    size_t interval = index_ / kIterSize;
    index_ -= interval * kIterSize;
    iter_container_ += interval;
  }
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>
Deque<T, Allocator>::Iterator<IsConst>::operator+(const int64_t& number) const {
  Iterator<IsConst> tmp = *this;
  tmp += number;
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>&
Deque<T, Allocator>::Iterator<IsConst>::operator-=(const int64_t& number) {
  const int64_t kIterSize = 32;
  index_ -= number;
  if (index_ < 0) {
    index_ = std::abs(index_);
    size_t interval = (index_ / kIterSize) + 1;
    index_ = (interval * kIterSize) - index_;
    if (index_ == kIterSize) {
      index_ = 0;
      --interval;
    }
    iter_container_ -= interval;
  }
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename Deque<T, Allocator>::template Iterator<IsConst>
Deque<T, Allocator>::Iterator<IsConst>::operator-(const int64_t& number) const {
  Iterator<IsConst> tmp = *this;
  tmp -= number;
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
std::ptrdiff_t Deque<T, Allocator>::Iterator<IsConst>::operator-(
    const Iterator<IsConst>& other) {
  const int64_t kIterSize = 32;
  int64_t interval = iter_container_ - other.iter_container_;
  int64_t answer = index_ - other.index_;
  answer += interval * kIterSize;
  return answer;
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator<(
    const Iterator<IsConst>& other) const {
  return (
      ((iter_container_ == other.iter_container_) && (index_ < other.index_)) ||
      ((iter_container_ - other.iter_container_) < 0));
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator==(
    const Iterator<IsConst>& other) const {
  return (iter_container_ == other.iter_container_ && index_ == other.index_);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator<=(
    const Iterator<IsConst>& other) const {
  return (*this < other) || (*this == other);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator>(
    const Iterator<IsConst>& other) const {
  return !(*this < other);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator>=(
    const Iterator<IsConst>& other) const {
  return !(*this < other) || (*this == other);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool Deque<T, Allocator>::Iterator<IsConst>::operator!=(
    const Iterator<IsConst>& other) const {
  return !(*this == other);
}

template <typename T, typename Allocator>
template <bool IsConst>
size_t Deque<T, Allocator>::Iterator<IsConst>::get_iter_index() {
  return index_;
}

template <typename T, typename Allocator>
template <bool IsConst>
T** Deque<T, Allocator>::Iterator<IsConst>::get_iter_pointer() {
  return iter_container_;
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::iterator Deque<T, Allocator>::begin() {
  bool emp;
  (container_ == nullptr || size_ == 0) ? emp = true : emp = false;
  return iterator(front_chunk_, front_elem_, emp);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cbegin()
    const {
  bool emp;
  (container_ == nullptr || size_ == 0) ? emp = true : emp = false;
  return const_iterator(front_chunk_, front_elem_, emp);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::begin()
    const {
  return cbegin();
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::iterator Deque<T, Allocator>::end() {
  bool emp;
  (container_ == nullptr || size_ == 0) ? emp = true : emp = false;
  if (emp) {
    return iterator(back_chunk_, back_elem_, emp);
  }
  return ++iterator(back_chunk_, back_elem_, emp);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cend() const {
  bool emp;
  (container_ == nullptr || size_ == 0) ? emp = true : emp = false;
  if (emp) {
    return const_iterator(back_chunk_, back_elem_, emp);
  }
  return ++const_iterator(back_chunk_, back_elem_, emp);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::end() const {
  return cend();
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rbegin() {
  return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator
Deque<T, Allocator>::crbegin() const {
  return const_reverse_iterator(cend());
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator
Deque<T, Allocator>::rbegin() const {
  return crbegin();
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rend() {
  auto ans = begin();
  if (container_ == nullptr || size_ == 0) {
    return reverse_iterator(ans);
  }
  return reverse_iterator(--ans);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator
Deque<T, Allocator>::crend() const {
  auto ans = cbegin();
  if (container_ == nullptr || size_ == 0) {
    return const_reverse_iterator(ans);
  }
  return const_reverse_iterator(--ans);
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator Deque<T, Allocator>::rend()
    const {
  return crend();
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const Allocator& alloc) {
  alloc_ = alloc;
  cont_alloc_ = alloc;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const Deque<T, Allocator>& from) {
  if (from.container_ != nullptr) {
    alloc_ = alloc_traits::select_on_container_copy_construction(from.alloc_);
    cont_alloc_ = cont_alloc_traits::select_on_container_copy_construction(
        from.cont_alloc_);
    container_ = cont_alloc_traits::allocate(cont_alloc_, from.capacity_);
    for (size_t indx = 0; indx < from.capacity_; ++indx) {
      container_[indx] = alloc_traits::allocate(alloc_, kChunkSize);
    }
    capacity_ = from.capacity_;
    front_chunk_ = container_ + (from.size_ / kChunkSize) + 1;
    back_chunk_ = container_ + (from.size_ / kChunkSize) + 1;
    front_elem_ = 0;
    back_elem_ = 0;
    for (auto indx = from.begin(); indx != from.end(); ++indx) {
      try {
        emplace_back(*indx);
      } catch (...) {
        size_t tmp = size_;
        for (size_t jndx = 0; jndx < tmp; ++jndx) {
          pop_back();
        }
        deallocation_process();
        cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
        throw;
      }
    }
    size_ = from.size_;
  }
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const size_t& count, const Allocator& alloc) {
  alloc_ = alloc;
  cont_alloc_ = alloc;
  count == 0 ? capacity_ = 3 : capacity_ = ((count / kChunkSize) + 1) * 3;
  front_elem_ = 0;
  back_elem_ = 0;
  container_ = cont_alloc_traits::allocate(cont_alloc_, capacity_);
  for (size_t indx = 0; indx < capacity_; ++indx) {
    container_[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  front_chunk_ = container_ + (count / kChunkSize) + 1;
  back_chunk_ = container_ + (count / kChunkSize) + 1;
  if (count != 0) {
    for (size_t indx = 0; indx < count; ++indx) {
      try {
        emplace_back();
      } catch (...) {
        size_t tmp = size_;
        for (size_t jndx = 0; jndx < tmp; ++jndx) {
          pop_back();
        }
        deallocation_process();
        cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
        throw;
      }
    }
  }
  size_ = count;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const size_t& count, const T& value,
                           const Allocator& alloc) {
  alloc_ = alloc;
  cont_alloc_ = alloc;
  capacity_ = ((count / kChunkSize) + 1) * 3;
  front_elem_ = 0;
  back_elem_ = 0;
  container_ = cont_alloc_traits::allocate(cont_alloc_, capacity_);
  for (size_t indx = 0; indx < capacity_; ++indx) {
    container_[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  front_chunk_ = container_ + (count / kChunkSize) + 1;
  back_chunk_ = container_ + (count / kChunkSize) + 1;
  for (size_t indx = 0; indx < count; ++indx) {
    try {
      emplace_back(value);
    } catch (...) {
      size_t tmp = size_;
      for (size_t jndx = 0; jndx < tmp; ++jndx) {
        pop_back();
      }
      deallocation_process();
      cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
      throw;
    }
  }
  size_ = count;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(Deque<T, Allocator>&& from) {
  alloc_ = (alloc_traits::select_on_container_copy_construction(from.alloc_));
  cont_alloc_ = (cont_alloc_traits::select_on_container_copy_construction(
      from.cont_alloc_));
  container_ = from.container_;
  front_chunk_ = from.front_chunk_;
  back_chunk_ = from.back_chunk_;
  front_elem_ = from.front_elem_;
  back_elem_ = from.back_elem_;
  size_ = from.size_;
  capacity_ = from.capacity_;
  from.need_of_alloc();
  from.front_elem_ = 0;
  from.back_elem_ = 0;
  from.size_ = 0;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(std::initializer_list<T> init,
                           const Allocator& alloc) {
  alloc_ = alloc;
  cont_alloc_ = alloc;
  capacity_ = ((init.size() / kChunkSize) + 1) * 3;
  container_ = cont_alloc_traits::allocate(cont_alloc_, capacity_);
  for (size_t indx = 0; indx < capacity_; ++indx) {
    container_[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  front_chunk_ = container_ + (init.size() / kChunkSize) + 1;
  back_chunk_ = container_ + (init.size() / kChunkSize) + 1;
  front_elem_ = 0;
  back_elem_ = 0;
  for (auto indx = init.begin(); indx != init.end(); ++indx) {
    try {
      emplace_back(*indx);
    } catch (...) {
      size_t tmp = size_;
      for (size_t jndx = 0; jndx < tmp; ++jndx) {
        pop_back();
      }
      deallocation_process();
      cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
      throw;
    }
  }
  size_ = init.size();
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(
    const Deque<T, Allocator>& other) {
  if (alloc_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = other.alloc_;
    cont_alloc_ = other.cont_alloc_;
  }
  size_t inserted_elements = 0;
  auto iter1 = begin();
  for (auto iter = other.begin(); iter != other.end(); ++iter, ++iter1) {
    if (inserted_elements < size_) {
      *iter1 = *iter;
      ++inserted_elements;
      continue;
    }
    try {
      emplace_back(*iter);
      ++inserted_elements;
    } catch (...) {
      clear();
    }
  }
  while (size_ > other.size_) {
    pop_back();
  }
  size_ = other.size_;
  return *this;
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(
    Deque<T, Allocator>&& from) {
  if (alloc_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = from.alloc_;
    cont_alloc_ = from.cont_alloc_;
  }
  if (container_ != nullptr) {
    clear();
  }
  container_ = from.container_;
  front_chunk_ = from.front_chunk_;
  back_chunk_ = from.back_chunk_;
  front_elem_ = from.front_elem_;
  back_elem_ = from.back_elem_;
  size_ = from.size_;
  capacity_ = from.capacity_;
  from.need_of_alloc();
  from.front_elem_ = 0;
  from.back_elem_ = 0;
  from.size_ = 0;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::~Deque() {
  size_t tmp = size_;
  for (size_t indx = 0; indx < tmp; ++indx) {
    pop_back();
  }
  for (size_t indx = 0; indx < capacity_; ++indx) {
    alloc_traits::deallocate(alloc_, container_[indx], kChunkSize);
  }
  cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::operator[](const size_t kIndx) {
  auto iter = begin();
  return *(iter + kIndx);
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::operator[](const size_t kIndx) const {
  auto iter = begin();
  return *(iter + kIndx);
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::at(const size_t kIndx) {
  if (kIndx >= size_) {
    throw std::out_of_range("out_of_range");
  }
  auto iter = begin();
  return *(iter + kIndx);
  ;
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::at(const size_t kIndx) const {
  if (kIndx >= size_) {
    throw std::out_of_range("out_of_range");
  }
  auto iter = begin();
  return *(iter + kIndx);
}

template <typename T, typename Allocator>
template <class... Args>
void Deque<T, Allocator>::emplace_back(Args&&... args) {
  ++back_elem_;
  if (back_elem_ >= kChunkSize) {
    back_elem_ = 0;
    if (back_chunk_ == container_ + capacity_ - 1) {
      resize();
    }
    ++back_chunk_;
  }
  if (back_chunk_ == front_chunk_ && size_ == 0) {
    back_elem_ = 0;
  }
  if (container_ == nullptr) {
    need_of_alloc();
  }
  try {
    alloc_traits::construct(alloc_, *back_chunk_ + back_elem_,
                            std::forward<Args>(args)...);
  } catch (...) {
    throw;
  }
  ++size_;
}

template <typename T, typename Allocator>
template <class... Args>
void Deque<T, Allocator>::emplace_front(Args&&... args) {
  --front_elem_;
  if (front_elem_ == SIZE_MAX) {
    front_elem_ = kChunkSize - 1;
    if (front_chunk_ == container_) {
      resize();
    }
    --front_chunk_;
  }
  if (back_chunk_ == front_chunk_ && size_ == 0) {
    front_elem_ = 0;
  }
  if (container_ == nullptr) {
    need_of_alloc();
  }
  try {
    alloc_traits::construct(alloc_, *front_chunk_ + front_elem_,
                            std::forward<Args>(args)...);
  } catch (...) {
    alloc_traits::destroy(alloc_, *front_chunk_ + front_elem_);
    ++front_elem_;
    throw;
  }
  ++size_;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::pop_back() {
  --size_;
  alloc_traits::destroy(alloc_, (*back_chunk_) + back_elem_);
  --back_elem_;
  if (back_elem_ == SIZE_MAX) {
    --back_chunk_;
    back_elem_ = kChunkSize - 1;
  }
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::pop_front() {
  --size_;
  alloc_traits::destroy(alloc_, (*front_chunk_) + front_elem_);
  ++front_elem_;
  if (front_elem_ >= kChunkSize) {
    ++front_chunk_;
    front_elem_ = 0;
  }
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::insert(iterator iter, const T& value) {
  if (iter == end()) {
    emplace_back(value);
    return;
  }
  emplace_back(value);
  auto iter1 = end();
  --iter1;
  for (; iter1 != iter; --iter1) {
    std::swap(*iter1, *(iter1 - 1));
  }
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::emplace(iterator iter, T&& value) {
  emplace_back(std::move(value));
  auto iter1 = end();
  --iter1;
  if ((iter1.get_iter_index() == iter.get_iter_index() &&
       iter1.get_iter_pointer() == iter.get_iter_pointer()) ||
      iter.get_iter_pointer() == nullptr) {
    return;
  }
  for (; iter1 != iter; --iter1) {
    std::swap(*iter1, *(iter1 - 1));
  }
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::erase(iterator iter) {
  auto iter1 = end();
  if (iter1.get_iter_index() == iter.get_iter_index() &&
      iter1.get_iter_pointer() == iter.get_iter_pointer()) {
    pop_back();
    return;
  }
  for (auto iter2 = iter; iter2 != --end(); ++iter2) {
    std::swap(*iter2, *(iter2 + 1));
  }
  pop_back();
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::resize() {
  size_t interval = back_chunk_ - front_chunk_;
  size_t mini_interval = front_chunk_ - container_;
  T** new_container = cont_alloc_traits::allocate(cont_alloc_, capacity_ * 3);
  for (size_t indx = 0; indx < capacity_; ++indx) {
    new_container[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  for (size_t indx = 2 * (capacity_); indx < 3 * capacity_; ++indx) {
    new_container[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  for (size_t indx = 0; indx < capacity_; ++indx) {
    new_container[indx + capacity_] = container_[indx];
  }
  size_t tmp = size_;
  for (size_t jndx = 0; jndx < tmp; ++jndx) {
    pop_back();
  }
  cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
  container_ = std::move(new_container);
  capacity_ *= 3;
  new_container = nullptr;
  front_chunk_ = container_ + capacity_ / 3 + mini_interval;
  back_chunk_ = front_chunk_ + interval;
  size_ = tmp;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::clear() {
  size_t tmp = size_;
  for (size_t jndx = 0; jndx < tmp; ++jndx) {
    pop_back();
  }
  cont_alloc_traits::deallocate(cont_alloc_, container_, capacity_);
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::need_of_alloc() {
  capacity_ = 3;
  container_ = cont_alloc_traits::allocate(cont_alloc_, capacity_);
  for (size_t indx = 0; indx < capacity_; ++indx) {
    container_[indx] = alloc_traits::allocate(alloc_, kChunkSize);
  }
  front_chunk_ = container_ + 1;
  back_chunk_ = container_ + 1;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::deallocation_process() {
  for (size_t indx = 0; indx < capacity_; ++indx) {
    alloc_traits::deallocate(alloc_, container_[indx], kChunkSize);
  }
}
