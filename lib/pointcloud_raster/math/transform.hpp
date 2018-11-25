#pragma once
#include <pointcloud_raster/core/view_point.hpp>
#include <pointcloud_raster/math/types.hpp>
#include <vector>

namespace pointcloud_raster
{

class RotationTransform
{
public:

    std::tuple<double, double, double>
    transformPoint(double x, double y, double z);

    bool
    setFromViewPoint(ViewPointPreset viewPointPreset);

    bool
    setFromRotationMatrix(const math::RotationMatrix &rotationMatrix);

private:
    math::RotationMatrix rotationMatrix_;
};

}

