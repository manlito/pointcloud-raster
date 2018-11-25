#pragma once
#include <pointcloud_raster/core/size.hpp>
#include <pointcloud_raster/core/color.hpp>
#include <vector>
#include <string>

namespace pointcloud_raster
{

typedef Size<int> ImageSize;

template <typename DataType, int CHANNELS>
class Image
{
public:

    typedef DataType Type;
    constexpr static int channels = CHANNELS;

    Image() : size_(ImageSize(0, 0)) {}

    Image(ImageSize size) : size_(size)
        {
            data_.resize(size.width*size.height*CHANNELS);
            std::fill(data_.begin(), data_.end(), 0);
        }

    Image(ImageSize size, DataType defaultValue) : size_(size)
    {
        data_.resize(size.width*size.height*CHANNELS);
        std::fill(data_.begin(), data_.end(), defaultValue);
    }

    void
    Set(int x, int y, DataType data)
    {
        data_[(y*size_.width + x)*CHANNELS] = data;
    }

    DataType
    Get(int x, int y) const
    {
        return data_[(y*size_.width + x)*CHANNELS];
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

    DataType *
    Prt()
    {
        return data_.data();
    }

    const DataType *
    Prt() const
    {
        return data_.data();
    }

protected:
    ImageSize size_;
    std::vector<DataType> data_;

};

class RGBAImage : public Image<unsigned char, 4>
{
public:

    using Image::Image;

    void
    SetColor(int x, int y, const Color &color)
    {
        auto *dataPtr = &data_[(y*size_.width + x)*4];
        dataPtr[0] = color.red;
        dataPtr[1] = color.green;
        dataPtr[2] = color.blue;
        dataPtr[3] = color.alpha;
    }

    Color
    GetColor(int x, int y) const
    {
        auto *dataPtr = &data_[(y*size_.width + x)*4];
        return Color(dataPtr[0], dataPtr[1], dataPtr[2], dataPtr[3]);
    }

#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    bool
    SaveAsPNG(const std::string &filename, int compressionLevel = 6) const;
#endif

};

class DepthImage : public Image<float, 1>
{
public:

    using Image::Image;

};

}
