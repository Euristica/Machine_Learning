#ifndef ML_MATRIX_H
#define ML_MATRIX_H

#include <vector>
#include <stdint.h>
#include "vector.h"

template <typename T>
class Matrix {
public:
    Matrix() = default;
    Matrix(const std::vector<std::vector<T>>& matrix)
	    :M{matrix.size()} 
    {
        for (uint32_t i = 0; i < matrix[0].size(); i++)
            M[i] = Vector<T>{matrix[i]};
    }
    Matrix(const std::initializer_list<std::initializer_list<T>>& matrix) {
        M.reserve(matrix.size());
        for (auto& item : matrix)
            M.push_back(item);
    }
    Matrix(const std::vector<Vector<T>>& matrix)
            : M{matrix}
    {}
    Matrix(const Vector<T>& vector)
            :M{1, vector}
    {}
    Matrix(uint32_t rows, uint32_t cols)
            :M{rows, Vector<T>{cols}}
    {}
	
    uint32_t Height() const {
        return static_cast<uint32_t>(M.size());
    }

    uint32_t Width() const {
        if (!M.size())
            return 0;
        return static_cast<uint32_t>(M[0].size());
    }

    Matrix<T> Transpose() const;
    Matrix<T>& InplaceTranspose();

    // element-wise operations
    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T>& operator+=(const Matrix<T>& other);

    Matrix<T> operator-(const Matrix<T>& other) const;
    Matrix<T>& operator-=(const Matrix<T>& other);
    Matrix<T>& operator-();

    Matrix<T> operator*(const Matrix<T>& other) const;
    Matrix<T>& operator*=(const Matrix<T>& other);

    Matrix<T> operator/(const Matrix<T>& other) const;
    Matrix<T>& operator/=(const Matrix<T>& other);

    Matrix<T> operator*(const T& value) const;
    Matrix<T>& operator*=(const T& value);
    friend operator*(const T& value, const Matrix<T>& matrix);

    Matrix<T> operator/(const T& value) const;
    Matrix<T>& operator/=(const T& value);
    friend operator/(const T& value, const Matrix<T>& matrix);

    // matrix operations
    Matrix<T> Dot(const Matrix<T>& other) const;
    Matrix<T>& InplaceDot(const Matrix<T>& other);

    template <typename F>
    void Apply(const F& function);

    Vector<T>& operator[](uint32_t i);
    const Vector<T> operator[](uint32_t i) const;

    Matrix<T> VStack(const Matrix<T>& other) const;
    Matrix<T>& InplaceVStack(const Matrix<T>& other);

    Matrix<T> HStack(const Matrix<T>& other) const;
    Matrix<T>& InplaceHStack(const Matrix<T>& other);

private:
    // vector of Rows
    std::vector<Vector<T>> M;
};

#endif //ML_MATRIX_H
