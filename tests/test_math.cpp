#include <iostream>
#include <pointcloud_raster/math/transform.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(Math, MatrixInitialization)
{
    std::vector<double> matrixData {1, 2, 3, 4, 5, 6, 7, 8, 9};
    math::RotationMatrix rotationMatrix(matrixData);

    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
        EXPECT_EQ(matrixData[row*3 + col], rotationMatrix.data[row][col]);
}

TEST(Math, VariadicInitialization)
{
    math::Vector3D vector3D(1.0, 2.0, 3.0);
    auto &[x, y, z] = vector3D;
    EXPECT_FLOAT_EQ(x, 1.0);
    EXPECT_FLOAT_EQ(y, 2.0);
    EXPECT_FLOAT_EQ(z, 3.0);
}

TEST(Math, ListInitialization)
{
    math::Vector3D vector3D{1, 2, 3};
    auto &[x, y, z] = vector3D;
    EXPECT_FLOAT_EQ(x, 1.0);
    EXPECT_FLOAT_EQ(y, 2.0);
    EXPECT_FLOAT_EQ(z, 3.0);
}

TEST(Math, TestSpecialization)
{
    math::Vector3D vector3D(std::vector<double> {1, 2, 3});
    auto &[x, y, z] = vector3D;
    EXPECT_FLOAT_EQ(x, 1.0);
    EXPECT_FLOAT_EQ(y, 2.0);
    EXPECT_FLOAT_EQ(z, 3.0);
}

TEST(Math, MatrixMultiplication)
{
    std::vector<double> matrixData {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> vectorData(3, 10);

    math::RotationMatrix rotationMatrix(matrixData);
    math::Vector3D vector3D(vectorData);

    const auto &result = rotationMatrix * vector3D;
    EXPECT_FLOAT_EQ(result[0], 60.0);
    EXPECT_FLOAT_EQ(result[1], 150.0);
    EXPECT_FLOAT_EQ(result[2], 240.0);
}

