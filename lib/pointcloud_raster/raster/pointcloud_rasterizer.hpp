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
    struct OutputOptions
    {

        ImageSize raster_size{1024, 1024};
        ImageFormat format{ImageFormat::JPG};
        ViewPointPreset rasterViewPointPreset{ViewPointPreset::TOP};
        int pixel_size{2};
        Color background_color{240, 240, 240};
        OutputOptions() {}
    };

    PointcloudRasterizer(OutputOptions output_options = OutputOptions())
        : output_options_(output_options) {}

    void
    SetOutputOptions(const OutputOptions &output_options) { output_options_ = output_options; }

    bool
    AddToRasterFromLASFile(const std::string &pointcloud_file);

    const RGBAImage&
    GetRasterImage() const
    {
        return raster_image_;
    }

private:
    OutputOptions output_options_;
    RGBAImage raster_image_;
};

}