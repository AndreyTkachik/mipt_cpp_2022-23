#include "geometry.hpp"

Vector::Vector() {
  cordx_ = 0;
  cordy_ = 0;
}

Vector::Vector(const int64_t& xcord, const int64_t& ycord) {
  cordx_ = xcord;
  cordy_ = ycord;
}

Vector::Vector(const Vector& from) {
  cordx_ = from.cordx_;
  cordy_ = from.cordy_;
}

Vector::Vector(const Point& strt, const Point& end) {
  cordx_ = end.GetX() - strt.GetX();
  cordy_ = end.GetY() - strt.GetY();
}

int64_t Vector::operator*(const Vector& mnoz) const {
  int64_t res;
  res = cordx_ * mnoz.cordx_;
  res += cordy_ * mnoz.cordy_;
  return res;
}

Vector& Vector::operator=(const Vector& from) {
  cordx_ = from.cordx_;
  cordy_ = from.cordy_;
  return *this;
}

Vector::~Vector() = default;

int64_t Vector::operator^(const Vector& mnoz) const {
  int64_t res;
  res = cordx_ * mnoz.cordy_;
  res -= cordy_ * mnoz.cordx_;
  return res;
}

Vector& Vector::operator+=(const Vector& add) {
  cordx_ = cordx_ + add.cordx_;
  cordy_ = cordy_ + add.cordy_;
  return *this;
}

Vector& Vector::operator-=(const Vector& dec) {
  cordx_ = cordx_ - dec.cordx_;
  cordy_ = cordy_ - dec.cordy_;
  return *this;
}

Vector Vector::operator+(const Vector& add) {
  Vector res = *this;
  res += add;
  return res;
}

Vector Vector::operator-(const Vector& dec) {
  Vector res = *this;
  res -= dec;
  return res;
}

Vector& Vector::operator*=(const int64_t& mnoz) {
  cordx_ *= mnoz;
  cordy_ *= mnoz;
  return *this;
}

Vector Vector::operator*(const int64_t& mnoz) {
  Vector res = *this;
  res *= mnoz;
  return res;
}

Vector& Vector::operator-() {
  cordx_ = -cordx_;
  cordy_ = -cordy_;
  return *this;
}

int64_t& Vector::GetX() { return cordx_; }

const int64_t& Vector::GetX() const { return cordx_; }

int64_t& Vector::GetY() { return cordy_; }

const int64_t& Vector::GetY() const { return cordy_; }

Point::Point() {
  cordx_ = 0;
  cordy_ = 0;
}

Point::Point(const int64_t& xcord, const int64_t& ycord) {
  cordx_ = xcord;
  cordy_ = ycord;
}

Point::Point(const Point& from) {
  cordx_ = from.cordx_;
  cordy_ = from.cordy_;
}

Point& Point::operator=(const Point& from) {
  cordx_ = from.cordx_;
  cordy_ = from.cordy_;
  return *this;
}

Point::~Point() = default;

void Point::Move(const Vector& vec) {
  cordx_ += vec.GetX();
  cordy_ += vec.GetY();
}

bool Point::ContainsPoint(const Point& pnt) {
  return !((cordx_ != pnt.cordx_) || (cordy_ != pnt.cordy_));
}

bool Point::CrossSegment(const Segment& sgm) {
  Vector to_a(sgm.GetA(), *this);
  Vector from_a(*this, sgm.GetB());
  return (((to_a ^ from_a) == 0) && (to_a * from_a >= 0));
}

IShape* Point::Clone() { return new Point(*this); }

Vector Point::operator-(const Point& pnt) {
  Vector res(pnt, *this);
  return res;
}

int64_t& Point::GetX() { return cordx_; }

const int64_t& Point::GetX() const { return cordx_; }

int64_t& Point::GetY() { return cordy_; }

const int64_t& Point::GetY() const { return cordy_; }

Segment::Segment() = default;

Segment::Segment(const Point& apnt, const Point& bpnt) {
  pntA_ = apnt;
  pntB_ = bpnt;
}

Segment::Segment(const Segment& from) {
  pntA_ = from.pntA_;
  pntB_ = from.pntB_;
}

Segment& Segment::operator=(const Segment& from) {
  pntA_ = from.GetA();
  pntB_ = from.GetB();
  return *this;
}

Segment::~Segment() = default;

void Segment::Move(const Vector& vec) {
  pntA_.Move(vec);
  pntB_.Move(vec);
}

bool Segment::ContainsPoint(const Point& pnt) {
  Point tmp = pnt;
  return tmp.CrossSegment(*this);
}

