#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/io/las/las_reader.hpp>

namespace pointcloud_raster::raster
{

bool
PointcloudRasterizer::AddToRasterFromLASFile(const std::string &pointcloud_file)
{
    io::LASReader lasReader(pointcloud_file);
    if (!lasReader.Open())
        return false;

    const auto boundingBox = lasReader.GetBoundingBox();

    // Compute transform scale
    double scale = 1.0;
    if (output_options_.rasterViewPointPreset == OutputOptions::RasterViewPointPreset::TOP)
    {
        // Use XY plane
        scale = std::min(static_cast<double>(output_options_.raster_size.width) / boundingBox.width,
                         static_cast<double>(output_options_.raster_size.height) / boundingBox.height);
    }
    else
        throw std::runtime_error("Not implemented");

    while (auto nextPoint = lasReader.GetNextPoint())
    {
        // Transform for LAS to pixel coordinates
        if (output_options_.rasterViewPointPreset == OutputOptions::RasterViewPointPreset::TOP)
        {
            int x = std::max(0, std::min(raster_image_.Width() - 1, static_cast<int>((nextPoint->x - boundingBox.x) * scale)));
            int y = std::max(0, std::min(raster_image_.Height() - 1, static_cast<int>((nextPoint->y - boundingBox.y) * scale)));
            raster_image_.Set(x, raster_image_.Height() - y, nextPoint->color);
        }
        else
            throw std::runtime_error("Not implemented");
    }
}

}
