# Pointcloud Raster #

Provides a simple tool wit no dependencies to generate a raster from a pointcloud.

## Background ##

It is possible to create raster from PNGs using things like cloudcompare. However, tools that do raster usually have many requirements and possibly license constraints. I wanted something simple for my QA needs, where I simply pass the cloud, takes the photos for me, and works in a headless environment. 

In addition, I wanted to be able to do it on any device (IOS, android).

So, if you depend on high accuracy renders for your customers, this is not for. But if you only need to create screenshots/rasters which are for previews, this is for you.

## Roadmap ##

### Under development ###

[ ] Basic read from TXT or LAS files
[ ] Raster of a photo to output JPG file
[ ] Rastering from fixed view points (top, left, right, bottom)

### Planned ###

[ ] Support other input
[ ] Floating point outputs
[ ] Use raster output to geolocate it (TIFF output). If added, will be optional because of GDAL.
[ ] Applying additional transform
[ ] Arbitrary camera placement
[ ] Other raster formats
[ ] Low memory footprint with tiled read and write

## How to build ##

This program has no dependencies, beyond standard library, a modern CMake and compiler. Do:

```bash
git clone 
mkdir -p repo-build
cd repo-build
cmake  ../repo
make
make install
```

To disable example app and/or test, use these options. For instance:

```bash
cmake -D POINTCLOUD_SCREENSHOT_BUILD_APP=Off  -D POINTCLOUD_SCREENSHOT_BUILD_TESTS=Off ../repo
```

## How to use ##

### Example app ###

By default an example application is built that only accepts 3 parameters:

```bash
pointcloud_raster <cloud> <top|bottom|right|left> <max_size>
```

for instances

```bash
pointcloud_raster cloud.las top 1024
```

Creates a raster from top view, with a max size of 1024.

### Library usage ###

First, you install it and then in your `CMakeLists.txt` search as:

```cmake
find_package(PointcloudRaster 1.0 REQUIRED)
add_executable(your_program your_code.cpp)
target_link_libraries(your_program PointcloudRaster::PointcloudRaster)

```
