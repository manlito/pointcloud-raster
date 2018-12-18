#include <pointcloud_raster/io/las/las_reader.hpp>
#include <fstream>
#include <liblas/liblas.hpp>

namespace pointcloud_raster::io
{

struct LASWrapper
{
    std::ifstream input_stream;
    std::unique_ptr<liblas::Reader> las_reader;
};

LASReader::LASReader(const std::string filename) : filename_(filename)
{
}

bool
LASReader::Open()
{
    lasWrapper_ = std::make_unique<LASWrapper>();
    lasWrapper_->input_stream.open(filename_, std::ios::in | std::ios::binary);
    if (lasWrapper_->input_stream.is_open())
    {
        liblas::ReaderFactory reader_factory;
        lasWrapper_->las_reader =
            std::make_unique<liblas::Reader>(reader_factory.CreateWithStream(lasWrapper_->input_stream));
        return true;
    }
    return false;
}

bool
LASReader::SeekToFirstPoint()
{
    return lasWrapper_->las_reader->Seek(0);
}

bool
LASReader::ComputeBoundingBox()
{
    const auto &headers = lasWrapper_->las_reader->GetHeader();
    boundingBox_.x = std::floor(headers.GetMinX());
    boundingBox_.y = std::floor(headers.GetMinY());
    boundingBox_.z = std::floor(headers.GetMinZ());
    boundingBox_.width = std::ceil(headers.GetMaxX()) - boundingBox_.x;
    boundingBox_.height = std::ceil(headers.GetMaxY()) - boundingBox_.y;
    boundingBox_.depth = std::ceil(headers.GetMaxZ()) - boundingBox_.z;
    return true;
}

std::optional<Point>
LASReader::GetNextPoint()
{
    if (!lasWrapper_)
    {
        return std::nullopt;
    }

    if (lasWrapper_->las_reader->ReadNextPoint())
    {
        const auto &point = lasWrapper_->las_reader->GetPoint();
        const auto &color = point.GetColor();

        return Point(
           point.GetX(),
           point.GetY(),
           point.GetZ(),
           static_cast<unsigned char>(std::min((unsigned int)255, (unsigned int)color.GetRed())),
           static_cast<unsigned char>(std::min((unsigned int)255, (unsigned int)color.GetGreen())),
           static_cast<unsigned char>(std::min((unsigned int)255, (unsigned int)color.GetBlue()))
        );
    }
    else
    {
        return std::nullopt;
    }

}

LASReader::~LASReader()
{
}

}
