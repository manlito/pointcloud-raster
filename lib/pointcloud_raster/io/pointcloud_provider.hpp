#pragma once

#include <string>
#include <optional>
#include <memory>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>

namespace pointcloud_raster::io
{

class PointcloudProvider
{
public:
    PointcloudProvider() {}

    /**
     * Open file handle and try to create a reader
     * @return True is file could be opened
     */
    virtual bool
    Open() = 0;

    /**
     * Some transformations require recomputation of bounding box. For this,
     * we may want to read again all points. Calling this method should cause
     * GetNextPoint to return to the first point of the pointcloud
     * @return True is operation succeed
     */
    virtual bool
    SeekToFirstPoint() = 0;

    /**
     * Compute a bounding box for data. The reason it is not `const` is
     * because some providers may need to alter its state to compute it
     */
    virtual bool
    ComputeBoundingBox() = 0;

    /**
     * Read bounding box for data.
     * @return BoundingBox<double> Rounded 3D bounding box
     */
    virtual BoundingBox3D<double>
    GetBoundingBox() const { return boundingBox_; };

    /**
     * Reads a point from file
     * @return Read point or `nullopt` if end of file has been reached
     */
    virtual std::optional<Point>
    GetNextPoint() = 0;

    virtual
    ~PointcloudProvider() {};

protected:
    BoundingBox3D<double> boundingBox_;
};

}