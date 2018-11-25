#pragma once
#include <pointcloud_raster/core/size.hpp>
#include <pointcloud_raster/core/color.hpp>
#include <vector>
#include <string>

namespace pointcloud_raster
{

typedef Size<int> ImageSize;

class Image
{
public:

    Image() : size_(ImageSize(0, 0)) {}

    Image(ImageSize size) : size_(size)
    {
        data_.resize(size.width*size.height*4);
        std::fill(data_.begin(), data_.end(), 0);
    }

    void
    Set(int x, int y, const Color &color)
    {
        auto *dataPtr = &data_[(y*size_.width + x)*4];
        dataPtr[0] = color.red;
        dataPtr[1] = color.green;
        dataPtr[2] = color.blue;
        dataPtr[3] = color.alpha;
    }

    Color
    Get(int x, int y) const
    {
        auto *dataPtr = &data_[(y*size_.width + x)*4];
        return Color(dataPtr[0], dataPtr[1], dataPtr[2], dataPtr[3]);
    }

    int
    Width() const
    {
        return size_.width;
    }

    int
    Height() const
    {
        return size_.height;
    }

    unsigned char *
    Prt()
    {
        return data_.data();
    }

    const unsigned char *
    Prt() const
    {
        return data_.data();
    }

#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    bool
    SaveAsPNG(const std::string &filename, int compressionLevel = 6) const;
#endif

private:
    ImageSize size_;
    std::vector<unsigned char> data_;
};

}
