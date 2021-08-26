
add_executable(pointcloud_raster_app)
target_sources(pointcloud_raster_app PRIVATE main.cpp)
target_link_libraries(pointcloud_raster_app PointcloudRaster::PointcloudRaster)
target_compile_features(pointcloud_raster_app PRIVATE cxx_std_11)

if (POINTCLOUD_RASTER_LAS_SUPPORT)
    target_compile_definitions(pointcloud_raster_app -DWITH_LAS)
endif()

install(TARGETS pointcloud_raster_app
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/")

