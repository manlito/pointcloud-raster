#include <iostream>
#include <pointcloud_raster/io/ply/ply_reader.hpp>
#include <gtest/gtest.h>

using namespace pointcloud_raster;

TEST(IOPLY, Read)
{
    io::PLYReader plyReader(PLY_FILE_SIMPLE_INPUT);
    std::vector<Point> points;
    if (plyReader.Open()) {
        while (auto nextPoint = plyReader.GetNextPoint()) {
            points.emplace_back(*nextPoint);
        }
        EXPECT_EQ(points.size(), 24);
        EXPECT_EQ(points[5].x, 1);
        EXPECT_EQ(points[15].y, 4);
        EXPECT_EQ(points[19].z, 4);
    } else {
        FAIL() << "Can't open PLY file";
    }
    SUCCEED();
}

TEST(IOPLY, ComputeBoundingBox)
{
    io::PLYReader plyReader(PLY_FILE_SIMPLE_INPUT);
    if (plyReader.Open()) {
        plyReader.ComputeBoundingBox();
        auto boundingBox = plyReader.GetBoundingBox();
        EXPECT_EQ(boundingBox.width, 4);
        EXPECT_EQ(boundingBox.height, 4);
        EXPECT_EQ(boundingBox.depth, 4);
        EXPECT_EQ(boundingBox.x, 0);
        EXPECT_EQ(boundingBox.y, 0);
        EXPECT_EQ(boundingBox.z, 0);
    } else {
        FAIL() << "Can't open PLY file";
    }
    SUCCEED();
}
