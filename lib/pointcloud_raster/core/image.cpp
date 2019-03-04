#include <pointcloud_raster/core/image.hpp>
#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
#include <fstream>
#include <png.h>
#endif

namespace pointcloud_raster
{

#ifdef POINTCLOUD_RASTER_PNG_SUPPORT
bool
RGBAImage::SaveAsPNG(const std::string &filename, int compressionLevel) const
{
    FILE *fp = std::fopen(filename.c_str(), "wb");

    if (!fp)
        return false;

    auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        std::fclose(fp);
        return false;
    }

    auto info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp) nullptr);
        std::fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, compressionLevel);
    png_set_IHDR(png_ptr, info_ptr,
                 size_.width, size_.height,
                 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Point to data
    std::vector<png_bytep> row_pointers;
    row_pointers.resize(size_.height);
    for (int row = 0; row < size_.height; ++row)
        row_pointers[row] = const_cast<png_bytep>(&data_[row*size_.width*4]);

    int png_transforms = PNG_TRANSFORM_IDENTITY;

    // Write
    png_set_rows(png_ptr, info_ptr, &row_pointers[0]);
    png_write_png(png_ptr, info_ptr, png_transforms, nullptr);
    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    std::fclose(fp);

    return true;
}
#endif

}
