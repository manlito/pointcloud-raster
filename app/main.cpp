#include <iostream>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>

int
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./pointcloud_raster_app pointcloud.las output_image.png" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string pointcloudFile(argv[1]);
    std::cout << "Rendering pointcloud" << pointcloudFile << std::endl;

    pointcloud_raster::raster::PointcloudRasterizer rasterizer;
    rasterizer.AddToRasterFromLASFile(pointcloudFile);
    const auto &image = rasterizer.GetRasterImage();

#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    const std::string pngFile(argv[2]);
    std::cout << "Saving to image " << pngFile << std::endl;
    if (image.SaveAsPNG(pngFile))
        std::cout << "Saved image!" << std::endl;
    else
        std::cerr << "Error saving image" << std::endl;
#else
    std::cerr << "Library built without PNG support. Ignoring " << pngFile << std::endl;
#endif

    return EXIT_SUCCESS;
}
