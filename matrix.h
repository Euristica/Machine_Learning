#ifndef ML_MATRIX_H
#define ML_MATRIX_H

#include <algorithm>
#include <sstream>
#include <stdint.h>
#include <vector>
<<<<<<< HEAD
=======
#include <assert.h>
>>>>>>> 9ed4a653d27c721b0f53afb26b19a7b6dd115b3c
#include "vector.h"

template <typename T>
class Matrix;

template <typename T, typename F>
Matrix<T> Apply(const Matrix<T>& a, const F& f);

template <typename T, typename F>
Matrix<T> ZipApply(const Matrix<T>& a, const Matrix<T>& b, const F& f);

template <typename T>
class Matrix {
public:
    Matrix() = default;

    Matrix(const std::vector<std::vector<T>>& matrix)
            : M(matrix.size())
    {
        for (uint32_t i = 0; i < matrix[0].size(); ++i)
            M[i] = MathArray<T>{matrix[i]};
    }

    Matrix(const std::initializer_list<std::initializer_list<T>>& matrix) {
        M.reserve(matrix.size());
        for (auto& item : matrix)
            M.push_back(item);
    }

    Matrix(const std::vector<MathArray<T>>& matrix)
            : M{matrix}
    {}

    Matrix(const MathArray<T>& vector)
            : M(vector.Size())
    {
        for (uint32_t i = 0; i < vector.Size(); ++i)
            M[i] = vector[i];

    }

    Matrix(uint32_t rows, uint32_t cols)
            : M(rows, MathArray<T>(cols))
    {}

    uint32_t Height() const {
        return static_cast<uint32_t>(M.size());
    }

    uint32_t Width() const {
        if (!M.size())
            return 0;
        return static_cast<uint32_t>(M[0].Size());
    }

    Matrix<T>& Transpose() {
      std::vector<MathArray<T>> result(Width(), Height());
      for (uint32_t i = 0; i < Height(); ++i) {
        for (uint32_t j = 0; j < Width(); ++j) {
          result[j][i] = M[i][j];
        }
      }
      M = result;
      return *this;
    }

    /*element-wise operations*/
    Matrix<T> operator+(const Matrix<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
           a += b;
        });
    }

    Matrix<T>& operator+=(const Matrix<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
           a += b;
        });
    }

    Matrix<T> operator-(const Matrix<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
            a -= b;
        });
    }

    Matrix<T>& operator-=(const Matrix<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
            a -= b;
        });
    }

    Matrix<T> operator-() const {
        return ::Apply(*this, [](MathArray<T>& a) {
            a = -a;
        });
    };

    Matrix<T> operator*(const Matrix<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
            a *= b;
        });
    }

    Matrix<T>& operator*=(const Matrix<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
            a *= b;
        });
    }

    Matrix<T> operator/(const Matrix<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
            a /= b;
        });
    }

    Matrix<T>& operator/=(const Matrix<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
            a /= b;
        });
    }

    template <typename SCALAR>
    Matrix<T> operator*(const SCALAR& value) const {
        return ::Apply(*this, [&](MathArray<T>& a) {
           a *= value;
        });
    }

    template <typename SCALAR>
    Matrix<T>& operator*=(const SCALAR& value) {
        return Apply([&](MathArray<T>& a) {
            a *= value;
        });
    };

    template <typename SCALAR>
    Matrix<T> operator/(const SCALAR& value) const {
        return ::Apply(*this, [&](MathArray<T>& a) {
            a /= value;
        });
    }

    template <typename SCALAR>
    Matrix<T>& operator/=(const SCALAR& value) {
        return Apply([&](MathArray<T>& a) {
            a /= value;
        });
    }

    /*matrix operations*/
    Matrix<T>& Dot(const Matrix<T>& other) {
        if (Width() != other.Height()) {
            std::stringstream errorDescription;
            errorDescription << "The number of rows of one matrix must be "
                             << "equal to the number of columns of the "
                             << "other matrix, but " << Width() << " and "
                             << other.Height() << " were given.";
            throw std::length_error{errorDescription.str()};
        }
        std::vector<MathArray<T>> result(Height(),  MathArray<T>(other.Width()));
        for (uint32_t i = 0; i < Height(); ++i) {
            for (uint32_t j = 0; j < other.Width(); ++j) {
                for (uint32_t k = 0; k < Width(); ++k) {
                    result[i][j] += M[i][k] * other.M[k][j];
                }
            }
        }
        M = result;
        return *this;
    }

    template <typename F>
    Matrix<T>& Apply(const F& f);

    template <typename F>
    Matrix<T>& ZipApply(const Matrix<T>& other, const F& f);

    MathArray<T>& operator[](uint32_t i) {
        if (i >= Height()) {
            std::stringstream errorDescription;
            errorDescription << "Out of range: trying to access element "
                             << i << " while matrix height is " << Height();
            throw std::out_of_range{errorDescription.str()};
        }
        return M[i];
    }

    const MathArray<T> operator[](uint32_t i) const {
        if (i >= Height()){
            std::stringstream errorDescription;
            errorDescription << "Out of range: trying to access element "
                             << i << " while matrix height is " << Height();
            throw std::out_of_range{errorDescription.str()};
        }
        return M[i];
    };

    Matrix<T> VStack(const Matrix<T>& other) const {
        Matrix<T> result;
        result.M.reserve(M.size() + other.M.size());
        result.M = M;
        result.M.insert(result.M.end(), other.M.begin(), other.M.end());
        return result;
    }

    Matrix<T> HStack(const Matrix<T>& other) const;

private:
    // vector of Rows
    std::vector<MathArray<T>> M;
};

template <typename T>
Matrix<T> Transpose(const Matrix<T>& a) {
  Matrix<T> result(a.Width(), a.Height());
  for (uint32_t i = 0; i < a.Height(); ++i) {
    for (uint32_t j = 0; j < a.Width(); ++j) {
      result[j][i] = a[i][j];
    }
  }
  return result;
}

template <typename T>
template <typename F>
Matrix<T>& Matrix<T>::Apply(const F& f) {
    std::for_each(M.begin(), M.end(), f);
    return *this;
}

template <typename T, typename F>
Matrix<T> Apply(const Matrix<T>& a, const F& f) {
    Matrix<T> result = a;
    result.Apply(f);
    return result;
}

template <typename T>
template <typename F>
Matrix<T>& Matrix<T>::ZipApply(const Matrix<T>& other, const F &f) {
    if ((Height() != other.Height()) || (M[0].Size() != other.Width())) {
        std::stringstream errorDescription;
        errorDescription << "Two matrices must be same size, but "
                         << "Height first matrix: " << Height()
                         << " and Height second matrix: " << other.Height()
                         << "\n Width first matrix: " << Width()
                         << " and Width second matrix: " << other.Width()
                         << " were given.";
        throw std::length_error{errorDescription.str()};
    }
    for (uint32_t i = 0; i < Height(); ++i) {
        for (uint32_t j = 0; j < Width(); ++j) {
            f(M[i][j], other.M[i][j]);
        }
    }
}

template <typename T, typename F>
Matrix<T> ZipApply(const Matrix<T>& a, const Matrix<T>& b, const F& f) {
    Matrix<T> result = a;
    result.ZipApply(b, f);
    return result;
}

#endif //ML_MATRIX_H
