project(pixels-common)

set(pixels_common_cxx
        lib/physical/storage/LocalFS.cpp
        lib/physical/Storage.cpp
        lib/physical/natives/PixelsRandomAccessFile.cpp
        lib/physical/natives/DirectRandomAccessFile.cpp
        lib/physical/natives/ByteBuffer.cpp
        lib/physical/io/PhysicalLocalReader.cpp
        lib/physical/StorageFactory.cpp lib/physical/Request.cpp lib/physical/RequestBatch.cpp lib/physical/scheduler/NoopScheduler.cpp lib/physical/SchedulerFactory.cpp)

include_directories(include)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto/pixels.proto)

add_library(pixels-common ${pixels_common_cxx} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(pixels-common ${Protobuf_LIBRARIES})