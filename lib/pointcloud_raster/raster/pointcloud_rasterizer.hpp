#pragma once

#include <string>
#include <pointcloud_raster/core/size.hpp>
#include <pointcloud_raster/core/image_format.hpp>
#include <pointcloud_raster/core/color.hpp>
#include <pointcloud_raster/core/view_point.hpp>
#include <pointcloud_raster/core/image.hpp>

namespace pointcloud_raster::raster
{

class PointcloudRasterizer
{
public:
    struct RasterOptions
    {
        ImageSize rasterSize{1024, 1024};
        ImageFormat format{ImageFormat::JPG};
        ViewPointPreset rasterViewPointPreset{ViewPointPreset::TOP};
        Color backgroundColor{0, 0, 0, 0};
        RasterOptions() {}
    };

    PointcloudRasterizer() {}

    void
    AddOutputRaster(const RasterOptions &outputOptions)
    {
        outputRasters_.push_back(outputOptions);
    }

    bool
    AddToRasterFromLASFile(const std::string &pointcloud_file);

    const std::vector<RGBAImage>&
    GetRasterImages() const
    {
        return rasterImages_;
    }

private:
    std::vector<RasterOptions> outputRasters_;
    std::vector<RGBAImage> rasterImages_;
};

}