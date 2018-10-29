# Shared files
set(DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(LAS_FILE_SIMPLE_INPUT "${DATA_PATH}/cloud-dummy.las")
set(LIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../lib")
set(LIB_TARGET PointcloudRaster)

# LAS Read
add_executable(TestReadLAS test_read_las)
target_include_directories(TestReadLAS
    PRIVATE
        ${INCLUDE_DIRS}
)
target_link_libraries(TestReadLAS
    PRIVATE
        ${LIB_TARGET}
)
target_compile_definitions(TestReadLAS
    PRIVATE
        LAS_FILE_SIMPLE_INPUT="${LAS_FILE_SIMPLE_INPUT}"
)

add_test(NAME ReadLAS COMMAND TestReadLAS)

