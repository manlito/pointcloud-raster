#pragma once

#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>
#include <pointcloud_raster/io/pointcloud_provider.hpp>

namespace pointcloud_raster::io
{

struct PLYWrapper;

class PLYReader : public PointcloudProvider
{
public:
    PLYReader(const std::string filename);

    /**
     * Open and read a full PLY file with RPLYCPP
     * @return True is file could be opened and read
     */
    virtual bool
    Open();

    /**
     * Move to first point
     * @return True is file could be opened
     */
    virtual bool
    SeekToFirstPoint();

    /**
     * Reads a point from PLY file
     * @return Read point or `nullopt` if end of file has been reached
     */
    virtual std::optional<Point>
    GetNextPoint();

    virtual ~PLYReader();

private:
    const std::string filename_;

    std::vector<Point>::iterator currentPoint_;

    /* PLYReader is all-in-memory */
    std::vector<Point> points_;
};

}