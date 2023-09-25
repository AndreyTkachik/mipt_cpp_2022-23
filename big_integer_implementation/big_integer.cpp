#include "big_integer.hpp"

BigInt::BigInt() { nmb_.push_back(0); }

BigInt::BigInt(const int64_t& from) {
  int64_t temp = from;
  bool fl = false;
  if (from == -1) {
    sign_ = true;
    nmb_.push_back(1);
  }
  if (from < 0) {
    sign_ = true;
    temp = -(temp + 1);
    fl = true;
  }
  if (from == 0) {
    nmb_.push_back(0);
  }
  while (temp != 0) {
    int push = temp % kRazr;
    if (fl) {
      ++push;
      fl = false;
    }
    nmb_.push_back(push);
    temp = temp / kRazr;
  }
}

BigInt::BigInt(const std::string& from) {
  std::string temps = from;
  if (from[0] == '-') {
    sign_ = true;
    temps.erase(0, 1);
  }
  const size_t kDeg = 6;
  size_t iter = 1;
  std::string temp;
  for (size_t i = temps.size() - 1; i < temps.size(); --i) {
    if ((((iter - 1) % kDeg) == 0) && (iter != 1)) {
      nmb_.push_back(std::stoi(temp));
      temp.clear();
    }
    temp = temps[i] + temp;
    ++iter;
  }
  nmb_.push_back(std::stoi(temp));
  DelAllZero();
}

BigInt::BigInt(const BigInt& from) {
  sign_ = from.sign_;
  nmb_ = from.nmb_;
}

BigInt& BigInt::operator=(const BigInt& from) {
  if (nmb_ == from.nmb_) {
    return *this;
  }
  sign_ = from.sign_;
  nmb_ = from.nmb_;
  return *this;
}

BigInt::~BigInt() = default;

BigInt& BigInt::operator+=(const BigInt& add) {
  if (!sign_ && !add.sign_) {
    Incr(add);
    return *this;
  }
  if (!sign_ && add.sign_) {
    BigInt tmpadd = add;
    tmpadd.sign_ = false;
    Decr(tmpadd);
    return *this;
  }
  if (sign_ && !add.sign_) {
    sign_ = false;
    Decr(add);
    sign_ ^= 1;
    return *this;
  }
  sign_ = false;
  BigInt tmpadd = add;
  tmpadd.sign_ = false;
  Incr(tmpadd);
  sign_ ^= 1;
  return *this;
}

BigInt& BigInt::operator-=(const BigInt& dec) {
  if (!sign_ && !dec.sign_) {
    Decr(dec);
    return *this;
  }
  if (!sign_ && dec.sign_) {
    BigInt tmpdec = dec;
    tmpdec.sign_ = false;
    Incr(tmpdec);
    return *this;
  }
  if (sign_ && !dec.sign_) {
    sign_ = false;
    Incr(dec);
    sign_ ^= 1;
    return *this;
  }
  sign_ = false;
  BigInt tmpdec = dec;
  tmpdec.sign_ = false;
  Decr(tmpdec);
  sign_ ^= 1;
  return *this;
}

BigInt BigInt::operator-() const {
  BigInt temp;
  temp = *this;
  temp.sign_ = !(temp.sign_);
  temp.DelAllZero();
  return (temp);
}

BigInt& BigInt::operator++() {
  *this += 1;
  DelAllZero();
  return *this;
}

BigInt& BigInt::operator--() {
  *this -= 1;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt temp = *this;
  *this += 1;
  DelAllZero();
  return temp;
}

BigInt BigInt::operator--(int) {
  BigInt temp = *this;
  *this -= 1;
  return temp;
}

