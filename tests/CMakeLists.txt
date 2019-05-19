include(GoogleTest)

# Shared files
set(DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(LAS_FILE_SIMPLE_INPUT "${DATA_PATH}/cloud-dummy.las")
set(TXT_FILE_SIMPLE_INPUT "${DATA_PATH}/cloud-dummy.txt")
set(LIB_LINK_LIBRARIES PointcloudRaster::PointcloudRaster GTest::GTest GTest::Main)
set(LIB_DEFINITIONS
    LAS_FILE_SIMPLE_INPUT="${LAS_FILE_SIMPLE_INPUT}"
    TXT_FILE_SIMPLE_INPUT="${TXT_FILE_SIMPLE_INPUT}"
    TESTS_OUTPUT_DIR="${POINTCLOUD_RASTER_TEST_OUTPUT}"
)

# Math
add_executable(TestMath test_math.cpp)
target_link_libraries(TestMath PRIVATE ${LIB_LINK_LIBRARIES})
target_compile_features(TestMath PRIVATE cxx_std_11)
gtest_discover_tests(TestMath)

if (POINTCLOUD_RASTER_LAS_SUPPORT)
    # LAS Read
    add_executable(TestReadLAS test_io_las.cpp)
    target_link_libraries(TestReadLAS PRIVATE ${LIB_LINK_LIBRARIES})
    target_compile_definitions(TestReadLAS PRIVATE ${LIB_DEFINITIONS})
    target_compile_features(TestReadLAS PRIVATE cxx_std_17)
    gtest_discover_tests(TestReadLAS)
endif ()

# TXT Read
add_executable(TestReadTXT test_io_txt.cpp)
target_link_libraries(TestReadTXT PRIVATE ${LIB_LINK_LIBRARIES})
target_compile_definitions(TestReadTXT PRIVATE ${LIB_DEFINITIONS})
target_compile_features(TestReadTXT PRIVATE cxx_std_17)
gtest_discover_tests(TestReadTXT)

# PNG
if (POINTCLOUD_RASTER_PNG_SUPPORT)
    # PNG Write
    add_executable(TestCoreImage test_core_image.cpp)
    target_link_libraries(TestCoreImage PRIVATE ${LIB_LINK_LIBRARIES})
    target_compile_definitions(TestCoreImage PRIVATE ${LIB_DEFINITIONS})
    target_compile_features(TestCoreImage PRIVATE cxx_std_17)
    gtest_discover_tests(TestCoreImage)
endif()

# PoincloudRasterizer
add_executable(TestRasterizer test_rasterizer.cpp)
target_link_libraries(TestRasterizer PRIVATE ${LIB_LINK_LIBRARIES})
target_compile_definitions(TestRasterizer PRIVATE ${LIB_DEFINITIONS})
target_compile_features(TestRasterizer PRIVATE cxx_std_11)
gtest_discover_tests(TestRasterizer)
