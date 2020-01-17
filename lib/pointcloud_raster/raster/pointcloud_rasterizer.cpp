#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/math/transform.hpp>
#include <pointcloud_raster/io/las/las_reader.hpp>
#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <cmath>

namespace pointcloud_raster::raster
{

bool
PointcloudRasterizer::Rasterize()
{
    // Helper to pack each Raster into unit
    struct Raster
    {
        RasterOptions outputRaster;
        math::RotationTransform rotationTransform;
        BoundingBox3D<double> boundingBox;
        double scale;
        RGBAImage *rasterImage;
        DepthImage depthImage;
    };
    std::vector<Raster> rasters;

    // Copy configuration
    std::transform(outputRasters_.begin(),
                   outputRasters_.end(),
                   std::back_inserter(rasters),
                   [](const auto &outputRaster)
                   {
                       Raster rasterMetadata;
                       rasterMetadata.outputRaster = outputRaster;
                       return rasterMetadata;
                   });

    // Allocate resulting images
    rasterImages_.resize(outputRasters_.size());
    {
        auto rasterImagesIterator = rasterImages_.begin();
        auto rastersIterator = rasters.begin();
        for (;
            rasterImagesIterator != rasterImages_.end();
            rasterImagesIterator++, rastersIterator++)
        {
            rastersIterator->rasterImage = &(*rasterImagesIterator);
        }
    }

    // Create and compute rotation transforms, of each output
    for (auto &raster : rasters)
    {
        raster.rotationTransform.SetFromViewPoint(
            raster.outputRaster.rasterViewPointPreset);
    }

    for (auto &inputProvider : inputProviders_)
    {
        // Open all pointcloud providers (readers)
        if (!inputProvider->Open())
            return false;

        // Compute explicitly bounding box (e.g. parse header in LAS)
        inputProvider->ComputeBoundingBox();

        // Compute a bounding box and assign a rotation transform for each output config
        for (auto &raster : rasters)
        {
            const auto &outputRaster = raster.outputRaster;
            const auto &rotationTransform = raster.rotationTransform;
            auto &boundingBox = raster.boundingBox;

            // Default, bounding box same as the LAS header
            boundingBox = inputProvider->GetBoundingBox();

            if (outputRaster.rasterViewPointPreset != ViewPointPreset::TOP)
            {
                // Recompute bounding box, using all points in the cloud
                double minX = std::numeric_limits<double>::max();
                double minY = std::numeric_limits<double>::max();
                double minZ = std::numeric_limits<double>::max();
                double maxX = -std::numeric_limits<double>::max();
                double maxY = -std::numeric_limits<double>::max();
                double maxZ = -std::numeric_limits<double>::max();
                if (outputRaster.rasterViewPointPreset == ViewPointPreset::FRONT_ISOMETRIC ||
                    outputRaster.rasterViewPointPreset == ViewPointPreset::BACK_ISOMETRIC)
                {
                    // Arbitrary transforms need bounding box computation

                    while (auto nextPoint = inputProvider->GetNextPoint())
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
        }
    }

    // Allocate images
    for (auto &raster : rasters)
    {
        // Compute transform scale
        raster.scale = std::min(static_cast<double>(raster.outputRaster.rasterSize.width) / raster.boundingBox.width,
                                static_cast<double>(raster.outputRaster.rasterSize.height) / raster.boundingBox.height);
        ImageSize imageSize(raster.boundingBox.width * raster.scale,
                            raster.boundingBox.height * raster.scale);

        raster.rasterImage->resize(imageSize, 0);

        // Allocate depth map, used for Z-distance test
        raster.depthImage = DepthImage(imageSize, -std::numeric_limits<DepthImage::Type>::max());

    }

    // Render
    for (const auto &inputProvider : inputProviders_)
    {
        if (!inputProvider->SeekToFirstPoint())
            return false;

        while (auto nextPoint = inputProvider->GetNextPoint())
        {
            for (auto &raster : rasters)
            {
                // Transform point
                const auto &[transformedX, transformedY, transformedZ] =
                raster.rotationTransform.TransformPoint(math::Vector3D(nextPoint->x, nextPoint->y, nextPoint->z));

                // When render, give priority based on distance to camera
                int x = std::max(0, std::min(raster.rasterImage->Width() - 1,
                                             static_cast<int>((transformedX - raster.boundingBox.x)*raster.scale)));
                int y = std::max(0, std::min(raster.rasterImage->Height() - 1,
                                             raster.rasterImage->Height()
                                                 - static_cast<int>((transformedY - raster.boundingBox.y)
                                                     *raster.scale)));

                // Draw if depth will result smaller
                if (auto currentZ = raster.depthImage.Get(x, y);
                    transformedZ > currentZ)
                {
                    raster.depthImage.Set(x, y, static_cast<float>(transformedZ));
                    // Normalize color to range
                    Color rangeNormalizedColor(
                        std::max(0,
                                 std::min(255,
                                          (int) (255.0*((double) nextPoint->color.red)/((double) maxPointColor_)))),
                        std::max(0,
                                 std::min(255,
                                          (int) (255.0*((double) nextPoint->color.green)/((double) maxPointColor_)))),
                        std::max(0,
                                 std::min(255,
                                          (int) (255.0*((double) nextPoint->color.blue)/((double) maxPointColor_))))
                    );
                    raster.rasterImage->SetColor(x, y, rangeNormalizedColor);
                }
            }
        }
    }

    return true;
}

}
