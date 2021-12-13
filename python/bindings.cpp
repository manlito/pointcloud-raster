#include <pointcloud_raster/io/pointcloud_provider.hpp>
#include <pointcloud_raster/raster/pointcloud_rasterizer.hpp>
#ifdef WITH_LAS
#include <pointcloud_raster/io/las/las_reader.hpp>
#endif
#include <pointcloud_raster/io/txt/txt_reader.hpp>
#include <pointcloud_raster/io/ply/ply_reader.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

bool
rasterizeHelper(const std::string &pointcloudFile,
                const std::string &pointcloudFormat,
                const std::string &outputPrefix,
                const std::string &rasterViewPreset,
                const int rasterSize,
                const int pointcloudMaxColor)
{
    std::vector<std::pair<std::string, pointcloud_raster::ViewPointPreset>> viewPresets;
    if (rasterViewPreset == "TOP" || rasterViewPreset == "ALL")
        viewPresets.push_back({"top", pointcloud_raster::ViewPointPreset::TOP});
    if (rasterViewPreset == "SIDE" || rasterViewPreset == "ALL")
        viewPresets.push_back({"side", pointcloud_raster::ViewPointPreset::SIDE});
    if (rasterViewPreset == "FRONT" || rasterViewPreset == "ALL")
        viewPresets.push_back({"front", pointcloud_raster::ViewPointPreset::FRONT});
    if (rasterViewPreset == "PERSPECTIVE" || rasterViewPreset == "ALL")
        viewPresets.push_back({"perspective", pointcloud_raster::ViewPointPreset::FRONT_ISOMETRIC});

    pointcloud_raster::raster::PointcloudRasterizer rasterizer;
    rasterizer.SetMaxPointColor(pointcloudMaxColor);
    for (const auto &[suffix, viewProfile]: viewPresets)
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
        return false;
    }
#endif
    else if (pointcloudFormat == "TXT")
        rasterizer.AddInputProvider(new pointcloud_raster::io::TXTReader(pointcloudFile));
    else if (pointcloudFormat == "PLY")
        rasterizer.AddInputProvider(new pointcloud_raster::io::PLYReader(pointcloudFile));
    else
    {
        return false;
    }

    if (!rasterizer.Rasterize())
    {
        return false;
    }
#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
    {
        auto rasterImageIterator = rasterizer.GetRasterImages().begin();
        for (const auto &[suffix, viewProfile]: viewPresets)
        {
            const std::string pngFile = outputPrefix + "_" + suffix + ".png";
            if (!rasterImageIterator->SaveAsPNG(pngFile))
                return false;
            rasterImageIterator++;
        }
    }
#else
    return false;
#endif

    return true;
}

bool
rasterize_ply(const std::string &input_file,
              const std::string &output_prefix,
              const std::string &raster_view_preset,
              const int raster_size,
              const int pointcloud_max_color)
{
    return rasterizeHelper(input_file, "PLY", output_prefix, raster_view_preset, raster_size, pointcloud_max_color);
}

PYBIND11_MODULE(py_pointcloud_raster, m)
{
    m.doc() = "pointcloud_raster python bindings";
    m.def("rasterize_ply", &rasterize_ply, "Creates raster for PLY file");
}
