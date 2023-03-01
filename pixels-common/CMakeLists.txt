project(pixels-common)

set(pixels_common_cxx
        lib/physical/storage/LocalFS.cpp lib/physical/Storage.cpp)

add_library(pixels-common ${pixels_common_cxx})
include_directories(include)