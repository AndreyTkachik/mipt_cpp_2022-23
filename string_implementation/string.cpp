#include "string.hpp"

void String::MkNewSrtwthCap() {
  if (size_ > (capacity_ - 1)) {
    capacity_++;
    while ((capacity_ - 1) < size_) {
      --capacity_;
      capacity_ *= 2;
      ++capacity_;
    }
    char* strng1 = new char[size_ + 1]();
    this->CharCopy(strng1);
    delete[] strng_;
    strng_ = strng1;
  }
}

void String::CapacityChange() {
  --capacity_;
  capacity_ *= 2;
  ++capacity_;
  char* temp = new char[capacity_];
  memcpy(temp, strng_, size_);
  temp[size_] = '\0';
  delete[] strng_;
  strng_ = temp;
}

void String::CharCopy(char* to1) {
  for (size_t i = 0; i < size_; i++) {
    if (strng_[i] == '\0') {
      break;
    }
    to1[i] = strng_[i];
  }
  to1[size_] = '\0';
}

String::String() { strng_ = nullptr; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_ - 1; }

char* String::Data() { return strng_; }

const char* String::Data() const { return strng_; }

bool String::Empty() const { return size_ == 0; }

String::String(const size_t& size, const char& character) {
  size_ = size;
  capacity_ += size_;
  strng_ = new char[capacity_];
  for (size_t i = 0; i < size_; i++) {
    strng_[i] = character;
  }
  strng_[size_] = '\0';
}

String::String(const char* character) {
  size_ = strlen(character);
  capacity_ += size_;
  strng_ = new char[capacity_];
  for (size_t i = 0; i < size_; i++) {
    strng_[i] = character[i];
  }
  strng_[size_] = '\0';
}

String::String(const String& from) {
  size_ = from.Size();
  capacity_ += size_;
  strng_ = new char[capacity_];
  for (size_t i = 0; i < size_; i++) {
    strng_[i] = *(from.Data() + i);
  }
  strng_[size_] = '\0';
}

String& String::operator=(const String& from) {
  if (strng_ == from.Data()) {
    return *this;
  }
  size_ = from.Size();
  capacity_ = size_ + 1;
  delete[] strng_;
  strng_ = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    strng_[i] = *(from.Data() + i);
  }
  strng_[size_] = '\0';
  return *this;
}

String::~String() { delete[] strng_; }

void String::Clear() { size_ = 0; }

void String::PushBack(const char& character) {
  ++size_;
  if (strng_ == nullptr) {
    strng_ = new char[size_ + 1];
    if (character == '\0') {
      strng_[0] = '\0';
      Clear();
      capacity_ = 1;
      return;
    }
    ++capacity_;
    strng_[0] = character;
    strng_[1] = '\0';
    return;
  }
  if (size_ > (capacity_ - 1)) {
    CapacityChange();
    strng_[size_ - 1] = character;
    strng_[size_] = '\0';
  } else {
    if (character == '\0') {
      strng_[0] = '\0';
      Clear();
      capacity_ = 1;
      return;
    }
    strng_[size_ - 1] = character;
    strng_[size_] = '\0';
  }
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  --size_;
  strng_[size_] = '\0';
}

void String::Resize(const size_t& new_size) {
  size_ = new_size;
  MkNewSrtwthCap();
}

void String::Resize(const size_t& new_size, const char& character) {
  if (new_size > (capacity_ - 1)) {
    size_t dist = size_;
    size_ = new_size;
    MkNewSrtwthCap();
    for (size_t i = dist; i < size_; ++i) {
      strng_[i] = character;
    }
  } else {
    size_ = new_size;
  }
}

void String::Reserve(const size_t& new_cap) {
  if (new_cap > (capacity_ - 1)) {
    capacity_ = new_cap + 1;
    char* temp = new char[capacity_];
    memcpy(temp, strng_, size_);
    delete[] strng_;
    strng_ = temp;
    strng_[size_] = '\0';
  }
}

