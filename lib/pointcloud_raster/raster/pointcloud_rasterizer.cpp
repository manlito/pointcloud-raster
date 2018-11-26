#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/math/transform.hpp>
#include <pointcloud_raster/io/las/las_reader.hpp>
#include <cmath>

namespace pointcloud_raster::raster
{

bool
PointcloudRasterizer::AddToRasterFromLASFile(const std::string &pointcloud_file)
{
    io::LASReader lasReader(pointcloud_file);
    if (!lasReader.Open())
        return false;

    auto boundingBox = lasReader.GetBoundingBox();

    // Transform bounding box from desired viewPoint
    math::RotationTransform rotationTransform;
    rotationTransform.SetFromViewPoint(output_options_.rasterViewPointPreset);

    if (output_options_.rasterViewPointPreset != ViewPointPreset::TOP)
    {
        // Recompute bounding box, using all points in the cloud
        double minX = std::numeric_limits<double>::max();
        double minY = std::numeric_limits<double>::max();
        double minZ = std::numeric_limits<double>::max();
        double maxX = -std::numeric_limits<double>::max();
        double maxY = -std::numeric_limits<double>::max();
        double maxZ = -std::numeric_limits<double>::max();
        if (output_options_.rasterViewPointPreset == ViewPointPreset::FRONT_ISOMETRIC ||
            output_options_.rasterViewPointPreset == ViewPointPreset::BACK_ISOMETRIC)
        {
            // Arbitrary transforms need bounding box computation
            io::LASReader lasReaderBoundingBox(pointcloud_file);
            if (!lasReaderBoundingBox.Open())
                return false;

            while (auto nextPoint = lasReaderBoundingBox.GetNextPoint())
            {
                const auto &transformedPoint = rotationTransform.TransformPoint(math::Vector3D(nextPoint->x, nextPoint->y, nextPoint->z));
                minX = std::min(minX, transformedPoint[0]);
                minY = std::min(minY, transformedPoint[1]);
                minZ = std::min(minZ, transformedPoint[2]);
                maxX = std::max(maxX, transformedPoint[0]);
                maxY = std::max(maxY, transformedPoint[1]);
                maxZ = std::max(maxZ, transformedPoint[2]);
            }
        }
        else
        {
            // Otherwise, only compute from initial bounding box
            std::vector<math::Vector3D> boundingBoxCuboidCorners{
                math::Vector3D{boundingBox.x, boundingBox.y, boundingBox.z},
                math::Vector3D{boundingBox.x, boundingBox.y, boundingBox.z + boundingBox.depth},
                math::Vector3D{boundingBox.x, boundingBox.y + boundingBox.height, boundingBox.z},
                math::Vector3D{boundingBox.x, boundingBox.y + boundingBox.height, boundingBox.z + boundingBox.depth},
                math::Vector3D{boundingBox.x + boundingBox.width, boundingBox.y, boundingBox.z},
                math::Vector3D{boundingBox.x + boundingBox.width, boundingBox.y, boundingBox.z + boundingBox.depth},
                math::Vector3D{boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height, boundingBox.z},
                math::Vector3D{boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height, boundingBox.z + boundingBox.depth},
            };
            // Transform all corners
            std::vector<math::Vector3D> transformedBoundingBoxCuboidCorners;
            std::transform(boundingBoxCuboidCorners.begin(),
                           boundingBoxCuboidCorners.end(),
                           std::back_inserter(transformedBoundingBoxCuboidCorners),
                           [&rotationTransform](const auto &boundingBoxCorner)
                           {
                               return rotationTransform.TransformPoint(boundingBoxCorner);
                           });
            for (const auto &transformedBoundingBoxCorner : transformedBoundingBoxCuboidCorners)
            {
                minX = std::min(minX, transformedBoundingBoxCorner[0]);
                minY = std::min(minY, transformedBoundingBoxCorner[1]);
                minZ = std::min(minZ, transformedBoundingBoxCorner[2]);
                maxX = std::max(maxX, transformedBoundingBoxCorner[0]);
                maxY = std::max(maxY, transformedBoundingBoxCorner[1]);
                maxZ = std::max(maxZ, transformedBoundingBoxCorner[2]);
            }
        }
        boundingBox.x = std::floor(minX);
        boundingBox.y = std::floor(minY);
        boundingBox.z = std::floor(minZ);
        boundingBox.width = std::ceil(maxX) - boundingBox.x;
        boundingBox.height = std::ceil(maxY) - boundingBox.y;
        boundingBox.depth = std::ceil(maxZ) - boundingBox.z;
    }

    // Compute transform scale
    double scale = std::min(static_cast<double>(output_options_.raster_size.width) / boundingBox.width,
                            static_cast<double>(output_options_.raster_size.height) / boundingBox.height);
    ImageSize imageSize(boundingBox.width * scale, boundingBox.height * scale);

    // Allocate image
    raster_image_ = RGBAImage(imageSize);

    // Allocate depth map, used for Z-distance test
    DepthImage depth_image(imageSize, -std::numeric_limits<DepthImage::Type>::max());

    while (auto nextPoint = lasReader.GetNextPoint())
    {
        // Transform point
        const auto &[transformedX, transformedY, transformedZ] =
            rotationTransform.TransformPoint(math::Vector3D(nextPoint->x, nextPoint->y, nextPoint->z));

        // When render, give priority based on distance to camera
        int x = std::max(0, std::min(raster_image_.Width() - 1, static_cast<int>((transformedX - boundingBox.x) * scale)));
        int y = raster_image_.Height() - std::max(0, std::min(raster_image_.Height() - 1, static_cast<int>((transformedY - boundingBox.y) * scale)));

        // Draw if depth will result smaller
        if (auto currentZ = depth_image.Get(x, y);
            transformedZ > currentZ)
        {
            depth_image.Set(x, y, static_cast<float>(transformedZ));
            raster_image_.SetColor(x, y, nextPoint->color);
        }
    }
}

}
