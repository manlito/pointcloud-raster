#pragma once

#include <string>
#include <optional>
#include <memory>
#include <fstream>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>
#include <pointcloud_raster/io/pointcloud_provider.hpp>

namespace pointcloud_raster::io
{

class TXTReader : public PointcloudProvider
{
public:
    TXTReader(const std::string filename,
              int skipLines = 0);

    /**
     * Open file handle and try to create LAS object
     * @return True is file could be opened
     */
    bool
    Open();

    /**
     * Move to first point
     * @return True is file could be opened
     */
    virtual bool
    SeekToFirstPoint();

    /**
     * Compute bounding box data from LAS header
     * @return BoundingBox<double> Rounded 3D bounding box
     */
    virtual bool
    ComputeBoundingBox();

    /**
     * Reads a point from las file
     * @return Read point or `nullopt` if end of file has been reached
     */
    std::optional<Point>
    GetNextPoint();

    ~TXTReader();

private:
    const std::string filename_;
    const int skipLines_;
    std::fstream inputStream_;
};

}