#pragma once

namespace pointcloud_raster
{

template<typename BoundingBoxType>
struct BoundingBox3D
{
    BoundingBoxType x, y, z, width, height, depth;
    BoundingBox3D() : x(0), y(0), z(0), width(0), height(0), depth(0) {}
    BoundingBox3D(BoundingBoxType x,
                  BoundingBoxType y,
                  BoundingBoxType z,
                  BoundingBoxType width,
                  BoundingBoxType height,
                  BoundingBoxType depth) :
        x(x), y(y), z(z), width(width), height(height), depth(depth) {}
};

}

