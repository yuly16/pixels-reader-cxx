//
// Created by liyu on 3/20/23.
//

#include "reader/StringColumnReader.h"

StringColumnReader::StringColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {
    bufferOffset = 0;
	starts = nullptr;
	orders = nullptr;
}

void StringColumnReader::close() {

}

void StringColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding & encoding, int offset,
                              int size, int pixelStride, duckdb::Vector vector,
                              pixels::proto::ColumnChunkIndex & chunkIndex) {
    if(offset == 0) {
        elementIndex = 0;
        bufferOffset = 0;
        readContent(input, input->bytesRemaining(), encoding);
    }
    // TODO: support dictionary
	auto result_ptr = duckdb::FlatVector::GetData<duckdb::string_t>(vector);

    // TODO: if dictionary encoded
	if (encoding.kind() == pixels::proto::ColumnEncoding_Kind_DICTIONARY) {
		for(int i = 0; i < size; i++) {
			int originId = orders[(int) contentDecoder->next()];
			int tmpLen = starts[originId + 1] - starts[originId];
			// use setRef instead of setVal to reduce memory copy.
			result_ptr[i] = duckdb::string_t((char *)(originsBuf->getPointer() + starts[originId]), tmpLen);
			elementIndex++;
		}
	} else {
		for(int i = 0; i < size; i++) {
			int len = (int) lensDecoder->next();
			// use setRef instead of setVal to reduce memory copy
			result_ptr[i] = duckdb::string_t((char *)(contentBuf->getPointer() + bufferOffset), len);
			bufferOffset += len;
			elementIndex++;
		}
	}

}

void StringColumnReader::readContent(std::shared_ptr<ByteBuffer> input,
                                     uint32_t inputLength,
                                     pixels::proto::ColumnEncoding & encoding) {
    if(encoding.kind() == pixels::proto::ColumnEncoding_Kind_DICTIONARY) {
        input->markReaderIndex();
        input->skipBytes(inputLength - 3 * sizeof(int));
        originsOffset = input->getInt();
        startsOffset = input->getInt();
        int ordersOffset = input->getInt();
        input->resetReaderIndex();
        // read buffers
        contentBuf = std::make_shared<ByteBuffer>(*input, 0, originsOffset);
		originsBuf = std::make_shared<ByteBuffer>(
		    *input, originsOffset, startsOffset - originsOffset);
		std::shared_ptr<ByteBuffer> startsBuf = std::make_shared<ByteBuffer>(
		    *input, startsOffset, ordersOffset - startsOffset);
		std::shared_ptr<ByteBuffer> ordersBuf = std::make_shared<ByteBuffer>(
		    *input, ordersOffset, inputLength - ordersOffset);
		int bufferStart = 0;
		std::shared_ptr<RunLenIntDecoder> startsDecoder =
		    std::make_shared<RunLenIntDecoder>(startsBuf, false);
		if(encoding.has_dictionarysize()) {
			startsLength = encoding.dictionarysize() + 1;
			starts = new int[startsLength];
			int i = 0;
			while (startsDecoder->hasNext()) {
				starts[i++] = bufferStart + (int) startsDecoder->next();
			}
			starts[i] = bufferStart + startsOffset - originsOffset;
		} else {
            throw InvalidArgumentException("StringColumnReader::readContent: dictionary size must  be defined. ");
		}
		int originNum = startsLength - 1;
		std::shared_ptr<RunLenIntDecoder> ordersDecoder =
		    std::make_shared<RunLenIntDecoder>(ordersBuf, false);
		orders = new int[originNum];
		for (int i = 0; i < originNum && ordersDecoder->hasNext(); i++)
		{
			orders[i] = (int) ordersDecoder->next();
		}
		contentDecoder = std::make_shared<RunLenIntDecoder>(contentBuf, false);
    } else {
        input->markReaderIndex();
        input->skipBytes(inputLength - sizeof(int));
        int lensOffset = input->getInt();
        input->resetReaderIndex();
        // read strings
        // TODO: isDirect
        contentBuf = std::make_shared<ByteBuffer>(*input, 0, lensOffset);
        std::shared_ptr<ByteBuffer> lensBuf = std::make_shared<ByteBuffer>(
                *input, lensOffset, inputLength - sizeof(int) - lensOffset);
        lensDecoder = std::make_shared<RunLenIntDecoder>(lensBuf, false);
    }
}
StringColumnReader::~StringColumnReader() {
	if(starts != nullptr) {
		delete[] starts;
	}
	if(orders != nullptr) {
		delete[] orders;
	}
}
