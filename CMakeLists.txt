cmake_minimum_required(VERSION 3.5)
project(pointcloud_raster VERSION 1.0.0 )

# Options
option(POINTCLOUD_RASTER_BUILD_APP "Build sample app to get images from a pointcloud" ON)
option(POINTCLOUD_RASTER_BUILD_TESTS "Build tests" ON)
option(POINTCLOUD_RASTER_LAS_SUPPORT "Enable build with LibLAS for LAS/LAZ files" ON)
set(POINTCLOUD_RASTER_TEST_OUTPUT "." CACHE STRING "Folder to put test generated files")

# Default build to Release
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build to default type  '${default_build_type}'.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build.")
endif ()

add_subdirectory(lib)
add_subdirectory(app)

# Testing
if (POINTCLOUD_RASTER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install and create cmake files
#install(
#  EXPORT POINTCLOUD_RASTER_EXPORTS
#  NAMESPACE PointcloudRaster
#  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PointcloudRasterUtils
#  FILE PointcloudRasterTargets.cmake
#)