BigInt& BigInt::operator*=(const BigInt& mnozh) {
  bool tmpsign = sign_ ^ mnozh.sign_;
  BigInt tmpmnozh = mnozh;
  BigInt res;
  size_t sdv = 0;
  tmpmnozh.sign_ = false;
  for (size_t i = 0; i < tmpmnozh.nmb_.size(); ++i) {
    res += Mult(*this, tmpmnozh.nmb_[i], sdv);
    ++sdv;
  }
  *this = res;
  sign_ = tmpsign;
  DelAllZero();
  return *this;
}

BigInt& BigInt::operator/=(const BigInt& del) {
  bool newsg = sign_ ^ del.sign_;
  BigInt cdel = del;
  cdel.sign_ = sign_ = false;
  if (*this < del) {
    return *this = 0;
  }
  BigInt el = 0;
  BigInt lf = 0;
  BigInt right = *this;
  BigInt dist = right - lf;
  while (!((*this - (el * cdel) < cdel) && (*this - (el * cdel) >= 0))) {
    el = lf + DistDB2(dist);
    if (lf == 1) {
      break;
    }
    if (el * cdel <= *this) {
      lf = el + 1;
      dist = dist - ((DistDB2(dist)) + 1);
    } else {
      dist = DistDB2(dist);
    }
  }
  if (lf == 0) {
    return *this = lf;
  }
  *this = lf - 1;
  sign_ = newsg;
  return *this;
}

BigInt operator+(const BigInt& add1, const BigInt& add2) {
  BigInt temp = add1;
  temp += add2;
  return temp;
}

BigInt operator-(const BigInt& dec1, const BigInt& dec2) {
  BigInt temp = dec1;
  temp -= dec2;
  return temp;
}

BigInt operator*(const BigInt& mnozh1, const BigInt& mnozh2) {
  BigInt temp = mnozh1;
  temp *= mnozh2;
  return temp;
}

BigInt operator/(const BigInt& delim, const BigInt& del) {
  BigInt temp = delim;
  temp /= del;
  return temp;
}

BigInt operator%(const BigInt& delim, const BigInt& del) {
  BigInt temp = delim;
  temp = (temp - (temp / del) * del);
  return temp;
}

BigInt& BigInt::operator%=(const BigInt& del) {
  *this = *this % del;
  return *this;
}

bool BigInt::operator<(const BigInt& num) const {
  if (!sign_ && num.sign_) {
    return false;
  }
  if (sign_ && !num.sign_) {
    return true;
  }
  if (nmb_.size() != num.nmb_.size()) {
    if (nmb_.size() < num.nmb_.size()) {
      if (!sign_) {
        return true;
      }
    }
    return false;
  }
  const size_t kTempsz = nmb_.size();
  for (size_t i = kTempsz - 1; i < nmb_.size(); --i) {
    if (nmb_[i] == num.nmb_[i]) {
      continue;
    }
    bool res = (nmb_[i] < num.nmb_[i]) ^ (sign_);
    return res;
  }
  return false;
}

bool BigInt::operator==(const BigInt& num) const {
  if ((num.nmb_.back() == 0) && (nmb_.back() == 0)) {
    return true;
  }
  if (num.sign_ != sign_) {
    return false;
  }
  if (num.nmb_.size() != nmb_.size()) {
    return false;
  }
  for (size_t i = 0; i < nmb_.size(); ++i) {
    if (num.nmb_[i] != nmb_[i]) {
      return false;
    }
  }
  return true;
}

bool BigInt::operator>(const BigInt& num) const {
  return !(*this < num) && !(num == *this);
}

bool BigInt::operator<=(const BigInt& num) const { return !(*this > num); }

bool BigInt::operator>=(const BigInt& num) const { return !(*this < num); }

