//
// Created by liyu on 3/19/23.
//

#include "reader/CharColumnReader.h"

CharColumnReader::CharColumnReader(std::shared_ptr<TypeDescription> type)
    :ColumnReader(type) {
    // TODO: implement
}

void CharColumnReader::close() {
    // TODO: implement
}

void CharColumnReader::read(std::shared_ptr<ByteBuffer> input,
                            pixels::proto::ColumnEncoding encoding,
                            int offset, int size, int pixelStride,
                            int vectorIndex, std::shared_ptr<ColumnVector> vector,
                            pixels::proto::ColumnChunkIndex chunkIndex) {
    // TODO: implement
}
