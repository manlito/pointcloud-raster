#pragma once

#include <string>
#include <optional>
#include <memory>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>

namespace pointcloud_raster::io
{

struct LASWrapper;

class LASReader
{
public:
    LASReader(const std::string filename);

    /**
     * Open file handle and try to create LAS object
     * @return True is file could be opened
     */
    bool
    Open();

    /**
     * Read bounding box data from LAS header
     * @return BoundingBox<double> Rounded 3D bounding box
     */
    BoundingBox3D<double>
    GetBoundingBox() const;

    /**
     * Reads a point from las file
     * @return Read point or `nullopt` if end of file has been reached
     */
    std::optional<Point>
    GetNextPoint();

    ~LASReader();

private:
    const std::string filename_;
    std::unique_ptr<LASWrapper> las_wrapper_;

};

}