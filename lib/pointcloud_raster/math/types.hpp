#pragma once
#include <pointcloud_raster/math/matrix.hpp>
#include <tuple>

namespace pointcloud_raster::math
{

typedef Matrix<double, 3, 3> RotationMatrix;
typedef Matrix<double, 3, 1> Vector3D;

}

MATRIX_STRUCTURE_BINDING_SPECIALIZATION(pointcloud_raster::math::RotationMatrix)
MATRIX_STRUCTURE_BINDING_SPECIALIZATION(pointcloud_raster::math::Vector3D)
