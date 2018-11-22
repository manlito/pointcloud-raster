#include <iostream>
#include <pointcloud_raster/io/las/las_reader.hpp>

using namespace pointcloud_raster;

int main()
{
    std::cout << "Opening " << LAS_FILE_SIMPLE_INPUT << std::endl;
    io::LASReader lasReader(LAS_FILE_SIMPLE_INPUT);
    if (lasReader.Open()) {
        while (auto nextPoint = lasReader.GetNextPoint()) {
            std::cout << " Point [" << nextPoint->x << ", " << nextPoint->y << ", " << nextPoint->z << "]\n";
        }
        std::cout << std::flush;
    } else {
        return 1;
    }

    return 0;
}
