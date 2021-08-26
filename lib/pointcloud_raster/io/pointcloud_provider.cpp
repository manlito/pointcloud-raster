#include <pointcloud_raster/io/pointcloud_provider.hpp>
#include <cmath>

namespace pointcloud_raster::io
{

bool
PointcloudProvider::ComputeBoundingBox()
{
    if (SeekToFirstPoint())
    {
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double minZ = std::numeric_limits<double>::max();
        double maxX = -std::numeric_limits<double>::max();
        double maxY = -std::numeric_limits<double>::max();
        double maxZ = -std::numeric_limits<double>::max();

        while (auto nextPoint = GetNextPoint())
        {
            minX = std::min(minX, nextPoint->x);
            minY = std::min(minY, nextPoint->y);
            minZ = std::min(minZ, nextPoint->z);
            maxX = std::max(maxX, nextPoint->x);
            maxY = std::max(maxY, nextPoint->y);
            maxZ = std::max(maxZ, nextPoint->z);
        }
        boundingBox_.x = std::floor(minX);
        boundingBox_.y = std::floor(minY);
        boundingBox_.z = std::floor(minZ);
        boundingBox_.width = std::ceil(maxX) - boundingBox_.x;
        boundingBox_.height = std::ceil(maxY) - boundingBox_.y;
        boundingBox_.depth = std::ceil(maxZ) - boundingBox_.z;
    }
    return SeekToFirstPoint();
}

}