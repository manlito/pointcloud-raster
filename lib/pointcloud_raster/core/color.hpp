#pragma once

namespace pointcloud_raster
{

struct Color
{
    unsigned char red{0};
    unsigned char green{0};
    unsigned char blue{0};
    unsigned char alpha{0};
    Color() {}
    explicit Color(unsigned char red, unsigned char green, unsigned char blue)
        : red(red), green(green), blue(blue), alpha(0) {}
    explicit Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
        : red(red), green(green), blue(blue), alpha(alpha) {}

    bool
    operator==(const Color &color) const
    {
        return color.green == green && color.red == red && color.blue == blue && color.alpha == alpha;
    }
    bool
    operator!=(const Color &color) const
    {
        return !(color == *this);
    }
};

}
