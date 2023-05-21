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

void DecimalColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding & encoding, int offset,
                               int size, int pixelStride, duckdb::Vector vector,
                               pixels::proto::ColumnChunkIndex & chunkIndex) {
	auto result_ptr = duckdb::FlatVector::GetData<long>(vector);


//	if(type->getPrecision() != columnVector->getPrecision() || type->getScale() != columnVector->getScale()) {
//		throw InvalidArgumentException("reader of decimal(" + std::to_string(type->getPrecision())
//		                               + "," + std::to_string(type->getScale()) + ") doesn't match the column "
//									   "vector of decimal(" + std::to_string(columnVector->getPrecision()) + ","
//		                               + std::to_string(columnVector->getScale()) + ")");
//	}
    if(offset == 0) {
        // TODO: here we check null
        ColumnReader::elementIndex = 0;
    }
    // TODO: we didn't implement the run length encoded method
	memcpy(result_ptr, input->getPointer() + input->getReadPos(), size * sizeof(long));
	input->setReadPos(input->getReadPos() + size * sizeof(long));
//	for(int i = 0; i < size; i++) {
//		result_ptr[i] = input->getLong();
//	}
}
