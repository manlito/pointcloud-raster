#pragma once
#include <pointcloud_raster/core/view_point.hpp>
#include <pointcloud_raster/math/types.hpp>
#include <vector>

namespace pointcloud_raster::math
{

class RotationTransform
{
public:

    void
    SetFromViewPoint(ViewPointPreset viewPointPreset);

    void
    SetFromRotationMatrix(const math::RotationMatrix &rotationMatrix);

    Vector3D
    TransformPoint(const Vector3D &point) const;

private:
    math::RotationMatrix rotationMatrix_;
};

}

