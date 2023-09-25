#pragma once

#include <string.h>

#include <iostream>
#include <vector>

class String {
 public:
  String();
  String(const size_t& size, const char& character);
  String(const char* character);
  String(const String& from);
  String& operator=(const String& from);
  ~String();
  void Clear();
  void PushBack(const char& character);
  void PopBack();
  void Resize(const size_t& new_size);
  void Resize(const size_t& new_size, const char& character);
  void Reserve(const size_t& new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  char& operator[](const size_t& numb);
  const char& operator[](const size_t& numb) const;
  char& Front();
  char& Back();
  const char& Front() const;
  const char& Back() const;
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  char* Data();
  const char* Data() const;
  bool operator<(const String& str) const;
  bool operator>(const String& str) const;
  bool operator<=(const String& str) const;
  bool operator>=(const String& str) const;
  bool operator==(const String& str) const;
  bool operator!=(const String& str) const;
  String& operator+=(const String& str);
  String& operator*=(const size_t& n);
  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings) const;

 private:
  char* strng_;
  size_t size_ = 0;
  size_t capacity_ = 1;
  void CapacityChange();
  void CharCopy(char* to1);
  void MkNewSrtwthCap();
};

String operator+(const String& str1, const String& str2);
String operator*(const String& str, const size_t& n);
std::ostream& operator<<(std::ostream& osi, const String& str);
std::istream& operator>>(std::istream& isi, String& str);
