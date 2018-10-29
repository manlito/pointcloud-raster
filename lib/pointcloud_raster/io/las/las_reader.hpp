#pragma once

#include <string>
#include <optional>
#include <memory>
#include <pointcloud_raster/core/point.hpp>

namespace pointcloud_raster::io
{

struct LASWrapper;

class LASReader
{
public:
    LASReader(const std::string filename) : filename_(filename) {}

    /**
     * Open file handle and try to create LAS object
     * @return True is file could be opened
     */
    bool
    Open();

    /**
     * Reads a point from las file
     * @return Read point or `nullopt` if end of file has been reached
     */
    std::optional<Point>
    GetNextPoint();

private:
    const std::string filename_;
    std::unique_ptr<LASWrapper> las_wrapper_;

};

}