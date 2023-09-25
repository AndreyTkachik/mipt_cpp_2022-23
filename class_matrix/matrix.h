#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix() { matr_.resize(N, std::vector<T>(M)); }

  Matrix(const std::vector<std::vector<T>>& from) { matr_ = from; }

  Matrix(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      std::vector<T> temp;
      for (size_t j = 0; j < M; ++j) {
        temp.push_back(elem);
      }
      matr_.push_back(temp);
    }
  }

  Matrix(const Matrix& from) = default;

  Matrix<N, M, T>& operator=(const Matrix& from) = default;

  ~Matrix() = default;

  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& add) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matr_[i][j] += add(i, j);
      }
    }
    return *this;
  }

  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& dec) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matr_[i][j] -= dec(i, j);
      }
    }
    return *this;
  }

  Matrix<N, M, T>& operator*=(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matr_[i][j] *= elem;
      }
    }
    return *this;
  }

  template <size_t K>
  Matrix<N, K, T> operator*(Matrix<M, K, T> mnoz) {
    Matrix<N, K, T> ans;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < K; ++j) {
        for (size_t iter = 0; iter < M; ++iter) {
          ans(i, j) += matr_[i][iter] * mnoz(iter, j);
        }
      }
    }
    return ans;
  }

  Matrix<N, M, T> operator+(const Matrix<N, M, T>& add1) {
    return (Matrix<N, M, T>(*this) += add1);
  }

  Matrix<N, M, T> operator-(const Matrix<N, M, T>& dec1) {
    return (Matrix<N, M, T>(*this) -= dec1);
  }

  Matrix<N, M, T> operator*(const T& elem) {
    return (Matrix<N, M, T>(*this) *= elem);
  }

  Matrix<M, N, T> Transposed() {
    Matrix<M, N, T> temp;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        temp(j, i) = matr_[i][j];
      }
    }
    return temp;
  }

  T& operator()(const size_t& str, const size_t& stl) {
    return matr_[str][stl];
  }

  const T& operator()(const size_t& str, const size_t& stl) const {
    return matr_[str][stl];
  }

  bool operator==(const Matrix<N, M, T>& mat) const {
    return (matr_ == mat.matr_);
  }

  bool operator!=(const Matrix<N, M, T>& mat) const {
    return !(matr_ == mat.matr_);
  }

 private:
  std::vector<std::vector<T>> matr_;
};

template <size_t N, typename T>
class Matrix<N, N, T> {
 public:
  Matrix() { matr_.resize(N, std::vector<T>(N)); }

  Matrix(const std::vector<std::vector<T>>& from) { matr_ = from; }

  Matrix(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      std::vector<T> temp;
      for (size_t j = 0; j < N; ++j) {
        temp.push_back(elem);
      }
      matr_.push_back(temp);
    }
  }

  Matrix(const Matrix& from) = default;

  Matrix<N, N, T>& operator=(const Matrix& from) = default;

  ~Matrix() = default;

  Matrix<N, N, T>& operator+=(const Matrix<N, N, T>& add) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matr_[i][j] += add.matr_[i][j];
      }
    }
    return *this;
  }

  Matrix<N, N, T>& operator-=(const Matrix<N, N, T>& dec) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matr_[i][j] -= dec.matr_[i][j];
      }
    }
    return *this;
  }

  Matrix<N, N, T>& operator*=(const T& elem) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matr_[i][j] *= elem;
      }
    }
    return *this;
  }

  Matrix<N, N, T> operator*(Matrix<N, N, T> mnoz) {
    Matrix<N, N, T> ans;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        for (size_t iter = 0; iter < N; ++iter) {
          ans(i, j) += matr_[i][iter] * mnoz(iter, j);
        }
      }
    }
    return ans;
  }

  Matrix<N, N, T> operator+(const Matrix<N, N, T>& add1) {
    return (Matrix<N, N, T>(*this) += add1);
  }

  Matrix<N, N, T> operator-(const Matrix<N, N, T>& dec1) {
    return (Matrix<N, N, T>(*this) -= dec1);
  }

  Matrix<N, N, T> operator*(const T& elem) {
    return (Matrix<N, N, T>(*this) *= elem);
  }

  Matrix<N, N, T> Transposed() {
    Matrix<N, N, T> temp;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        temp(j, i) = matr_[i][j];
      }
    }
    return temp;
  }

  T Trace() {
    T res = matr_[0][0];
    for (size_t i = 1; i < N; ++i) {
      res += matr_[i][i];
    }
    return res;
  }

  T& operator()(const size_t& str, const size_t& stl) {
    return matr_[str][stl];
  }

  const T& operator()(const size_t& str, const size_t& stl) const {
    return matr_[str][stl];
  }

  bool operator==(const Matrix<N, N, T>& mat) const {
    return (matr_ == mat.matr_);
  }

  bool operator!=(const Matrix<N, N, T>& mat) const {
    return !(matr_ == mat.matr_);
  }

 private:
  std::vector<std::vector<T>> matr_;
};
