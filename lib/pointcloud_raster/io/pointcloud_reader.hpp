#pragma once

#include <string>
#include <optional>
#include <memory>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>

namespace pointcloud_raster::io
{

class PointcloudReader
{
public:
    PointcloudReader() {}
    PointcloudReader(const std::string filename) : filename_(filename) {}

    /**
     * Open file handle and try to create a reader
     * @return True is file could be opened
     */
    virtual bool
    Open() = 0;

    /**
     * Read or compute a bounding box for data
     * @return BoundingBox<double> Rounded 3D bounding box
     */
    virtual BoundingBox3D<double>
    GetBoundingBox() const = 0;

    /**
     * Reads a point from file
     * @return Read point or `nullopt` if end of file has been reached
     */
    virtual std::optional<Point>
    GetNextPoint() = 0;

    virtual
    ~PointcloudReader() {};

private:
    const std::string filename_;

};

}