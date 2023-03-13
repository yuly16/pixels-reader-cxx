project(pixels-core)

set(pixels_core_cxx
        lib/PixelsReaderImpl.cpp
        lib/PixelsReaderBuilder.cpp
        lib/reader/PixelsRecordReaderImpl.cpp lib/PixelsVersion.cpp)

add_library(pixels-core ${pixels_core_cxx})

target_link_libraries(
        pixels-core
        pixels-common
)

include_directories(${CMAKE_SOURCE_DIR}/pixels-common/include)
include_directories(include)