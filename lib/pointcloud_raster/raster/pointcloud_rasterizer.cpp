#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/math/transform.hpp>
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
    ImageSize imageSize;
    if (output_options_.rasterViewPointPreset == ViewPointPreset::TOP)
    {
        // Use XY plane
        scale = std::min(static_cast<double>(output_options_.raster_size.width) / boundingBox.width,
                         static_cast<double>(output_options_.raster_size.height) / boundingBox.height);
        imageSize.width = boundingBox.width * scale;
        imageSize.height = boundingBox.height * scale;
    }
    else
        throw std::runtime_error("Not implemented");

    raster_image_ = RGBAImage(imageSize);
    DepthImage depth_image(imageSize, -std::numeric_limits<DepthImage::Type>::max());

    while (auto nextPoint = lasReader.GetNextPoint())
    {
        // Transform for LAS to pixel coordinates
        if (output_options_.rasterViewPointPreset == ViewPointPreset::TOP)
        {
            // When render, give priority based on distance to camera
            int x = std::max(0, std::min(raster_image_.Width() - 1, static_cast<int>((nextPoint->x - boundingBox.x) * scale)));
            int y = raster_image_.Height() - std::max(0, std::min(raster_image_.Height() - 1, static_cast<int>((nextPoint->y - boundingBox.y) * scale)));

            // Draw if depth will result smaller
            if (auto currentZ = depth_image.Get(x, y);
                nextPoint->z > currentZ)
            {
                depth_image.Set(x, y, static_cast<float>(nextPoint->z));
                raster_image_.SetColor(x, y, nextPoint->color);
            }
        }
        else
            throw std::runtime_error("Not implemented");
    }
}

}
