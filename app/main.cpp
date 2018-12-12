#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#include <pointcloud_raster/io/las/las_reader.hpp>

int
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./pointcloud_raster_app pointcloud.las output_image_prefix" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string pointcloudFile(argv[1]);
    std::cout << "Rendering pointcloud" << pointcloudFile << std::endl;

    const std::vector<std::pair<std::string, pointcloud_raster::ViewPointPreset>> viewPresets {
        {"top", pointcloud_raster::ViewPointPreset::TOP},
        {"side", pointcloud_raster::ViewPointPreset::SIDE},
        {"front", pointcloud_raster::ViewPointPreset::FRONT},
        {"perspective", pointcloud_raster::ViewPointPreset::FRONT_ISOMETRIC}
    };

    pointcloud_raster::raster::PointcloudRasterizer rasterizer;
    for (const auto &[suffix, viewProfile] : viewPresets)
    {
        pointcloud_raster::raster::PointcloudRasterizer::RasterOptions rasterOptions;
        rasterOptions.rasterViewPointPreset = viewProfile;
        rasterizer.AddOutputRaster(rasterOptions);
    }
    rasterizer.AddInputProvider(new pointcloud_raster::io::LASReader(pointcloudFile));
    if (!rasterizer.Rasterize())
    {
        std::cerr << "Rasterization failed" << std::endl;
        return EXIT_FAILURE;
    }

    if (viewPresets.size() != rasterizer.GetRasterImages().size())
    {
        std::cerr << "Error: Number of resulting rasters is not the same as given input configurations."
                  << "Given " << viewPresets.size() << " vs generated " << rasterizer.GetRasterImages().size()
                  << std::endl;
        return EXIT_FAILURE;
    }


#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    {
        auto rasterImageIterator = rasterizer.GetRasterImages().begin();
        for (const auto &[suffix, viewProfile] : viewPresets)
        {
            const std::string pngFile = std::string(argv[2]) + "_" + suffix + ".png";
            std::cout << "Saving image " << pngFile << std::endl;
            if (!rasterImageIterator->SaveAsPNG(pngFile))
                std::cerr << "Error saving image" << std::endl;
            rasterImageIterator++;
        }
    }
#else
    std::cerr << "Library built without PNG support. No outputs bill be saved." << std::endl;
#endif

    return EXIT_SUCCESS;
}
