#include <pointcloud_raster/core/view_point.hpp>
#include <pointcloud_raster/math/types.hpp>
#include <vector>
#include <tuple>
#include <cmath>
#include "transform.hpp"

namespace pointcloud_raster::math
{

void
math::RotationTransform::SetFromViewPoint(ViewPointPreset viewPointPreset)
{
    enum class RotationAxis {X, Y, Z};
    auto buildRotationMatrix = [](const RotationAxis rotationAxis, double radians)
    {
        if (rotationAxis == RotationAxis::X)
        {
            return RotationMatrix({1, 0, 0,
                                   0, std::cos(radians), -std::sin(radians),
                                   0, std::sin(radians), std::cos(radians)});
        }
        else if (rotationAxis == RotationAxis::Y)
        {
            return RotationMatrix({std::cos(radians), 0, std::sin(radians),
                                   0, 1, 0,
                                   -std::sin(radians), 0, std::cos(radians)});
        }
        else if (rotationAxis == RotationAxis::Z)
        {
            return RotationMatrix({std::cos(radians), -std::sin(radians), 0,
                                   std::sin(radians), std::cos(radians), 0,
                                   0, 0, 1});
        }
    };
    const RotationMatrix rotateAroundX = buildRotationMatrix(RotationAxis::X, -M_PI_2);
    const RotationMatrix rotateAroundZ = buildRotationMatrix(RotationAxis::Z, M_PI_2);
    if (viewPointPreset == ViewPointPreset::TOP)
    {
        rotationMatrix_ = RotationMatrix({
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        });
    }
    else if (viewPointPreset == ViewPointPreset::FRONT)
    {
        rotationMatrix_ = rotateAroundX;
    }
    else if (viewPointPreset == ViewPointPreset::LEFT)
    {
        rotationMatrix_ = rotateAroundX*rotateAroundZ;
    }
    else if (viewPointPreset == ViewPointPreset::BACK)
    {
        rotationMatrix_ = rotateAroundX*rotateAroundZ*rotateAroundZ;
    }
    else if (viewPointPreset == ViewPointPreset::SIDE)
    {
        rotationMatrix_ = rotateAroundX*rotateAroundZ*rotateAroundZ*rotateAroundZ;
    }
    else if (viewPointPreset == ViewPointPreset::BOTTOM)
    {
        rotationMatrix_ = rotateAroundX*rotateAroundX*rotateAroundZ*rotateAroundZ;
    }
    else if (viewPointPreset == ViewPointPreset::FRONT_ISOMETRIC)
    {
        rotationMatrix_ =
            buildRotationMatrix(RotationAxis::Z, 3*M_PI_4/4)*buildRotationMatrix(RotationAxis::Y, 3*M_PI_4/4)
                *buildRotationMatrix(RotationAxis::X, -M_PI_4);
    }
    else if (viewPointPreset == ViewPointPreset::BACK_ISOMETRIC)
    {
        rotationMatrix_ =
            buildRotationMatrix(RotationAxis::X, -5*M_PI_4/4)
                *buildRotationMatrix(RotationAxis::Y, -1*M_PI_4/4)*buildRotationMatrix(RotationAxis::Z, M_PI+5*M_PI_4/8);
    }
    else
    {
        throw std::runtime_error("viewPointPreset not implemented");
    }
}

void
RotationTransform::SetFromRotationMatrix(const math::RotationMatrix &rotationMatrix)
{
    rotationMatrix_ = rotationMatrix;
}

Vector3D
RotationTransform::TransformPoint(const Vector3D &point) const
{
    return rotationMatrix_ * point;
}

}
