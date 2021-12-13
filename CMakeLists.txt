cmake_minimum_required(VERSION 3.8)
project(pointcloud_raster VERSION 1.1.0)

# Options
option(POINTCLOUD_RASTER_BUILD_APP "Build sample app to get images from a pointcloud" ON)
option(POINTCLOUD_RASTER_BUILD_TESTS "Build tests (requires GoogleTest)" ON)
option(POINTCLOUD_RASTER_LAS_SUPPORT "Enable build with LibLAS for LAS/LAZ files" OFF)
option(POINTCLOUD_RASTER_PLY_SUPPORT "Enable build with internal PLY reader (only XYZRGB pointclouds)" ON)
option(POINTCLOUD_RASTER_PNG_SUPPORT "Enable build with libPNG (otherwise, memory only interface will be available)" ON)
option(POINTCLOUD_RASTER_PYTHON_SUPPORT "Enable python bindings" OFF)
set(POINTCLOUD_RASTER_TEST_OUTPUT "." CACHE STRING "Folder to put test generated files")

# Default build to Release
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build to default type  '${default_build_type}'.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build.")
endif ()

if (POINTCLOUD_RASTER_PNG_SUPPORT)
    find_package(PNG REQUIRED)
endif()

# Needed for pybind compilation
if (POINTCLOUD_RASTER_PYTHON_SUPPORT)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

add_subdirectory(lib)
if (POINTCLOUD_RASTER_BUILD_APP AND POINTCLOUD_RASTER_PNG_SUPPORT)
    add_subdirectory(app)
endif ()

# Python
if (POINTCLOUD_RASTER_PYTHON_SUPPORT)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(py_pointcloud_raster
        ${CMAKE_CURRENT_SOURCE_DIR}/python/bindings.cpp
    )
    target_link_libraries(py_pointcloud_raster
        PRIVATE
            PointcloudRaster::PointcloudRaster
    )
    target_compile_features(py_pointcloud_raster PRIVATE cxx_std_17)
endif()

# Testing
if (POINTCLOUD_RASTER_BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif ()

