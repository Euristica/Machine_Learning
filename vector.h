#ifndef ML_VECTOR_H
#define ML_VECTOR_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <vector>

template <typename T>
class MathArray;

template <typename T, typename F>
MathArray<T> ZipApply(const MathArray<T>& a, const MathArray<T>& b, const F& f);

template <typename T, typename F>
MathArray<T> Apply(const MathArray<T>& a, const F& f);

template <typename T>
std::ostream& operator<<(std::ostream& os, const MathArray<T> vector);

template <typename T>
class MathArray {
public:
    MathArray() = default;
    MathArray(const std::vector<T>& vector)
        : V{vector}
    {}
    MathArray(uint32_t dim)
        : V(dim)
    {}
    MathArray(uint32_t dim, const T& value)
        : V(dim, value)
    {}
    MathArray(std::initializer_list<T> list)
        : V{list}
    {}

    uint32_t Size() const {
        return static_cast<uint32_t>(V.size());
    };

    MathArray<T> operator+(const MathArray<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
           a += b;
        });
    }

    MathArray<T>& operator+=(const MathArray<T>& other) {
        return ZipApply(other, [](T& a, const T& b){
			a += b;
        });
    }

    MathArray<T> operator-(const MathArray<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
			a -= b;
        });
    }

    MathArray<T>& operator-=(const MathArray<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
			a -= b;
        });
    }

    MathArray<T> operator-() const {
        return ::Apply(*this, [](T& x) {
			x = -x;
        });
    }

    MathArray<T> operator*(const MathArray<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
			a *= b;
        });
    }

    MathArray<T>& operator*=(const MathArray<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
			a *= b;
        });
    }

    MathArray<T> operator/(const MathArray<T>& other) const {
        return ::ZipApply(*this, other, [](T& a, const T& b) {
			a /= b;
        });
    }

    MathArray<T>& operator/=(const MathArray<T>& other) {
        return ZipApply(other, [](T& a, const T& b) {
			a /= b;
        });
    }

    template <typename SCALAR>
    MathArray<T> operator*(const SCALAR& value) const {
        return ::Apply(*this, [&](T& x) {
            x *= value;
        });
    }

    template <typename SCALAR>
    MathArray<T>& operator*=(const SCALAR& value) {
        return Apply([&](T& x) {
            x *= value;
        });
    }

    template <typename SCALAR>
    MathArray<T> operator/(const SCALAR& value) const {
        return ::Apply(*this, [&](T& x) {
            x /= value;
        });
    }

    template <typename SCALAR>
    MathArray<T>& operator/=(const SCALAR& value) {
        return Apply([&](T& x) {
            x /= value;
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
    MathArray<T>& Apply(const F& f);

    template <typename F>
    MathArray<T>& ZipApply(const MathArray<T>& other, const F& f);

private:
    std::vector<T> V;
};

template <typename T>
template <typename F>
MathArray<T>& MathArray<T>::Apply(const F& f) {
	std::for_each(V.begin(), V.end(), f);
    return *this;
}

template <typename T, typename F>
MathArray<T> Apply(const MathArray<T>& a, const F& f) {
    MathArray<T> result = a;
    result.Apply(f);
    return result;
}

template <typename T>
template <typename F>
MathArray<T>& MathArray<T>::ZipApply(const MathArray<T>& other, const F& f) {
    if (Size() != other.Size()) {
        std::stringstream errorDescription;
        errorDescription << "Two vectors must be the same size, but "
                         << Size() << " and " << other.Size()
                         << " were given.";
        throw std::length_error{errorDescription.str()};
    }
    for (uint32_t i = 0; i < Size(); ++i) {
        f(V[i], other.V[i]);
    }
    return *this;
}

template <typename T, typename F>
MathArray<T> ZipApply(const MathArray<T>& a, const MathArray<T>& b, const F& f) {
    MathArray<T> result = a;
    result.ZipApply(b, f);
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const MathArray<T> vector) {
    os << "[";
    for (const T& x : vector) {
        os << x << ", ";
    }
    os << "\b\b]";
    return os;
}

template <typename T, typename SCALAR>
inline MathArray<T> operator*(const SCALAR& value, const MathArray<T>& vector) {
    return vector * value;
}

#endif //ML_VECTOR_H
