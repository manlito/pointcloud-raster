#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(Raster, Rasterize)
{
    raster::PointcloudRasterizer::OutputOptions rasterizerOptions;
    rasterizerOptions.raster_size.width = 32;
    rasterizerOptions.raster_size.height = 32;
    raster::PointcloudRasterizer rasterizer(rasterizerOptions);
    rasterizer.AddToRasterFromLASFile(LAS_FILE_SIMPLE_INPUT);
    const auto &image = rasterizer.GetRasterImage();
#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    std::string outputImage = std::string(TESTS_OUTPUT_DIR) + "/rasterized_out.png";
    image.SaveAsPNG(outputImage);
#endif

}
