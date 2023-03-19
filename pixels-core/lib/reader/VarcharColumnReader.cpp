//
// Created by liyu on 3/19/23.
//

#include "reader/VarcharColumnReader.h"

VarcharColumnReader::VarcharColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {
    // TODO: implement
}

void VarcharColumnReader::close() {
    // TODO: implement
}

void VarcharColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding encoding, int offset,
                               int size, int pixelStride, int vectorIndex, std::shared_ptr<ColumnVector> vector,
                               pixels::proto::ColumnChunkIndex chunkIndex) {
    // TODO: implement
}
