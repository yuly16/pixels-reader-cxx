//
// Created by liyu on 3/19/23.
//

#include "reader/IntegerColumnReader.h"

IntegerColumnReader::IntegerColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {
    isLong = false;
    // TODO: implement
}

void IntegerColumnReader::close() {
    // TODO: implement
}

void IntegerColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding & encoding, int offset,
                               int size, int pixelStride, int vectorIndex, std::shared_ptr<ColumnVector> vector,
                               pixels::proto::ColumnChunkIndex & chunkIndex) {
    std::shared_ptr<LongColumnVector> columnVector =
            std::static_pointer_cast<LongColumnVector>(vector);
    // if read from start, init the stream and decoder
    if(offset == 0) {
        ColumnReader::elementIndex = 0;
        if(encoding.kind() == pixels::proto::ColumnEncoding_Kind_NONE) {
            isLong = type->getCategory() == TypeDescription::Category::LONG;
        }
    }
    // TODO: we didn't implement the run length encoded method

    if(isLong) {
        // if long
        for(int i = 0; i < size; i++) {
            if(elementIndex % pixelStride == 0) {
                int pixelsId = elementIndex / pixelStride;
                // TODO: what is hasnull?
            }
            columnVector->vector[i + vectorIndex] = (long)input->getLong();
            elementIndex++;
        }
    } else {
        // if int
        for(int i = 0; i < size; i++) {
            if(elementIndex % pixelStride == 0) {
                int pixelsId = elementIndex / pixelStride;
                // TODO: what is hasnull?
            }
            columnVector->vector[i + vectorIndex] = input->getInt();
            elementIndex++;
        }
    }

}