bool Segment::CrossProections(const Segment& proec1, const Segment& proec2) {
  Segment tmp1 = proec1;
  Segment tmp2 = proec2;
  return tmp1.ContainsPoint(tmp2.GetA()) || tmp1.ContainsPoint(tmp2.GetB()) ||
         tmp2.ContainsPoint(tmp1.GetA()) || tmp2.ContainsPoint(tmp1.GetB());
}

bool Segment::CrossSegment(const Segment& sgm) {
  Segment temp = sgm;
  Line ln1(pntA_, pntB_);
  Line ln2(sgm.GetA(), sgm.GetB());
  if (ln1.CrossSegment(temp) && ln2.CrossSegment(*this)) {
    Point thisax(GetA().GetX(), 0);
    Point thisbx(GetB().GetX(), 0);
    Point thisay(0, GetA().GetY());
    Point thisby(0, GetB().GetY());
    Point sgmax(sgm.GetA().GetX(), 0);
    Point sgmbx(sgm.GetB().GetX(), 0);
    Point sgmay(0, sgm.GetA().GetY());
    Point sgmby(0, sgm.GetB().GetY());
    Segment thisx(thisax, thisbx);
    Segment thisy(thisay, thisby);
    Segment sgmx(sgmax, sgmbx);
    Segment sgmy(sgmay, sgmby);
    if (CrossProections(thisx, sgmx) && CrossProections(thisy, sgmy)) {
      return true;
    }
  }
  return ((temp.GetA().CrossSegment(*this)) &&
          (temp.GetB().CrossSegment(*this)) && (pntA_.CrossSegment(sgm)) &&
          (pntB_.CrossSegment(sgm)));
}

IShape* Segment::Clone() { return new Segment(*this); }

Point& Segment::GetA() { return pntA_; }

const Point& Segment::GetA() const { return pntA_; }

Point& Segment::GetB() { return pntB_; }

const Point& Segment::GetB() const { return pntB_; }

Line::Line() = default;

Line::Line(const Point& pnta, const Point& pntb) {
  lineA_ = pnta;
  lineB_ = pntb;
}

Line::Line(const Line& from) {
  lineA_ = from.lineA_;
  lineB_ = from.lineB_;
}

Line& Line::operator=(const Line& from) {
  lineA_ = from.lineA_;
  lineB_ = from.lineB_;
  return *this;
}

Line::~Line() = default;

void Line::Move(const Vector& vec) {
  lineA_.Move(vec);
  lineB_.Move(vec);
}

bool Line::ContainsPoint(const Point& pnt) {
  return (((lineA_.GetX() - lineB_.GetX()) * pnt.GetY() +
           (lineB_.GetY() - lineA_.GetY()) * pnt.GetX() +
           (lineB_.GetX() * lineA_.GetY() - lineB_.GetY() * lineA_.GetX())) ==
          0);
}

bool Line::CrossSegment(const Segment& sgm) {
  Vector vecseg1(lineA_, sgm.GetA());
  Vector vecseg2(lineA_, sgm.GetB());
  Vector vecline(lineA_, lineB_);
  return (((vecline ^ vecseg1) * (vecline ^ vecseg2)) <= 0);
}

IShape* Line::Clone() { return new Line(*this); }

int64_t Line::GetA() {
  int64_t res = (lineB_.GetY() - lineA_.GetY());
  return res;
}

int64_t Line::GetB() {
  int64_t res = (lineA_.GetX() - lineB_.GetX());
  return res;
}

int64_t Line::GetC() {
  int64_t res = (lineB_.GetX() * lineA_.GetY() - lineB_.GetY() * lineA_.GetX());
  return res;
}

Ray::Ray() = default;

Ray::Ray(const Point& pnta, const Point& pntb) {
  rayA_ = pnta;
  rayB_ = pntb;
  Vector temp(rayA_, rayB_);
  naprav_ = temp;
}

Ray::Ray(const Ray& from) {
  rayA_ = from.rayA_;
  rayB_ = from.rayB_;
  naprav_ = from.naprav_;
}

Ray& Ray::operator=(const Ray& from) {
  rayA_ = from.rayA_;
  rayB_ = from.rayB_;
  naprav_ = from.naprav_;
  return *this;
}

Ray::~Ray() = default;

void Ray::Move(const Vector& vec) {
  rayA_.Move(vec);
  rayB_.Move(vec);
  Vector temp(rayA_, rayB_);
  naprav_ = temp;
}

bool Ray::ContainsPoint(const Point& pnt) {
  Vector to_a(rayA_, pnt);
  return (((to_a ^ naprav_) == 0) && ((to_a * naprav_) >= 0));
}

