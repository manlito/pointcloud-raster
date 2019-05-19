# Pointcloud Raster #

Provides a simple tool wit no dependencies to generate a raster from a pointcloud.

## Background ##

It is possible to create raster from PNGs using things like awesome cloudcompare. However, tools that do raster usually have many requirements to build and possibly license constraints. I wanted something simple for my QA needs, where I simply pass the cloud, takes the photos for me, and works in a headless environment.

In addition, I wanted to be able to do it on any device (IOS, android).

So, if you depend on high accuracy renders for your customers, this is not for you. But if you only need to create screenshots/rasters which are for previews or quick screenshots, enjoy.

## Features ##

- Supports TXT and LAS files
- Can output to PNG file (requires libpng)
- Supports rasters from arbitrary view points (cameras)
- Files are rasterized on fly, meaning low memory usage. However, raster is hold in memory, which implies max size depends on your system.

## Todos ##

- [ ] Floating point outputs (e.g., adding elevation map)
- [ ] Use raster output to geolocate it (TIFF output). If added, will be optional because of GDAL.
- [ ] Low memory footprint with tiled write of output rasters

## How to build ##

This program has no dependencies, beyond standard library, a modern CMake and compiler. Do:

```bash
git clone https://github.com/manlito/pointcloud-raster
mkdir -p pointcloud-raster-build
cd pointcloud-raster-build
cmake  ../pointcloud-raster
make -j4
make install
```

To disable example app and/or test, use these options. For instance:

```bash
cmake -D POINTCLOUD_RASTER_BUILD_APP=Off  -D POINTCLOUD_RASTER_BUILD_TESTS=Off ../pointcloud-raster
```

### Without PNG ###

Such use case can be cool if you only need to return an in memory raster and have another way to display.

```bash
cmake -D POINTCLOUD_RASTER_BUILD_APP=Off -D POINTCLOUD_RASTER_PNG_SUPPORT=Off ../pointcloud-raster
```

### Without LAS ###

If your use case needs a different way of reading, you should create your own PointcloudProvider. Please see [TXTReader](lib/pointcloud_raster/io/txt) to see how. When, you should build as follows:

```bash
cmake -D POINTCLOUD_RASTER_BUILD_APP=Off -D POINTCLOUD_RASTER_LAS_SUPPORT=Off ../pointcloud-raster
```

## How to use ##

### Example app ###

By default an example application is built that only accepts 3 parameters:

```bash
pointcloud_raster <cloud.las|cloud.txt> <LAS|TXT> <SIDE|FRONT|TOP|BOTTOM|PERSPECTIVE|ALL> <output_prefix> <max_size>
```

For instance:

```bash
pointcloud_raster cloud.las LAS TOP preview 1024
```

Creates a single file in working dir named `preview_top.png`. But special case:

```bash
pointcloud_raster cloud.las LAS ALL preview 1024
```

Creates 4 files in working dir.

>Note: Output dir is expected to exist.

### Library usage ###

First, you install it and then in your `CMakeLists.txt` search as:

```cmake
find_package(PointcloudRaster 1.0 REQUIRED)
add_executable(your_program your_code.cpp)
target_link_libraries(your_program PointcloudRaster::PointcloudRaster)

```
 See [Examples repo](https://github.com/manlito/pointcloud-raster-examples/blob/master/examples/rasterize-txt/CMakeLists.txt) for more.