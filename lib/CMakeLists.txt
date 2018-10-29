# Main libraryy
add_library(PointcloudRaster)
target_include_directories(PointcloudRaster
    PUBLIC
        $<INSTALL_INTERFACE:.>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
)
target_sources(PointcloudRaster
    PRIVATE
        pointcloud_raster/raster/pointcloud_rasterizer.cpp
    )

target_compile_features(PointcloudRaster
    PRIVATE
        cxx_std_17
)

if (POINTCLOUD_RASTER_LAS_SUPPORT)
    find_package(libLAS 1.8 REQUIRED)
    message(INFO "LibLAS build enabled: libLAS version ${libLAS_VERSION}")
    target_sources(PointcloudRaster
        PRIVATE
            pointcloud_raster/io/las/las_reader.cpp
    )
    target_link_libraries(PointcloudRaster
        PUBLIC
            ${libLAS_LIBRARIES}
    )
    target_include_directories(PointcloudRaster
        PRIVATE
            ${libLAS_INCLUDE_DIRS}
    )
endif ()