void String::ShrinkToFit() {
  if (size_ < capacity_) {
    capacity_ = size_;
    capacity_++;
  }
}

void String::Swap(String& other) {
  std::swap(other.strng_, strng_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

char& String::operator[](const size_t& numb) { return *(strng_ + numb); }

const char& String::operator[](const size_t& numb) const {
  return *(strng_ + numb);
}

char& String::Front() { return *strng_; }

char& String::Back() { return *(strng_ + size_ - 1); }

const char& String::Front() const { return *strng_; }

const char& String::Back() const { return *(strng_ + size_ - 1); }

bool String::operator<(const String& str) const {
  if (size_ < str.Size()) {
    return true;
  }
  if (size_ > str.Size()) {
    return false;
  }
  for (size_t i = 0; i < size_; ++i) {
    if (strng_[i] == str.strng_[i]) {
      continue;
    }
    return (strng_[i] < str.strng_[i]);
  }
  return false;
}

bool String::operator==(const String& str) const {
  if (size_ != str.Size()) {
    return false;
  }
  for (size_t i = 0; i < size_; ++i) {
    if (str.strng_[i] != strng_[i]) {
      return false;
    }
  }
  return true;
}

bool String::operator>(const String& str) const {
  return !(*this < str) && !(str == *this);
}

bool String::operator<=(const String& str) const { return !(*this > str); }

bool String::operator>=(const String& str) const { return !(*this < str); }

bool String::operator!=(const String& str) const { return !(*this == str); }

String operator+(const String& str1, const String& str2) {
  String res(str1);
  res += str2;
  return res;
}

String& String::operator+=(const String& str) {
  if ((capacity_ - 1) < (size_ + str.Size())) {
    Reserve((size_ + str.Size()) * 2);
  }
  size_t old = size_;
  size_ += str.Size();
  for (size_t i = 0; i < str.Size(); i++) {
    strng_[old + i] = str[i];
  }
  strng_[size_] = '\0';
  return *this;
}

String& String::operator*=(const size_t& n) {
  if (n == 0) {
    strng_ = nullptr;
    return *this;
  }
  char* temp = new char[size_ + 1];
  memcpy(temp, strng_, size_);
  temp[size_] = '\0';
  for (size_t i = 0; i < (n - 1); ++i) {
    *this += temp;
  }
  delete[] temp;
  return *this;
}

String operator*(const String& str, const size_t& n) {
  if (n == 0) {
    String res;
    return res;
  }
  String res(str);
  res *= n;
  return res;
}

std::ostream& operator<<(std::ostream& osi, const String& str) {
  size_t iter = 0;
  while (str[iter] != '\0') {
    osi << str[iter];
    ++iter;
  }
  return osi;
}

std::istream& operator>>(std::istream& isi, String& str) {
  const size_t kKtmp = 100000;
  char* temp = new char[kKtmp];
  isi >> temp;
  str = temp;
  delete[] temp;
  return isi;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> res;
  String temp;
  for (size_t i = 0; i < size_; ++i) {
    if (strng_[i] == delim[0]) {
      bool cond = true;
      for (size_t j = 0; j < delim.Size(); ++j) {
        if (strng_[i + j] != delim[j]) {
          cond = false;
        }
      }
      if (cond) {
        res.push_back(temp);
        temp.Clear();
        i += delim.Size();
        if (strng_[i] == delim[0]) {
          String null;
          res.push_back(null);
          temp.Clear();
          i += (delim.Size() - 1);
          continue;
        }
      }
    }
    temp.PushBack(strng_[i]);
  }
  res.push_back(temp);
  return res;
}

String String::Join(const std::vector<String>& strings) const {
  if ((strings.empty()) || (strings.front().strng_[0] == '\0')) {
    String result;
    return result;
  }
  String result(strings.front());
  for (size_t i = 1; i < strings.size(); ++i) {
    String temp = strings[i];
    result += *this;
    result += temp;
  }
  return result;
}
