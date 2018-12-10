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

    enum class InputType {LAS, TXT};

    PointcloudRasterizer() {}

    void
    AddOutputRaster(const RasterOptions &outputOptions)
    {
        outputRasters_.push_back(outputOptions);
    }

    void
    AddTXTFile(const std::string &inputFile)
    {
        inputFiles_.emplace_back(std::make_pair(InputType::TXT, inputFile));
    }

    void
    AddLASFile(const std::string &inputFile)
    {
        inputFiles_.emplace_back(std::make_pair(InputType::LAS, inputFile));
    }

    bool
    Rasterize();

    const std::vector<RGBAImage>&
    GetRasterImages() const
    {
        return rasterImages_;
    }

private:
    std::vector<RasterOptions> outputRasters_;
    std::vector<RGBAImage> rasterImages_;
    std::vector<std::pair<InputType, std::string>> inputFiles_;
};

}