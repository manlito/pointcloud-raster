#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <fstream>
#include <cmath>

namespace pointcloud_raster::io
{

TXTReader::TXTReader( const std::string filename, int skipLines, char delimiter,
    std::function<Point(const std::string &line, const char &delimiter)> readLineLambda)
    : filename_(filename), skipLines_(skipLines), delimiter_(delimiter), readLineLambda_(readLineLambda)
{
}

bool
TXTReader::Open()
{
    inputStream_.open(filename_);
    return SeekToFirstPoint();
}

bool
TXTReader::SeekToFirstPoint()
{
    inputStream_.clear();
    inputStream_.seekg(0, std::ios::beg);
    int skippedLines = 0;
    while (skippedLines < skipLines_){
        inputStream_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        skippedLines++;
    }
    return inputStream_.good();
}

bool
TXTReader::ComputeBoundingBox()
{
    if (SeekToFirstPoint())
    {
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double minZ = std::numeric_limits<double>::max();
        double maxX = -std::numeric_limits<double>::max();
        double maxY = -std::numeric_limits<double>::max();
        double maxZ = -std::numeric_limits<double>::max();

        while (auto nextPoint = GetNextPoint())
        {
            minX = std::min(minX, nextPoint->x);
            minY = std::min(minY, nextPoint->y);
            minZ = std::min(minZ, nextPoint->z);
            maxX = std::max(maxX, nextPoint->x);
            maxY = std::max(maxY, nextPoint->y);
            maxZ = std::max(maxZ, nextPoint->z);
        }
        boundingBox_.x = std::floor(minX);
        boundingBox_.y = std::floor(minY);
        boundingBox_.z = std::floor(minZ);
        boundingBox_.width = std::ceil(maxX) - boundingBox_.x;
        boundingBox_.height = std::ceil(maxY) - boundingBox_.y;
        boundingBox_.depth = std::ceil(maxZ) - boundingBox_.z;
    }
    return SeekToFirstPoint();
}

std::optional<Point>
TXTReader::GetNextPoint()
{
    std::string line;
    try
    {
        if (std::getline(inputStream_, line))
            return readLineLambda_(line, delimiter_);
    }
    catch (const std::exception)
    {
        return std::nullopt;
    }
    return std::nullopt;
}

TXTReader::~TXTReader()
{
}

}
