#include <cmath>

class Point;

class Segment;

class Line;

class Vector {
 public:
  Vector();
  Vector(const int64_t& xcord, const int64_t& ycord);
  Vector(const Vector& from);
  Vector& operator=(const Vector& from);
  ~Vector();
  Vector(const Point& strt, const Point& end);
  int64_t operator*(const Vector& mnoz) const;
  int64_t operator^(const Vector& mnoz) const;
  Vector& operator+=(const Vector& add);
  Vector& operator-=(const Vector& dec);
  Vector operator+(const Vector& add);
  Vector operator-(const Vector& dec);
  Vector& operator*=(const int64_t& mnoz);
  Vector operator*(const int64_t& mnoz);
  Vector& operator-();
  int64_t& GetX();
  const int64_t& GetX() const;
  int64_t& GetY();
  const int64_t& GetY() const;

 private:
  int64_t cordx_;
  int64_t cordy_;
};

class IShape {
 public:
  virtual void Move(const Vector& vec) = 0;
  virtual bool ContainsPoint(const Point& pnt) = 0;
  virtual bool CrossSegment(const Segment& sgm) = 0;
  virtual IShape* Clone() = 0;
  virtual ~IShape(){};
};

class Point : public IShape {
 public:
  Point();
  Point(const int64_t& xcord, const int64_t& ycord);
  Point(const Point& from);
  Point& operator=(const Point& from);
  ~Point();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) override;
  bool CrossSegment(const Segment& sgm) override;
  IShape* Clone() override;
  Vector operator-(const Point& pnt);
  int64_t& GetX();
  const int64_t& GetX() const;
  int64_t& GetY();
  const int64_t& GetY() const;

 private:
  int64_t cordx_;
  int64_t cordy_;
};

class Segment : public IShape {
 public:
  Segment();
  Segment(const Point& apnt, const Point& bpnt);
  Segment(const Segment& from);
  Segment& operator=(const Segment& from);
  ~Segment();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) override;
  bool CrossSegment(const Segment& sgm) override;
  IShape* Clone() override;
  Point& GetA();
  const Point& GetA() const;
  Point& GetB();
  const Point& GetB() const;

 private:
  static bool CrossProections(const Segment& proec1, const Segment& proec2);
  Point pntA_;
  Point pntB_;
};

class Line : public IShape {
 public:
  Line();
  Line(const Point& pnta, const Point& pntb);
  Line(const Line& from);
  Line& operator=(const Line& from);
  ~Line();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) override;
  bool CrossSegment(const Segment& sgm) override;
  IShape* Clone() override;
  int64_t GetA();
  int64_t GetB();
  int64_t GetC();

 private:
  Point lineA_;
  Point lineB_;
};

class Ray : public IShape {
 public:
  Ray();
  Ray(const Point& pnta, const Point& pntb);
  Ray(const Ray& from);
  Ray& operator=(const Ray& from);
  ~Ray();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) override;
  bool CrossSegment(const Segment& sgm) override;
  IShape* Clone() override;
  Point& GetA();
  Vector& GetVector();

 private:
  Point rayA_;
  Point rayB_;
  Vector naprav_;
};

class Circle : public IShape {
 public:
  Circle();
  Circle(const Point& pnt, const int64_t& rdx);
  Circle(const Circle& from);
  Circle& operator=(const Circle& from);
  ~Circle();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) override;
  bool CrossSegment(const Segment& sgm) override;
  IShape* Clone() override;
  Point& GetCentre();
  const Point& GetCentre() const;
  int64_t& GetRadius();
  const int64_t& GetRadius() const;

 private:
  Point cntr_;
  int64_t rad_;
  int64_t Distance(const Point& pnt1, const Point& pnt2);
};
