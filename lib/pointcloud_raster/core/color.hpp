#pragma once

namespace pointcloud_raster
{

struct Color
{
    unsigned int red{0};
    unsigned int green{0};
    unsigned int blue{0};
    unsigned int alpha{0};

    Color() {}

    explicit Color(unsigned int red, unsigned int green, unsigned int blue)
        : red(red), green(green), blue(blue), alpha(255) {}

    explicit Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
        : red(red), green(green), blue(blue), alpha(alpha) {}

    bool
    operator==(const Color &color) const
    {
        return color.green==green && color.red==red && color.blue==blue && color.alpha==alpha;
    }

    bool
    operator!=(const Color &color) const
    {
        return !(color==*this);
    }
};

}
