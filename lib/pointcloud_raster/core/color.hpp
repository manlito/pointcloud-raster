#pragma once

namespace pointcloud_raster
{

struct Color
{
    unsigned char red{0};
    unsigned char green{0};
    unsigned char blue{0};
    Color() {}
    explicit Color(unsigned char red, unsigned char green, unsigned char blue)
        : red(red), green(green), blue(blue) {}
};

}
