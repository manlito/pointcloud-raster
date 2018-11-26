#pragma once
#include <algorithm>
#include <vector>
#include <cstring>

namespace pointcloud_raster::math
{

template <typename DataType, size_t ROWS, size_t COLS>
struct Matrix
{
    typedef DataType type;
    constexpr static size_t size = ROWS * COLS;

    Matrix() {}

    template <typename ...T>
    Matrix(T ...ts) : data{ts...} {}

    Matrix(std::initializer_list<DataType> list)
    {
        std::memcpy(data, list.begin(), ROWS*COLS*sizeof(DataType));
    }

    Matrix(const std::vector<DataType> &copyFromdata)
    {
        std::memcpy(data, copyFromdata.data(), ROWS*COLS*sizeof(DataType));
    }

    DataType const&
    operator[](size_t index) const
    {
        if constexpr (ROWS == 1)
            return data[index];
        else
            return data[index / COLS][index % COLS];
    }

    DataType&
    operator[](size_t index)
    {
        if constexpr (ROWS == 1)
            return data[index];
        else
            return data[index / COLS][index % COLS];
    }

    template <std::size_t INDEX>
    DataType get() const
    {
        if constexpr (ROWS == 1)
            return data[INDEX];
        else
            return data[INDEX / COLS][INDEX % COLS];
    }

    template <size_t ROWS_RIGHT, size_t COLS_RIGHT>
    Matrix<DataType, ROWS, COLS_RIGHT>
    operator*(const Matrix<DataType, ROWS_RIGHT, COLS_RIGHT> &matrix) const
    {
        Matrix<DataType, ROWS, COLS_RIGHT> result;
        for (size_t row = 0; row < ROWS; row++)
            for (size_t col = 0; col < COLS_RIGHT; col++)
            {
                for (size_t cell = 0; cell < COLS; cell++)
                    result.data[row][col] += data[row][cell] * matrix.data[cell][col];
            }
        return result;
    }

    DataType data[ROWS][COLS] {0};
};

}

#define MATRIX_STRUCTURED_BINDING_SPECIALIZATION(MatrixType) \
namespace std { \
template<> \
struct tuple_size<MatrixType> \
    : std::integral_constant<std::size_t, MatrixType::size> {};\
template<std::size_t N> \
struct tuple_element<N, MatrixType> { \
    using type = MatrixType::type; \
}; \
}

