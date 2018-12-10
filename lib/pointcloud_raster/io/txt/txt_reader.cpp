#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <fstream>

namespace pointcloud_raster::io
{

TXTReader::TXTReader(const std::string filename) : filename_(filename)
{
}

bool
TXTReader::Open()
{
    return false;
}

BoundingBox3D<double>
TXTReader::GetBoundingBox() const
{
    BoundingBox3D<double> boundingBox3D;
    return boundingBox3D;
}

std::optional<Point>
TXTReader::GetNextPoint()
{

    return std::nullopt;
}

TXTReader::~TXTReader()
{
}

}
