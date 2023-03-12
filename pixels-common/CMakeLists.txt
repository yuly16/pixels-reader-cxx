project(pixels-common)

set(pixels_common_cxx
        lib/physical/storage/LocalFS.cpp
        lib/physical/Storage.cpp
        lib/physical/natives/PixelsRandomAccessFile.cpp
        lib/physical/natives/DirectRandomAccessFile.cpp
        lib/physical/natives/ByteBuffer.cpp
        lib/physical/io/PhysicalLocalReader.cpp
        lib/physical/StorageFactory.cpp lib/physical/Request.cpp lib/physical/RequestBatch.cpp lib/physical/scheduler/NoopScheduler.cpp lib/physical/SchedulerFactory.cpp)

add_library(pixels-common ${pixels_common_cxx})

include_directories(include)
