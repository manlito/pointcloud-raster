#pragma once

#include <string>
#include <memory>
#include <pointcloud_raster/core/size.hpp>
#include <pointcloud_raster/core/image_format.hpp>
#include <pointcloud_raster/core/color.hpp>
#include <pointcloud_raster/core/view_point.hpp>
#include <pointcloud_raster/core/image.hpp>
#include <pointcloud_raster/io/pointcloud_provider.hpp>

namespace pointcloud_raster::raster
{

class PointcloudRasterizer
{
public:
    struct RasterOptions
    {
        ImageSize rasterSize{1024, 1024};
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

    /**
     * Adds a new pointcloud provider to be rendered. Can be called multiple times.
     * This method TAKES OWNERSHIP of the provided pointer. If your provider does not
     * exist, create one by a subclassing PointcloudProvider;
     *
     * @param inputProvider Pointer to PointcloudProvider instance
     */
    void
    AddInputProvider(io::PointcloudProvider* inputProvider)
    {
        inputProviders_.emplace_back(std::unique_ptr<io::PointcloudProvider>(inputProvider));
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
    std::vector<std::unique_ptr<io::PointcloudProvider>> inputProviders_;
};

}