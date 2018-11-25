#pragma once
#include <algorithm>
#include <vector>
#include <cstring>

namespace pointcloud_raster::math
{

template <typename DataType, int ROWS, int COLS>
struct Matrix
{
    Matrix() {}

    Matrix(const std::vector<DataType> &copyFromdata)
    {
        std::memcpy(data, copyFromdata.data(), ROWS*COLS*sizeof(DataType));
    }

    DataType const&
    operator[](int index) const
    {
        if constexpr (ROWS == 1)
            return data[index];
        else
            return data[index / COLS][index % COLS];
    }

    DataType
    operator[](int index)
    {
        if constexpr (ROWS == 1)
            return data[index];
        else
            return data[index / COLS][index % COLS];
    }

    template <int ROWS_RIGHT, int COLS_RIGHT>
    Matrix<DataType, ROWS, COLS_RIGHT>
    operator*(const Matrix<DataType, ROWS_RIGHT, COLS_RIGHT> &matrix)
    {
        Matrix<DataType, ROWS, COLS_RIGHT> result;
        for (int row = 0; row < ROWS; row++)
            for (int col = 0; col < COLS_RIGHT; col++)
            {
                for (int cell = 0; cell < COLS; cell++)
                    result.data[row][col] += data[row][cell] * matrix.data[cell][col];
            }
        return result;
    }

    DataType data[ROWS][COLS] {0};
};

}