bool BigInt::operator!=(const BigInt& num) const { return !(*this == num); }

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
  if (num.sign_) {
    os << '-';
  }
  const size_t kTempsz = num.nmb_.size();
  const size_t kSix = 6;
  const int64_t kTen = 10;
  for (size_t i = kTempsz - 1; i < num.nmb_.size(); --i) {
    if (i == (kTempsz - 1)) {
      os << num.nmb_[i];
      continue;
    }
    size_t zeros = 0;
    int64_t cnmb = num.nmb_[i];
    for (size_t j = 1; j < kSix; ++j) {
      cnmb /= kTen;
      if (cnmb == 0) {
        ++zeros;
      }
    }
    for (; zeros > 0; --zeros) {
      os << '0';
    }
    os << num.nmb_[i];
  }
  return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
  std::string temp;
  is >> temp;
  BigInt tmp(temp);
  num = tmp;
  return is;
}

void BigInt::Incr(const BigInt& num) {
  size_t newsize = std::max(nmb_.size(), num.nmb_.size());
  nmb_.resize(newsize);
  for (size_t i = 0; i < num.nmb_.size(); ++i) {
    nmb_[i] += num.nmb_[i];
  }
  for (size_t i = 0; i < newsize - 1; ++i) {
    if (nmb_[i] >= kRazr) {
      nmb_[i] -= kRazr;
      ++nmb_[i + 1];
    }
  }
  if (nmb_[newsize - 1] >= kRazr) {
    nmb_[newsize - 1] -= kRazr;
    nmb_.push_back(1);
  }
  DelAllZero();
}

void BigInt::Decr(const BigInt& num) {
  BigInt temp;
  if (*this < num) {
    temp = *this;
    *this = num;
    sign_ = true;
  } else {
    temp = num;
  }
  size_t newsize = temp.nmb_.size();
  for (size_t i = 0; i < newsize; ++i) {
    nmb_[i] -= temp.nmb_[i];
  }
  for (size_t i = 0; i < nmb_.size() - 1; ++i) {
    if (nmb_[i] < 0) {
      nmb_[i] += kRazr;
      --nmb_[i + 1];
    }
  }
  int end = nmb_.size() - 1;
  while ((end > 0) && (nmb_[end] == 0)) {
    nmb_.pop_back();
    --end;
  }
  DelAllZero();
}

BigInt Mult(const BigInt& num1, const int& num2, const size_t& sdv) {
  BigInt res;
  res.nmb_.pop_back();
  for (size_t i = 0; i < sdv; ++i) {
    res.nmb_.push_back(0);
  }
  for (size_t i = 0; i < num1.nmb_.size(); ++i) {
    res.nmb_.push_back(num1.nmb_[i]);
  }
  for (size_t i = 0; i < res.nmb_.size(); ++i) {
    res.nmb_[i] *= num2;
  }
  for (size_t i = 0; i < res.nmb_.size() - 1; ++i) {
    if (res.nmb_[i] >= res.kRazr) {
      int64_t add = res.nmb_[i] / res.kRazr;
      res.nmb_[i] %= res.kRazr;
      res.nmb_[i + 1] += add;
    }
  }
  if (res.nmb_[res.nmb_.size() - 1] >= res.kRazr) {
    int64_t add = res.nmb_[res.nmb_.size() - 1] / res.kRazr;
    res.nmb_[res.nmb_.size() - 1] %= res.kRazr;
    res.nmb_.push_back(add);
  }
  return res;
}

BigInt DistDB2(BigInt dist) {
  int mod = 0;
  const size_t kSz = dist.nmb_.size();
  for (size_t i = kSz - 1; i < kSz; --i) {
    dist.nmb_[i] = dist.nmb_[i] + (mod * dist.kRazr);
    mod = dist.nmb_[i] % 2;
    dist.nmb_[i] /= 2;
  }
  dist.DelAllZero();
  return dist;
}

void BigInt::DelAllZero() {
  for (size_t i = nmb_.size() - 1; i > 0; --i) {
    if (nmb_[i] == 0) {
      nmb_.pop_back();
    } else {
      break;
    }
  }
  if ((nmb_.size() == 1) && (nmb_.back() == 0)) {
    sign_ = false;
  }
}
