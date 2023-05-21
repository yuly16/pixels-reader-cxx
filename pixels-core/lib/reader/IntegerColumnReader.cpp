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
                               int size, int pixelStride, duckdb::Vector vector,
                               pixels::proto::ColumnChunkIndex & chunkIndex) {

    // if read from start, init the stream and decoder
    if(offset == 0) {
        decoder = std::make_shared<RunLenIntDecoder>(input, true);
		isLong = type->getCategory() == TypeDescription::Category::LONG;
    }
    if(encoding.kind() == pixels::proto::ColumnEncoding_Kind_RUNLENGTH) {
		if(isLong) {
			auto result_ptr = duckdb::FlatVector::GetData<long>(vector);
			for (int i = 0; i < size; i++) {
				result_ptr[i] = decoder->next();
			}
		} else {
			auto result_ptr = duckdb::FlatVector::GetData<int>(vector);
			for (int i = 0; i < size; i++) {
				result_ptr[i] = decoder->next();
			}
		}
    } else {
        if(isLong) {
            // if long
			auto result_ptr = duckdb::FlatVector::GetData<long>(vector);
			memcpy(result_ptr, input->getPointer() + input->getReadPos(), size * sizeof(long));
			input->setReadPos(input->getReadPos() + size * sizeof(long));
        } else {
            // if int
			auto result_ptr = duckdb::FlatVector::GetData<int>(vector);
			memcpy(result_ptr, input->getPointer() + input->getReadPos(), size * sizeof(int));
			input->setReadPos(input->getReadPos() + size * sizeof(int));
        }
    }
}
