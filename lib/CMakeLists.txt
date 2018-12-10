# Main library
add_library(pointcloud_raster)
target_include_directories(pointcloud_raster
    PUBLIC
        $<INSTALL_INTERFACE:.>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
)
target_sources(pointcloud_raster
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/raster/pointcloud_rasterizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/core/image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/math/transform.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/io/txt/txt_reader.cpp
    )

target_compile_features(pointcloud_raster
    PUBLIC
        cxx_std_17
)

if (POINTCLOUD_RASTER_PNG_SUPPORT)
    target_compile_definitions(pointcloud_raster
        PUBLIC
            POINTCLOUD_RASTER_PNG_SUPPORT
    )
    target_link_libraries(pointcloud_raster
        PUBLIC
            PNG::PNG
    )
    target_include_directories(pointcloud_raster
        PRIVATE
            ${PNG_INCLUDE_DIRS}
    )
endif()

if (POINTCLOUD_RASTER_LAS_SUPPORT)
    find_package(libLAS 1.8 REQUIRED)
    message(STATUS "LibLAS build enabled: libLAS version ${libLAS_VERSION} ${libLAS_LIBRARIES}")
    target_sources(pointcloud_raster
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/io/las/las_reader.cpp
    )
    target_link_libraries(pointcloud_raster
        PUBLIC
            ${libLAS_LIBRARIES}
    )
    target_include_directories(pointcloud_raster
        PRIVATE
            ${libLAS_INCLUDE_DIRS}
    )
endif ()

add_library(PointcloudRaster::PointcloudRaster ALIAS pointcloud_raster)
