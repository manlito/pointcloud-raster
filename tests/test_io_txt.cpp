#include <iostream>
#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(IOTXT, Read)
{
    io::TXTReader txtReader(TXT_FILE_SIMPLE_INPUT);
    std::vector<Point> points;
    if (txtReader.Open()) {
        while (auto nextPoint = txtReader.GetNextPoint()) {
            points.emplace_back(*nextPoint);
        }
        EXPECT_EQ(points.size(), 24);
        EXPECT_EQ(points[5].x, 1);
        EXPECT_EQ(points[15].y, 4);
        EXPECT_EQ(points[19].z, 4);
    } else {
        FAIL() << "Can't open TXT file";
    }
    SUCCEED();
}

TEST(IOTXT, ComputeBoundingBox)
{
    const std::string pointcloudFile = std::string(TESTS_OUTPUT_DIR) + "/io_txt_bounding_box.txt";
    {
        std::string samplePointcloud = {
            "Just one random header\n"
            "1 1 4 0 0 0\n"
            "10 -2 4 0 0 0\n"
            "1 -2 8 0 0 0\n"
        };
        std::ofstream ofstream(pointcloudFile);
        ofstream << samplePointcloud;
    }
    io::TXTReader txtReader(pointcloudFile, 1);
    std::vector<Point> points;
    if (txtReader.Open()) {
        txtReader.ComputeBoundingBox();
        auto boundingBox = txtReader.GetBoundingBox();
        EXPECT_EQ(boundingBox.width, 9);
        EXPECT_EQ(boundingBox.height, 3);
        EXPECT_EQ(boundingBox.depth, 4);
        EXPECT_EQ(boundingBox.x, 1);
        EXPECT_EQ(boundingBox.y, -2);
        EXPECT_EQ(boundingBox.z, 4);
    } else {
        FAIL() << "Can't open TXT file";
    }
    SUCCEED();
}
