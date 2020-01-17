#pragma once
#include <pointcloud_raster/core/color.hpp>

namespace pointcloud_raster
{

struct Point
{
    double x{0};
    double y{0};
    double z{0};
    Color color;
    Point() {}
    explicit Point(double x, double y, double z, unsigned int red, unsigned int green, unsigned int blue)
        : x(x), y(y), z(z), color(red, green, blue) {}
};

}
