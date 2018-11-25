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
    las_wrapper_ = std::make_unique<LASWrapper>();
    las_wrapper_->input_stream.open(filename_, std::ios::in | std::ios::binary);
    if (las_wrapper_->input_stream.is_open())
    {
        liblas::ReaderFactory reader_factory;
        las_wrapper_->las_reader =
            std::make_unique<liblas::Reader>(reader_factory.CreateWithStream(las_wrapper_->input_stream));
        return true;
    }
    return false;
}

BoundingBox3D<int>
LASReader::GetBoundingBox() const
{
    const auto &headers =las_wrapper_->las_reader->GetHeader();
    BoundingBox3D<int> boundingBox3D;
    boundingBox3D.x = std::floor(headers.GetMinX());
    boundingBox3D.y = std::floor(headers.GetMinY());
    boundingBox3D.z = std::floor(headers.GetMinZ());
    boundingBox3D.width = std::ceil(headers.GetMaxX()) - boundingBox3D.x;
    boundingBox3D.height = std::ceil(headers.GetMaxY()) - boundingBox3D.y;
    boundingBox3D.depth = std::ceil(headers.GetMaxZ()) - boundingBox3D.z;
    return boundingBox3D;
}

std::optional<Point>
LASReader::GetNextPoint()
{
    if (!las_wrapper_)
    {
        return std::nullopt;
    }

    if (las_wrapper_->las_reader->ReadNextPoint())
    {
        const auto &point = las_wrapper_->las_reader->GetPoint();
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
