#include <pointcloud_raster/io/ply/ply_reader.hpp>
#include <fstream>
#include <rplycpp.hpp>

namespace pointcloud_raster::io
{

PLYReader::PLYReader(const std::string filename) : filename_(filename)
{
}

bool
PLYReader::Open()
{
    rplycpp::PLYReader reader;
    if (reader.Open(filename_))
    {
        auto vertices_handler = [this](const std::vector<double> &vertex) {
            // This is your app logic, just remember there is an array of double with all row values
            // Input ply has 9 vertex properties: x y z nx ny nz red green blue
            Point point;
            point.x = vertex[0];
            point.y = vertex[1];
            point.z = vertex[2];
            point.color.red = vertex[3];
            point.color.green = vertex[4];
            point.color.blue = vertex[5];
            points_.emplace_back(point);
        };
        // Register the handlers
        std::vector<rplycpp::PLYReadHandler> handlers;
        handlers.push_back(vertices_handler);

        // Read. This will be invoke your handlers when needed
        reader.Read(handlers);

        SeekToFirstPoint();

        return true;
    }
    return false;
}

bool
PLYReader::SeekToFirstPoint()
{
    currentPoint_ = points_.begin();
    return true;
}

std::optional<Point>
PLYReader::GetNextPoint()
{
    if (points_.size() == 0 || currentPoint_ == points_.end())
    {
        return std::nullopt;
    }

    auto point = *currentPoint_;
    currentPoint_++;

    return point;
}

PLYReader::~PLYReader()
{
}

}