bool Ray::CrossSegment(const Segment& sgm) {
  Vector to_a(rayA_, sgm.GetA());
  Vector to_b(rayA_, sgm.GetB());
  if ((to_a ^ to_b) == 0) {
    if ((to_a ^ naprav_) == 0) {
      if (((to_a * naprav_) >= 0) || ((to_b * naprav_) >= 0)) {
        return true;
      }
      if ((to_a * to_b) <= 0) {
        return true;
      }
    }
  }
  if (((to_a ^ naprav_) == 0) && ((to_a * naprav_) >= 0)) {
    return true;
  }
  if (((to_b ^ naprav_) == 0) && ((to_b * naprav_) >= 0)) {
    return true;
  }
  if (((to_a ^ naprav_) != 0) && ((to_b ^ naprav_) != 0)) {
    if (((to_a ^ naprav_) >= 0) && ((to_b * naprav_) >= 0) &&
        ((to_a ^ to_b) >= 0)) {
      return true;
    }
    if (((to_a ^ naprav_) <= 0) && ((to_b * naprav_) <= 0) &&
        ((to_a ^ to_b) <= 0)) {
      return true;
    }
  }
  return false;
}

IShape* Ray::Clone() { return new Ray(*this); }

Point& Ray::GetA() { return rayA_; }

Vector& Ray::GetVector() { return naprav_; }

Circle::Circle() = default;

Circle::Circle(const Point& pnt, const int64_t& rdx) {
  cntr_ = pnt;
  rad_ = rdx;
}

Circle::Circle(const Circle& from) {
  cntr_ = from.cntr_;
  rad_ = from.rad_;
}

Circle& Circle::operator=(const Circle& from) {
  cntr_ = from.cntr_;
  rad_ = from.rad_;
  return *this;
}

Circle::~Circle() = default;

void Circle::Move(const Vector& vec) { cntr_.Move(vec); }

bool Circle::ContainsPoint(const Point& pnt) {
  int64_t square1 = (pnt.GetX() - cntr_.GetX()) * (pnt.GetX() - cntr_.GetX());
  int64_t square2 = (pnt.GetY() - cntr_.GetY()) * (pnt.GetY() - cntr_.GetY());
  return ((square1 + square2) <= (rad_ * rad_));
}

int64_t Circle::Distance(const Point& pnt1, const Point& pnt2) {
  int64_t res =
      std::abs((pnt2.GetY() - pnt1.GetY()) * cntr_.GetX() +
               (pnt1.GetX() - pnt2.GetX()) * cntr_.GetY() +
               (pnt2.GetX() * pnt1.GetY() - pnt1.GetX() * pnt2.GetY()));
  int64_t znam1 = (pnt2.GetY() - pnt1.GetY()) * (pnt2.GetY() - pnt1.GetY());
  int64_t znam2 = (pnt2.GetX() - pnt1.GetX()) * (pnt2.GetX() - pnt1.GetX());
  znam1 += znam2;
  res /= znam1;
  return res;
}

bool Circle::CrossSegment(const Segment& sgm) {
  int64_t square11 =
      (sgm.GetA().GetX() - cntr_.GetX()) * (sgm.GetA().GetX() - cntr_.GetX());
  int64_t square21 =
      (sgm.GetA().GetY() - cntr_.GetY()) * (sgm.GetA().GetY() - cntr_.GetY());
  int64_t square12 =
      (sgm.GetB().GetX() - cntr_.GetX()) * (sgm.GetB().GetX() - cntr_.GetX());
  int64_t square22 =
      (sgm.GetB().GetY() - cntr_.GetY()) * (sgm.GetB().GetY() - cntr_.GetY());
  if (((square11 + square21) < (rad_ * rad_)) &&
      ((square12 + square22) < (rad_ * rad_))) {
    return false;
  }
  if ((!ContainsPoint(sgm.GetA()) && ContainsPoint(sgm.GetB())) ||
      (ContainsPoint(sgm.GetA()) && !ContainsPoint(sgm.GetB()))) {
    return true;
  }
  if (Distance(sgm.GetA(), sgm.GetB()) < (rad_ * rad_)) {
    Vector to_a(sgm.GetA(), cntr_);
    Vector to_b(sgm.GetB(), cntr_);
    Vector segb(sgm.GetA(), sgm.GetB());
    Vector sega(sgm.GetB(), sgm.GetA());
    if (((to_a * segb) > 0) && ((to_b * sega) > 0)) {
      return true;
    }
  }
  return false;
}

IShape* Circle::Clone() { return new Circle(*this); }

Point& Circle::GetCentre() { return cntr_; }

const Point& Circle::GetCentre() const { return cntr_; }

int64_t& Circle::GetRadius() { return rad_; }

const int64_t& Circle::GetRadius() const { return rad_; }
