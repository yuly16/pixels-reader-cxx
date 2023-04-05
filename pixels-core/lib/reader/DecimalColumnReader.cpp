//
// Created by yuly on 05.04.23.
//

#include "reader/DecimalColumnReader.h"

/**
 * The column reader of decimals.
 * <p><b>Note: it only supports short decimals with max precision and scale 18.</b></p>
 * @author hank
 */
DecimalColumnReader::DecimalColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {

}

void DecimalColumnReader::close() {

}

void DecimalColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding encoding, int offset,
                               int size, int pixelStride, int vectorIndex, std::shared_ptr<ColumnVector> vector,
                               pixels::proto::ColumnChunkIndex chunkIndex) {
    std::shared_ptr<DecimalColumnVector> columnVector =
            std::static_pointer_cast<DecimalColumnVector>(vector);
    // TODO: type precision and type scale should be the same as the column vector

    if(offset == 0) {
        // TODO: here we check null
        ColumnReader::elementIndex = 0;
    }
    // TODO: we didn't implement the run length encoded method

    for(int i = 0; i < size; i++) {
        if(elementIndex % pixelStride == 0) {
            int pixelsId = elementIndex / pixelStride;
            // TODO: deal with null
        }
        columnVector->vector[i + vectorIndex] = input->getLong();
        elementIndex++;
    }

}
