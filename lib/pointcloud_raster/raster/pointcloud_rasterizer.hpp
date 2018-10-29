#pragma once

#include <string>
#include <pointcloud_raster/core/size.hpp>
#include <pointcloud_raster/core/image_format.hpp>
#include <pointcloud_raster/core/color.hpp>

namespace pointcloud_raster::raster
{

class PointcloudRasterizer
{
public:
    struct OutputOptions
    {
        Size<int> raster_size{1024, 1024};
        ImageFormat format{ImageFormat::JPG};
        int pixel_size{2};
        Color background_color{240, 240, 240};
        OutputOptions() {}
    };
    PointcloudRasterizer(OutputOptions output_options = OutputOptions());

    void SetOutputOptions(OutputOptions output_options) { output_options_ = output_options; }
    bool AddToRasterFromFile(const std::string &pointcloud_file);
    void SaveRaster(const std::string &output_image_file);

private:
    OutputOptions output_options_;
};

}