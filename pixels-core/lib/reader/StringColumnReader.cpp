//
// Created by liyu on 3/20/23.
//

#include "reader/StringColumnReader.h"

StringColumnReader::StringColumnReader(std::shared_ptr<TypeDescription> type) : ColumnReader(type) {
    bufferOffset = 0;
}

void StringColumnReader::close() {

}

void StringColumnReader::read(std::shared_ptr<ByteBuffer> input, pixels::proto::ColumnEncoding encoding, int offset,
                              int size, int pixelStride, int vectorIndex, std::shared_ptr<ColumnVector> vector,
                              pixels::proto::ColumnChunkIndex chunkIndex) {
    if(offset == 0) {
        elementIndex = 0;
        bufferOffset = 0;
        readContent(input, input->bytesRemaining(), encoding);
    }
    // TODO: support dictionary
    std::shared_ptr<BinaryColumnVector> columnVector =
            std::static_pointer_cast<BinaryColumnVector>(vector);
    // TODO: if dictionary encoded
    for(int i = 0; i < size; i++) {
        if(elementIndex % pixelStride == 0) {
            int pixelId = elementIndex / pixelStride;
            // TODO: should write the remaining code
        }
        int len = (int) lensDecoder->next();
        // use setRef instead of setVal to reduce memory copy
        columnVector->setRef(i + vectorIndex, contentBuf->getPointer(), bufferOffset, len);
        bufferOffset += len;
        elementIndex++;
    }
}

void StringColumnReader::readContent(std::shared_ptr<ByteBuffer> input,
                                     uint32_t inputLength,
                                     pixels::proto::ColumnEncoding encoding) {
    // TODO: implement dictionary encoding
    input->markReaderIndex();
    input->setReadPos(inputLength - sizeof(int));
    int lensOffset = input->getInt();
    input->resetReaderIndex();
    // read strings
    // TODO: isDirect
    contentBuf = std::make_shared<ByteBuffer>(*input, 0, lensOffset);
    std::shared_ptr<ByteBuffer> lensBuf = std::make_shared<ByteBuffer>(
            *input, lensOffset, inputLength - sizeof(int) - lensOffset);
    lensDecoder = std::make_shared<RunLenIntDecoder>(lensBuf, false);

}
