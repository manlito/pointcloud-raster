#pragma once

namespace pointcloud_raster
{

template<typename SizeType>
struct Size
{
    SizeType width{0};
    SizeType height{0};

    Size() {}
    Size(SizeType width, SizeType height) : width(width), height(height) {}
};

}

