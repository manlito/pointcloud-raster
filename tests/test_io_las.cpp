#include <iostream>
#include <pointcloud_raster/io/las/las_reader.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(IOLAS, Read)
{
    io::LASReader lasReader(LAS_FILE_SIMPLE_INPUT);
    std::vector<Point> points;
    if (lasReader.Open()) {
        while (auto nextPoint = lasReader.GetNextPoint()) {
            points.emplace_back(*nextPoint);
        }
        EXPECT_EQ(points.size(), 24);
        EXPECT_EQ(points[5].x, 1);
        EXPECT_EQ(points[15].y, 4);
        EXPECT_EQ(points[19].z, 4);
    } else {
        FAIL() << "Can't open LAS file";
    }
    SUCCEED();
}
