#pragma once

#include <array>

#include "kae/vec.h"

namespace kae {

template <typename TElement, int Tdim>
struct Matrix {
    using Element = TElement;
    static constexpr int dim = Tdim;
    static constexpr int size = dim * dim;

    constexpr Matrix() = default;
    constexpr Matrix(std::array<Element, size> data);
    ~Matrix() = default;
    constexpr Matrix(const Matrix&) = default;
    constexpr Matrix& operator=(const Matrix&) = default;
    constexpr Matrix(Matrix&&) noexcept = default;
    constexpr Matrix& operator=(Matrix&&) noexcept = default;

    template <typename T>
    constexpr explicit Matrix(const Matrix<T, dim>& other);

    template <typename T>
    constexpr Matrix& operator=(const Matrix<T, dim>& other);

    constexpr Element& operator()(size_t row, size_t col);
    constexpr Element operator()(size_t row, size_t col) const;

private:
    std::array<Element, size> data_{};
};


template <typename T>
using Matrix2 = Matrix<T, 2>;
using Matrix2d = Matrix2<double>;
using Matrix2f = Matrix2<float>;
using Matrix2i = Matrix2<int>;
using Matrix2u = Matrix2<unsigned>;

template <typename T>
using Matrix3 = Matrix<T, 3>;
using Matrix3d = Matrix3<double>;
using Matrix3f = Matrix3<float>;
using Matrix3i = Matrix3<int>;
using Matrix3u = Matrix3<unsigned>;

template <typename T>
using Matrix4 = Matrix<T, 4>;
using Matrix4d = Matrix4<double>;
using Matrix4f = Matrix4<float>;
using Matrix4i = Matrix4<int>;
using Matrix4u = Matrix4<unsigned>;


template <typename TElement, int Tdim>
constexpr bool operator==(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);

template <typename TElement, int Tdim>
constexpr bool operator!=(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);

template <typename TElement, int Tdim>
constexpr Matrix<TElement, Tdim>& operator*=(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);

template <typename TElement, int Tdim>
constexpr Matrix<TElement, Tdim> operator*(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs);

template <typename TElement, int Tdim>
constexpr Vec<TElement, Tdim> operator*(const Matrix<TElement, Tdim>& lhs, const Vec<TElement, Tdim>& rhs);


template <typename TElement, int Tdim>
constexpr Matrix<TElement, Tdim>::Matrix(std::array<Element, size> data) :
    data_{std::move(data)}
{}

template <typename TElement, int Tdim>
template <typename T>
constexpr Matrix<TElement, Tdim>::Matrix(const Matrix<T, dim>& other)
{
    *this = other;
}

template <typename TElement, int Tdim>
template <typename T>
constexpr Matrix<TElement, Tdim>& Matrix<TElement, Tdim>::operator=(const Matrix<T, dim>& other)
{
    std::copy(other.data_.begin(), other.data_.end(), data_);
    return *this;
}

template <typename TElement, int Tdim>
constexpr TElement& Matrix<TElement, Tdim>::operator()(size_t row, size_t col)
{
    return data_[row * dim + col];
}

template <typename TElement, int Tdim>
constexpr TElement Matrix<TElement, Tdim>::operator()(size_t row, size_t col) const
{
    return data_[row * dim + col];
}


template <typename TElement, int Tdim>
constexpr bool operator==(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    for (int row = 0; row < Tdim; ++row)
        for (int col = 0; col < Tdim; ++col)
            if (lhs(row, col) != rhs(row, col))
                return false;
    return true;
}

template <typename TElement, int Tdim>
constexpr bool operator!=(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    return !(lhs == rhs);
}

template <typename TElement, int Tdim>
constexpr Matrix<TElement, Tdim>& operator*=(Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    Matrix<TElement, Tdim> lhs_copy = lhs;
    for (int row = 0; row < Tdim; ++row)
        for (int col = 0; col < Tdim; ++col) {
            lhs(row, col) = 0;
            for (int i = 0; i < Tdim; ++i)
                lhs(row, col) += lhs_copy(row, i) * rhs(i, col);
        }
    return lhs;
}

template <typename TElement, int Tdim>
constexpr Matrix<TElement, Tdim> operator*(const Matrix<TElement, Tdim>& lhs, const Matrix<TElement, Tdim>& rhs)
{
    Matrix<TElement, Tdim> matrix = lhs;
    matrix *= rhs;
    return matrix;
}

template <typename TElement, int Tdim>
constexpr Vec<TElement, Tdim> operator*(const Matrix<TElement, Tdim>& lhs, const Vec<TElement, Tdim>& rhs)
{
    Vec<TElement, Tdim> vec;
    for (int row = 0; row < Tdim; ++row)
        for (int col = 0; col < Tdim; ++col)
            vec[row] += lhs(row, col) * rhs[col];
    return vec;
}

}  // namespace kae
