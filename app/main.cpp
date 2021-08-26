#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#ifdef WITH_LAS
#include <pointcloud_raster/io/las/las_reader.hpp>
#endif
#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <pointcloud_raster/io/ply/ply_reader.hpp>

int
main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: ./pointcloud_raster_app input_cloud format output_dir" << std::endl;
        std::cout << " - input cloud is the path to the file to raster" << std::endl;
#ifdef WITH_LAS
        std::cout << " - format can be TXT, PLY or LAS" << std::endl;
#else
        std::cout << " - format can be TXT or PLY" << std::endl;
#endif
        std::cout << " - raster viewpoint, can be TOP, SIDE, FRONT, PERSPECTIVE or ALL" << std::endl;
        std::cout << " - output prefix for results. Parent folder should exist" << std::endl;
        std::cout << " - (optional) max width for raster" << std::endl;
        std::cout << " - (optional) max color value, per channel (default 255)" << std::endl;
        std::cout << " Example: ./pointcloud_raster_app input.las LAS ALL output/screenshot 2048 " << std::endl;
        return EXIT_FAILURE;
    }
    const std::string pointcloudFile(argv[1]);
    std::string pointcloudFormat(argv[2]);
    std::transform(pointcloudFormat.begin(), pointcloudFormat.end(), pointcloudFormat.begin(), ::toupper);
    std::string rasterViewPreset(argv[3]);
    std::transform(rasterViewPreset.begin(), rasterViewPreset.end(), rasterViewPreset.begin(), ::toupper);

    std::vector<std::pair<std::string, pointcloud_raster::ViewPointPreset>> viewPresets;
    if (rasterViewPreset == "TOP" || rasterViewPreset == "ALL")
        viewPresets.push_back({"top", pointcloud_raster::ViewPointPreset::TOP});
    if (rasterViewPreset == "SIDE" || rasterViewPreset == "ALL")
        viewPresets.push_back({"side", pointcloud_raster::ViewPointPreset::SIDE});
    if (rasterViewPreset == "FRONT" || rasterViewPreset == "ALL")
        viewPresets.push_back({"front", pointcloud_raster::ViewPointPreset::FRONT});
    if (rasterViewPreset == "PERSPECTIVE" || rasterViewPreset == "ALL")
        viewPresets.push_back({"perspective", pointcloud_raster::ViewPointPreset::FRONT_ISOMETRIC});

    const int rasterSize = argc >= 6 ? std::stoi(argv[5]) : 1024;
    const int pointcloudMaxColor = argc >= 7 ? std::stoi(argv[6]) : 255;

    std::cout << "Rasterizing " << pointcloudFile << " format " << rasterViewPreset << " size " << rasterSize
              << " pointcloud max color " << pointcloudMaxColor << std::endl;

    pointcloud_raster::raster::PointcloudRasterizer rasterizer;
    rasterizer.SetMaxPointColor(pointcloudMaxColor);
    for (const auto &[suffix, viewProfile] : viewPresets)
    {
        pointcloud_raster::raster::PointcloudRasterizer::RasterOptions rasterOptions;
        rasterOptions.rasterViewPointPreset = viewProfile;
        rasterOptions.rasterSize = {rasterSize, rasterSize};
        rasterizer.AddOutputRaster(rasterOptions);
    }
    if (pointcloudFormat == "LAS")
#ifdef WITH_LAS
        rasterizer.AddInputProvider(new pointcloud_raster::io::LASReader(pointcloudFile));
#else
    {
        std::cerr << "LAS support is not available" << std::endl;
        return EXIT_FAILURE;
    }
#endif
    else if (pointcloudFormat == "TXT")
        rasterizer.AddInputProvider(new pointcloud_raster::io::TXTReader(pointcloudFile));
    else if (pointcloudFormat == "PLY")
        rasterizer.AddInputProvider(new pointcloud_raster::io::PLYReader(pointcloudFile));
    else
    {
        std::cerr << "Unknown format " << pointcloudFormat << std::endl;
        return EXIT_FAILURE;
    }
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
            const std::string pngFile = std::string(argv[4]) + "_" + suffix + ".png";
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
