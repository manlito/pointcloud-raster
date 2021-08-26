#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <fstream>

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
