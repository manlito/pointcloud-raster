#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(Raster, Rasterize)
{
    raster::PointcloudRasterizer::RasterOptions rasterizerOptions;
    rasterizerOptions.rasterSize.width = 32;
    rasterizerOptions.rasterSize.height = 32;
    raster::PointcloudRasterizer rasterizer;
    rasterizer.AddOutputRaster(rasterizerOptions);
    rasterizer.AddInputProvider(new io::TXTReader(TXT_FILE_SIMPLE_INPUT));
    rasterizer.Rasterize();
    const auto &image = rasterizer.GetRasterImages().front();
    EXPECT_EQ(image.Width(), 32);
    EXPECT_EQ(image.Height(), 32);
#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    std::string outputImage = std::string(TESTS_OUTPUT_DIR) + "/rasterized_out.png";
    image.SaveAsPNG(outputImage);
#endif

}
