//
// Created by yuly on 06.04.23.
//

#include "reader/DateColumnReader.h"


DateColumnReader::DateColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {
	// TODO: implement
}

void DateColumnReader::close() {

}

void DateColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding & encoding, int offset,
                               int size, int pixelStride, duckdb::Vector vector,
                               pixels::proto::ColumnChunkIndex & chunkIndex) {
	auto result_ptr = duckdb::FlatVector::GetData<int>(vector);
	if(offset == 0) {
		decoder = std::make_shared<RunLenIntDecoder>(input, true);
	}
	if(encoding.kind() == pixels::proto::ColumnEncoding_Kind_RUNLENGTH) {
        for (int i = 0; i < size; i++) {
            result_ptr[i] = (int) decoder->next();
        }
	} else {
		memcpy(result_ptr, input->getPointer() + input->getReadPos(), size * sizeof(int));
		input->setReadPos(input->getReadPos() + size * sizeof(int));
	}
}
