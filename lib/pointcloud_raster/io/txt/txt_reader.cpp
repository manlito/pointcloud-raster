#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <fstream>

namespace pointcloud_raster::io
{

TXTReader::TXTReader(const std::string filename, int skipLines)
    : filename_(filename), skipLines_(skipLines)
{
}

bool
TXTReader::Open()
{
    inputStream_.open(filename_);
    return inputStream_.is_open();
}

bool
TXTReader::SeekToFirstPoint()
{
    inputStream_.clear();
    inputStream_.seekg(0, std::ios::beg);
    return inputStream_.good();
}

bool
TXTReader::ComputeBoundingBox()
{
    if (SeekToFirstPoint())
    {

    }
    return SeekToFirstPoint();
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
