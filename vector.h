#ifndef ML_VECTOR_H
#define ML_VECTOR_H

#include <iostream>
#include <sstream>
#include <stdint.h>
#include <vector>

template <typename T>
class Vector;

template <typename T, typename F>
Vector<T> ZipApply(const Vector<T>& a, const Vector<T>& b, const F& f);

template <typename T, typename F>
Vector<T> Apply(const Vector<T>& a, const F& f);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T> vector);

template <typename T>
class Vector {
public:
    Vector() = default;
    Vector(const std::vector<T>& vector)
        : V{vector}
    {}
    Vector(uint32_t dim)
        : V{dim}
    {}
    Vector(uint32_t dim, const T& value)
        : V{dim, value}
    {}
    Vector(std::initializer_list<T> list)
        : V{list}
    {}

    uint32_t Size() const {
        return static_cast<uint32_t>(V.size());
    };

    Vector<T> operator+(const Vector<T>& other) const {
        return ::ZipApply(*this, other, [](const T& a, const T& b) {
           return a + b;
        });
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        return ZipApply(other, [](const T& a, const T& b){
            return a + b;
        });
    }

    Vector<T> operator-(const Vector<T>& other) const {
        return ::ZipApply(*this, other, [](const T& a, const T& b) {
            return a - b;
        });
    }

    Vector<T>& operator-=(const Vector<T>& other) {
        return ZipApply(other, [](const T& a, const T& b) {
            return a - b;
        });
    }

    Vector<T> operator-() const {
        return ::Apply(*this, [](const T& a) {
            return -a;
        });
    }

    Vector<T> operator*(const Vector<T>& other) const {
        return ::ZipApply(*this, other, [](const T& a, const T& b) {
            return a * b;
        });
    }

    Vector<T>& operator*=(const Vector<T>& other) {
        return ZipApply(other, [](const T& a, const T& b) {
            return a * b;
        });
    }

    Vector<T> operator/(const Vector<T>& other) const {
        return ::ZipApply(*this, other, [](const T& a, const T& b) {
            return a / b;
        });
    }

    Vector<T>& operator/=(const Vector<T>& other) {
        return ZipApply(other, [](const T& a, const T& b) {
            return a / b;
        });
    }

    template <typename SCALAR>
    Vector<T> operator*(const SCALAR& value) const {
        return ::Apply(*this, [&](const T& a) {
            return a * value;
        });
    }

    template <typename SCALAR>
    Vector<T>& operator*=(const SCALAR& value) {
        return Apply([&](const T& a) {
            return a * value;
        });
    }

    template <typename SCALAR>
    Vector<T> operator/(const SCALAR& value) const {
        return ::Apply(*this, [&](const T& a) {
            return a / value;
        });
    }

    template <typename SCALAR>
    Vector<T>& operator/=(const SCALAR& value) {
        return Apply([&](const T& a) {
            return a / value;
        });
    }

    typename std::vector<T>::iterator begin() {
        return V.begin();
    }

    typename std::vector<T>::iterator end() {
        return V.end();
    }

    typename std::vector<T>::const_iterator begin() const {
        return V.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return V.end();
    }

    T& operator[](uint32_t i) {
        if (i >= Size()) {
            std::stringstream errorDescription;
            errorDescription << "Out of range: trying to access element "
                             << i << " while vector's dimension is " << Size();
            throw std::out_of_range{errorDescription.str()};
        }
        return V[i];
    }

    const T& operator[](uint32_t i) const {
        if (i >= Size()) {
            std::stringstream errorDescription;
            errorDescription << "Out of range: trying to access element "
                             << i << " while vector's dimension is " << Size();
            throw std::out_of_range{errorDescription.str()};
        }
        return V[i];
    }

    template <typename F>
    Vector<T>& Apply(const F& f);

    template <typename F>
    Vector<T>& ZipApply(const Vector<T>& other, const F& f);

private:
    std::vector<T> V;
};

template <typename T>
template <typename F>
Vector<T>& Vector<T>::Apply(const F& f) {
    for (uint32_t i = 0; i < Size(); ++i) {
        V[i] = f(V[i]);
    }
    return *this;
}

template <typename T, typename F>
Vector<T> Apply(const Vector<T>& a, const F& f) {
    Vector<T> result = a;
    result.Apply(f);
    return result;
}

template <typename T>
template <typename F>
Vector<T>& Vector<T>::ZipApply(const Vector<T>& other, const F& f) {
    if (Size() != other.Size()) {
        std::stringstream errorDescription;
        errorDescription << "Two vectors must be the same size, but "
                         << Size() << " and " << other.Size()
                         << " were given.";
        throw std::length_error{errorDescription.str()};
    }
    for (uint32_t i = 0; i < Size(); ++i) {
        V[i] = f(V[i], other.V[i]);
    }
    return *this;
}

template <typename T, typename F>
Vector<T> ZipApply(const Vector<T>& a, const Vector<T>& b, const F& f) {
    Vector<T> result = a;
    result.ZipApply(b, f);
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T> vector) {
    os << "[";
    for (const T& x : vector) {
        os << x << ", ";
    }
    os << "\b\b]";
    return os;
}

template <typename T, typename SCALAR>
inline Vector<T> operator*(const SCALAR& value, const Vector<T>& vector) {
    return vector * value;
}

#endif //ML_VECTOR_H
