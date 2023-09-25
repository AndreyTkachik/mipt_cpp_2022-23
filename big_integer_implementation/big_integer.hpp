#pragma once
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

class BigInt {
 public:
  BigInt();
  BigInt(const std::string& from);
  BigInt(const int64_t& from);
  BigInt(const BigInt& from);
  BigInt& operator=(const BigInt& from);
  ~BigInt();
  BigInt& operator+=(const BigInt& add);
  BigInt& operator-=(const BigInt& dec);
  BigInt& operator*=(const BigInt& mnozh);
  BigInt& operator%=(const BigInt& del);
  BigInt& operator/=(const BigInt& del);
  BigInt operator-() const;
  BigInt& operator++();
  BigInt& operator--();
  BigInt operator++(int);
  BigInt operator--(int);
  friend BigInt operator+(const BigInt& add1, const BigInt& add2);
  friend BigInt operator-(const BigInt& dec1, const BigInt& dec2);
  friend BigInt operator*(const BigInt& mnozh1, const BigInt& mnozh2);
  friend BigInt operator%(const BigInt& delim, const BigInt& del);
  friend BigInt operator/(const BigInt& delim, const BigInt& del);
  bool operator<(const BigInt& num) const;
  bool operator>(const BigInt& num) const;
  bool operator<=(const BigInt& num) const;
  bool operator>=(const BigInt& num) const;
  bool operator==(const BigInt& num) const;
  bool operator!=(const BigInt& num) const;
  friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
  friend std::istream& operator>>(std::istream& is, BigInt& num);

 private:
  std::vector<int64_t> nmb_;
  const int64_t kRazr = 1000000;
  bool sign_ = false;
  void Incr(const BigInt& num);
  void Decr(const BigInt& num);
  friend BigInt Mult(const BigInt& num1, const int& num2, const size_t& sdv);
  friend BigInt DistDB2(BigInt dist);
  void DelAllZero();
};
