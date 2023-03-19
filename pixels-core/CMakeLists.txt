project(pixels-core)

set(pixels_core_cxx
        lib/PixelsReaderImpl.cpp
        lib/PixelsReaderBuilder.cpp
        lib/reader/PixelsRecordReaderImpl.cpp
        lib/PixelsVersion.cpp
        lib/PixelsFooterCache.cpp
        lib/exception/PixelsReaderException.cpp
        lib/exception/PixelsFileMagicInvalidException.cpp
        lib/exception/PixelsFileVersionInvalidException.cpp
        lib/reader/PixelsReaderOption.cpp
        lib/TypeDescription.cpp
        lib/Category.cpp
        lib/vector/LongColumnVector.cpp
        lib/vector/VectorizedRowBatch.cpp
        lib/vector/ColumnVector.cpp
        lib/vector/ByteColumnVector.cpp
        lib/vector/BinaryColumnVector.cpp lib/reader/ColumnReader.cpp lib/reader/ColumnReaderBuilder.cpp lib/reader/IntegerColumnReader.cpp lib/reader/CharColumnReader.cpp lib/reader/VarcharColumnReader.cpp)

add_library(pixels-core ${pixels_core_cxx})

target_link_libraries(
        pixels-core
        pixels-common
)

include_directories(${CMAKE_SOURCE_DIR}/pixels-common/include)
include_directories(include)