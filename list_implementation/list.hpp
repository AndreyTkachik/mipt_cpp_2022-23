#pragma once

#include <iostream>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  struct Node;
  struct NodePtr;

 public:
  template <bool IsConst>
  class Iterator
      : public std::iterator<std::bidirectional_iterator_tag,
                             std::conditional_t<IsConst, const T, T>> {
   public:
    Iterator(Node* constr) { current_ = constr; }

    Iterator(NodePtr* constr) { current_ = reinterpret_cast<Node*>(constr); }

    Iterator<IsConst> operator++(int);

    Iterator<IsConst>& operator++();

    Iterator<IsConst> operator--(int);

    Iterator<IsConst>& operator--();

    std::conditional_t<IsConst, const T&, T&> operator*();

    std::conditional_t<IsConst, const T*, T*> operator->();

    bool operator==(const Iterator<IsConst>& other) const;

    bool operator!=(const Iterator<IsConst>& other) const;

    friend List;

   private:
    Node* current_;
  };

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<Iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<Iterator<true>>;

  iterator begin() { return iterator(head_->next); }

  const_iterator cbegin() const { return const_iterator(head_->next); }

  const_iterator begin() const { return cbegin(); }

  iterator end() { return iterator(head_); }

  const_iterator cend() const { return const_iterator(head_); }

  const_iterator end() const { return cend(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator rbegin() const { return crbegin(); }

  reverse_iterator rend() {
    auto ans = begin();
    return reverse_iterator(--ans);
  }

  const_reverse_iterator crend() const {
    auto ans = cbegin();
    return const_reverse_iterator(--ans);
  }

  const_reverse_iterator rend() const { return crend(); }

  using value_type = T;
  using allocator_type = Allocator;

  size_t size() const { return size_; }

  void push_back(const T& value);

  void pop_back();

  List();

  explicit List(const size_t& count, const Allocator& alloc = Allocator());

  List(const size_t& count, const T& value,
       const Allocator& alloc = Allocator());

  List(List<T, Allocator>& other);

  List(std::initializer_list<T> init, const Allocator& alloc = Allocator());

  List& operator=(List<T, Allocator>& other);

  ~List();

  void push_front(const T& value);

  void push_back(T&& value);

  void push_front(T&& value);

  void pop_front();

  T& front() { return *begin(); }

  const T& front() const { return *begin(); }

  T& back() { return *(--end()); }

  const T& back() const { return *(--end()); }

  bool empty() { return (size_ == 0); }

  Allocator& get_allocator() { return value_alloc_; }

 private:
  using alloc_traits = std::allocator_traits<Allocator>;

  using constr_traits = std::allocator_traits<std::allocator<int>>;
  using head_alloc = std::allocator<NodePtr>;
  using head_alloc_traits = std::allocator_traits<head_alloc>;

  using node_alloc = typename alloc_traits::template rebind_alloc<Node>;
  using node_alloc_traits = typename alloc_traits::template rebind_traits<Node>;

  Allocator value_alloc_;
  node_alloc node_alloc_;

  NodePtr* head_ = nullptr;
  size_t size_ = 0;

  struct NodePtr {
    NodePtr* prev;
    NodePtr* next;

    NodePtr() : prev(nullptr), next(nullptr) {}

    NodePtr(NodePtr* new_node) : prev(new_node), next(new_node){};

    ~NodePtr() = default;
  };

  struct Node : NodePtr {
    using NodePtr::next;
    using NodePtr::prev;
    T value;

    Node() : NodePtr(), value() {}

    Node(const T& val) : NodePtr(), value(val) {}

    Node(T&& val) : NodePtr() { value = std::move(val); }
  };

  friend Node;
  friend NodePtr;

  void clear();

  void push_back_none();
};

template <typename T, typename Allocator>
template <bool IsConst>
typename List<T, Allocator>::template Iterator<IsConst>
List<T, Allocator>::Iterator<IsConst>::operator++(int) {
  List<T, Allocator>::Iterator<IsConst> tmp = *this;
  current_ = reinterpret_cast<Node*>(current_->next);
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename List<T, Allocator>::template Iterator<IsConst>&
List<T, Allocator>::Iterator<IsConst>::operator++() {
  current_ = reinterpret_cast<Node*>(current_->next);
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename List<T, Allocator>::template Iterator<IsConst>
List<T, Allocator>::Iterator<IsConst>::operator--(int) {
  List<T, Allocator>::Iterator<IsConst> tmp = *this;
  current_ = reinterpret_cast<Node*>(current_->prev);
  return tmp;
}

template <typename T, typename Allocator>
template <bool IsConst>
typename List<T, Allocator>::template Iterator<IsConst>&
List<T, Allocator>::Iterator<IsConst>::operator--() {
  current_ = reinterpret_cast<Node*>(current_->prev);
  return *this;
}

template <typename T, typename Allocator>
template <bool IsConst>
std::conditional_t<IsConst, const T&, T&>
List<T, Allocator>::Iterator<IsConst>::operator*() {
  return current_->value;
}

template <typename T, typename Allocator>
template <bool IsConst>
std::conditional_t<IsConst, const T*, T*>
List<T, Allocator>::Iterator<IsConst>::operator->() {
  return &(current_->value);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool List<T, Allocator>::Iterator<IsConst>::operator==(
    const Iterator<IsConst>& other) const {
  return (current_ == other.current_);
}

template <typename T, typename Allocator>
template <bool IsConst>
bool List<T, Allocator>::Iterator<IsConst>::operator!=(
    const Iterator<IsConst>& other) const {
  return !(*this == other);
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(const T& value) {
  Node* new_node = node_alloc_traits::allocate(node_alloc_, 1);
  try {
    node_alloc_traits::construct(node_alloc_, new_node, value);
  } catch (...) {
    node_alloc_traits::deallocate(node_alloc_, new_node, 1);
    throw;
  }
  new_node->next = head_;
  new_node->prev = head_->prev;
  head_->prev->next = new_node;
  head_->prev = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
  head_alloc head_node_alloc = head_alloc();
  Node* del_node = reinterpret_cast<Node*>(head_->prev);
  head_->prev = del_node->prev;
  del_node->prev->next = head_;
  node_alloc_traits::destroy(node_alloc_, del_node);
  node_alloc_traits::deallocate(node_alloc_, del_node, 1);
  --size_;
}

template <typename T, typename Allocator>
List<T, Allocator>::List() {
  head_alloc head_node_alloc = head_alloc();
  head_ = head_alloc_traits::allocate(head_node_alloc, 1);
  head_alloc_traits::construct(head_node_alloc, head_, head_);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const size_t& count, const Allocator& alloc) {
  value_alloc_ = alloc;
  node_alloc_ = alloc;
  head_alloc head_node_alloc = head_alloc();
  head_ = head_alloc_traits::allocate(head_node_alloc, 1);
  head_alloc_traits::construct(head_node_alloc, head_, head_);
  if (count != 0) {
    for (size_t indx = 0; indx < count; ++indx) {
      try {
        push_back_none();
      } catch (...) {
        clear();
        head_alloc_traits::destroy(head_node_alloc, head_);
        head_alloc_traits::deallocate(head_node_alloc, head_, 1);
        throw;
      }
    }
  }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const size_t& count, const T& value,
                         const Allocator& alloc) {
  value_alloc_ = alloc;
  node_alloc_ = alloc;
  head_alloc head_node_alloc = head_alloc();
  head_ = head_alloc_traits::allocate(head_node_alloc, 1);
  head_alloc_traits::construct(head_node_alloc, head_, head_);
  if (count != 0) {
    for (size_t indx = 0; indx < count; ++indx) {
      try {
        push_back(value);
      } catch (...) {
        clear();
        throw;
      }
    }
  }
  size_ = count;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>& other) {
  value_alloc_ =
      alloc_traits::select_on_container_copy_construction(other.value_alloc_);
  node_alloc_ =
      alloc_traits::select_on_container_copy_construction(other.node_alloc_);
  head_alloc head_node_alloc = head_alloc();
  head_ = head_alloc_traits::allocate(head_node_alloc, 1);
  head_alloc_traits::construct(head_node_alloc, head_, head_);
  for (auto iter = other.begin(); iter != other.end(); ++iter) {
    try {
      push_back(*iter);
    } catch (...) {
      clear();
      head_alloc_traits::destroy(head_node_alloc, head_);
      head_alloc_traits::deallocate(head_node_alloc, head_, 1);
      throw;
    }
  }
  size_ = other.size();
}

template <typename T, typename Allocator>
List<T, Allocator>::List(std::initializer_list<T> init,
                         const Allocator& alloc) {
  value_alloc_ = alloc;
  node_alloc_ = alloc;
  head_alloc head_node_alloc = head_alloc();
  head_ = head_alloc_traits::allocate(head_node_alloc, 1);
  head_alloc_traits::construct(head_node_alloc, head_, head_);
  for (auto iter = init.begin(); iter != init.end(); ++iter) {
    try {
      push_back(*iter);
    } catch (...) {
      clear();
      throw;
    }
  }
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>& other) {
  if (alloc_traits::propagate_on_container_copy_assignment::value) {
    value_alloc_ = other.value_alloc_;
    node_alloc_ = other.node_alloc_;
  }
  size_t inserted_elements = 0;
  for (auto iter = other.begin(), iter1 = begin(); iter != other.end();
       ++iter, ++iter1) {
    if (inserted_elements < size_) {
      *iter1 = *iter;
      ++inserted_elements;
      continue;
    }
    try {
      push_back(*iter);
      ++inserted_elements;
    } catch (...) {
      clear();
      throw;
    }
  }
  while (size_ > other.size_) {
    pop_back();
  }
  size_ = other.size_;
  return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
  clear();
  head_alloc head_node_alloc = head_alloc();
  head_alloc_traits::destroy(head_node_alloc, head_);
  head_alloc_traits::deallocate(head_node_alloc, head_, 1);
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& value) {
  Node* new_node = node_alloc_traits::allocate(node_alloc_, 1);
  try {
    node_alloc_traits::construct(node_alloc_, new_node, value);
  } catch (...) {
    node_alloc_traits::deallocate(node_alloc_, new_node, 1);
    throw;
  }
  new_node->next = head_->next;
  new_node->prev = head_;
  head_->next->prev = new_node;
  head_->next = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(T&& value) {
  Node* new_node = node_alloc_traits::allocate(node_alloc_, 1);
  try {
    node_alloc_traits::construct(node_alloc_, new_node, value);
  } catch (...) {
    node_alloc_traits::deallocate(node_alloc_, new_node, 1);
    throw;
  }
  new_node->next = head_;
  new_node->prev = head_->prev;
  head_->prev->next = new_node;
  head_->prev = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(T&& value) {
  Node* new_node = node_alloc_traits::allocate(node_alloc_, 1);
  try {
    node_alloc_traits::construct(node_alloc_, new_node, value);
  } catch (...) {
    node_alloc_traits::deallocate(node_alloc_, new_node, 1);
    throw;
  }
  new_node->next = head_->next;
  new_node->prev = head_;
  head_->next->prev = new_node;
  head_->next = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
  head_alloc head_node_alloc = head_alloc();
  Node* del_node = reinterpret_cast<Node*>(head_->next);
  head_->next = del_node->next;
  del_node->next->prev = head_;
  node_alloc_traits::destroy(node_alloc_, del_node);
  node_alloc_traits::deallocate(node_alloc_, del_node, 1);
  --size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::clear() {
  size_t tmpsize = size_;
  for (size_t iter = 0; iter < tmpsize; ++iter) {
    pop_back();
  }
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back_none() {
  Node* new_node = node_alloc_traits::allocate(node_alloc_, 1);
  try {
    node_alloc_traits::construct(node_alloc_, new_node);
  } catch (...) {
    node_alloc_traits::deallocate(node_alloc_, new_node, 1);
    throw;
  }
  new_node->next = head_;
  new_node->prev = head_->prev;
  head_->prev->next = new_node;
  head_->prev = new_node;
  ++size_;
}
