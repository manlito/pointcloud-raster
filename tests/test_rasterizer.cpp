#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/io/las/las_reader.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(Raster, Rasterize)
{
    raster::PointcloudRasterizer::RasterOptions rasterizerOptions;
    rasterizerOptions.rasterSize.width = 32;
    rasterizerOptions.rasterSize.height = 32;
    raster::PointcloudRasterizer rasterizer;
    rasterizer.AddOutputRaster(rasterizerOptions);
    rasterizer.AddInputProvider(new io::LASReader(LAS_FILE_SIMPLE_INPUT));
    rasterizer.Rasterize();
    const auto &image = rasterizer.GetRasterImages().front();
#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    std::string outputImage = std::string(TESTS_OUTPUT_DIR) + "/rasterized_out.png";
    image.SaveAsPNG(outputImage);
#endif

}
