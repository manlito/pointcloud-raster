# Main library
add_library(pointcloud_raster)
target_include_directories(pointcloud_raster
    PUBLIC
        $<INSTALL_INTERFACE:.>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
)
target_sources(pointcloud_raster
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/raster/pointcloud_rasterizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/core/image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/math/transform.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/io/pointcloud_provider.cpp
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
endif()

if (POINTCLOUD_RASTER_PLY_SUPPORT)
    option(RPLYCPP_BUILD_SAMPLES OFF)
    option(RPLYCPP_BUILD_TESTS OFF)
    add_subdirectory(third_party/rplycpp)
    message(STATUS "RPLYCPP build enabled")
    target_sources(pointcloud_raster
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/io/ply/ply_reader.cpp
    )
    target_include_directories(pointcloud_raster
        PRIVATE
            ${libLAS_INCLUDE_DIRS}
    )
    target_link_libraries(pointcloud_raster
        PRIVATE
            RPLYCPP::RPLYCPP
    )
endif()

if (POINTCLOUD_RASTER_LAS_SUPPORT)
    find_package(libLAS 1.8 REQUIRED)
    message(STATUS "LibLAS build enabled: libLAS version ${libLAS_VERSION} ${libLAS_LIBRARIES}")
    # LibLAS not exporting correctly targets
    find_package(Boost COMPONENTS filesystem iostreams system thread program_options)
    target_sources(pointcloud_raster
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud_raster/io/las/las_reader.cpp
    )
    target_link_libraries(pointcloud_raster
        PUBLIC
            ${libLAS_LIBRARIES}
    )
    target_include_directories(pointcloud_raster
        PRIVATE
            third_party/rplycpp/src
    )
endif ()

add_library(PointcloudRaster::PointcloudRaster ALIAS pointcloud_raster)

# Install and create cmake files
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/PointcloudRaster)

install(
    TARGETS
        pointcloud_raster
    EXPORT
        PointcloudRasterTargets
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(pointcloud_raster PROPERTIES EXPORT_NAME PointcloudRaster)

# Install headers
install(
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}/"
    FILES_MATCHING PATTERN
        "*.hpp"
)

# Create CMakeTargets file
install(
    EXPORT
        PointcloudRasterTargets
    FILE
        PointcloudRasterTargets.cmake
    NAMESPACE
        PointcloudRaster::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

# Create ConfigVersion.cmake
include(CMakePackageConfigHelpers)
message(${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterConfigVersion.cmake)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterConfigVersion.cmake
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/PointcloudRasterConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install cmake configuration files
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterConfigVersion.cmake
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

# Export
export(
    EXPORT
        PointcloudRasterTargets
    FILE
        ${CMAKE_CURRENT_BINARY_DIR}/PointcloudRasterTargets.cmake
    NAMESPACE
        PointcloudRaster::
)

export(PACKAGE PointcloudRaster)
